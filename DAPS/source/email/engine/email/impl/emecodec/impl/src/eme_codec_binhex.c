
#include "eme_codec_binhex.h"
#include "oem_asyncproc_api.h"

#ifdef AEE_SIMULATOR
   #define CRASH(P) EME_UTILS_MEMSET(NULL, 0, 2)
#endif

const WE_INT d_tbl[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,-1,-1,13,14,15,16,17,18,19,-1,20,21,-1,-1,-1,-1,-1,
					-1,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,-1,37,38,39,40,41,42,43,-1,44,45
					,46,47,-1,-1,-1,-1,48,49,50,51,52,53,54,-1,55,56,57,58,59,60,-1,-1,61,62,63};
#if 0
    const WE_CHAR * _tbl = "!\"#$%&'()*+,-012345689@ABCDEFGHIJKLMNPQRSTUVXYZ[`abcdefhijklmpqr";
#endif

    
typedef struct
{
   WE_INT pos;
   WE_INT len;
   byte g_dest[3];
   WE_CHAR acRes;
} BinHex;

typedef struct 
{
   GET from;
   WE_VOID* pf;
   WRITE to;
   WE_VOID* pt;

   WE_INT len;
	WE_INT pre;
	WE_INT cx;
   WE_INT32 datafork_len;

   WE_INT result;//0: init state, <0 : error, 1: success, 2: failed.
   BinHex binhex;
   WE_VOID* pvCBMgr;
   
   BH_NOTIFY notify;
   WE_VOID* pNotify;
} St_BinHex;



void parser(byte src[], byte dest[])
{			
   WE_INT i;
	for(i = 0; i< 4; i++)
	{
#if 0
		for(WE_INT j = 0; j < (WE_INT)EME_UTILS_STRLEN(_tbl); ++j)
		{
			if(_tbl[j] == (WE_CHAR)src[i])
				src[i] = (byte)j;
		}
#else
		src[i] = (byte)(d_tbl[(WE_INT)src[i] - (WE_INT)'!']);
#endif
	}
	//parse...........
	dest[0] = (src[0]<<2) | ((src[1]) >> 4);
	dest[1] = (src[1]<<4 & 0xf0) | (src[2] >> 2);
	dest[2] = (src[2] << 6) | (src[3]);		
}


WE_INT decodeget(GET from,void* p,BinHex* pMe)
{
	if(pMe->pos == pMe->len)
	{
		byte tmp_src[4];
		WE_INT c;
		WE_INT i = 0;
		EME_UTILS_MEMSET(tmp_src,0, 4);
		while(i < 4)
		{
			while(TRUE)
			{
				c = from(p);
				if(c != '\r' && c != '\n')
					break;
			}

			if(c == ':')
			{
				if(i == 0)
					return -1;			
				else
					break;
			}
			tmp_src[i] = (byte)c;
			++i;
		}
		parser(tmp_src, pMe->g_dest);
		pMe->len = 3;
		pMe->pos = 0;
	}
	
	return (WE_INT)(pMe->g_dest[pMe->pos++]);
}

WE_INT read(GET from,void* p, WE_CHAR buf[], WE_INT len,BinHex *pMe)
{
	WE_INT c;
   WE_INT i;
	for(i = 0; i < len ; ++i)
	{
		c = decodeget(from,p,pMe);
		if(c == -1)
			return i;
		buf[i] = (WE_CHAR)(c);
	}

	return len;
}


//maxlen 
WE_BOOL parseHead(GET from,void*p, WE_CHAR patternStr[])
{
	WE_INT c;
   WE_INT j = 0;

	while((c = (WE_INT)from(p)) != '(')
	{}

	while((c = (WE_INT)from(p)) != ')' && j < (WE_INT)EME_UTILS_STRLEN(patternStr))
	{
		if(c == patternStr[j])
			++j;
		else
			j = 0;
	}

	if( j == (WE_INT)EME_UTILS_STRLEN(patternStr))
		return TRUE;

	return FALSE;
}

void reverse(byte buf[] ,WE_INT len)
{
	if(len%2 == 0)
	{
      WE_INT i;
		for(i = 0; i< len/2; ++i)
		{
         byte bTmp = buf[i];
         buf[i] = buf[len - i - 1];
         buf[len - i - 1] = bTmp;
			//swap(buf[i],buf[len - i - 1]);
		}
	}
#ifdef AEE_SIMULATOR
	else
		CRASH(NULL);
#endif
}


WE_VOID realParse(void* pUser)
{
   St_BinHex* pMe = (St_BinHex*)pUser;  
   
   if(pMe->len == pMe->datafork_len)
      pMe->result = 1;

   if(pMe->result >= 1)
   {
      //notify success
      if(pMe->pNotify != NULL)   
         pMe->notify(pMe->pNotify, TRUE);
      EME_UTILS_FREE(NULL, pUser);
      return;
   }
   else if(pMe->result < 0)
   {
      //notify failed
      if(pMe->pNotify != NULL)
         pMe->notify(pMe->pNotify, FALSE);
      EME_UTILS_FREE(NULL, pUser);
      return;
   }

	while(pMe->len <= pMe->datafork_len)
	{
		pMe->pre = pMe->cx;
		pMe->cx = decodeget(pMe->from,pMe->pf,&pMe->binhex);
#if 0
		pMe->to(pt,cx);
#else
		if(pMe->cx == -1)
      {
         pMe->result = -1;         
         OEM_AsyncProc_AddProc(pMe->pvCBMgr, realParse,pUser);
         break;
      }
		
		if(pMe->cx == 0x90)
		{
			pMe->pre = pMe->cx;
			pMe->cx = decodeget(pMe->from,pMe->pf,&pMe->binhex);
			if(pMe->cx == 0)
         {
            ++pMe->len;
				pMe->to(pMe->pt,0x90);
         }
			else
			{
				if(pMe->pre == -1)
            {
               pMe->result = -1;               
               OEM_AsyncProc_AddProc(pMe->pvCBMgr, realParse,pUser);
               break;               
            }
				else
				{
               WE_INT z;
					for(z = 0; z < pMe->cx - 1; ++z)
               {
                  ++pMe->len;
                  pMe->to(pMe->pt, pMe->pre);
               }
						
				}
			}

		}
		else
		{
         ++pMe->len;
			pMe->to(pMe->pt,pMe->cx);
		}

      if((pMe->len) % (48 * 1024) == 0)
      {
         OEM_AsyncProc_AddProc(pMe->pvCBMgr, realParse,pUser);
         break;
      }
      
      if(pMe->len == pMe->datafork_len)
      {
         pMe->result = 1;
         if(pMe->pNotify != NULL)
         {
            pMe->notify(pMe->pNotify, TRUE);
         }
         EME_UTILS_FREE(NULL, pUser);
         break;
      }
      
#endif
	}
}

WE_BOOL parseBinHex(void* pvCBMgr, GET from, void* pf,WRITE to,void* pt,BH_NOTIFY notify,WE_VOID* pNotify)
{
	WE_INT read_len = 0;
	WE_CHAR *patternStr = "BinHex 4.0";
   WE_INT c;

   WE_INT filename_len = 0;
	WE_CHAR* filename = NULL;
	
   WE_INT32 datafork_len = 0;

   
   St_BinHex * pUser = NULL;
   BinHex binHex = {0};

   if(!parseHead(from,pf,patternStr))
		return FALSE;
	
	while((c = (WE_INT)from(pf)) != (WE_INT)':')
	{}

	filename_len = decodeget(from,pf,&binHex);
	filename = (WE_CHAR*)EME_UTILS_MALLOC(NULL,filename_len + 1);
   if(NULL == filename)
   {
      return FALSE;
   }
   EME_UTILS_MEMSET(filename,0, filename_len + 1);
	read_len = read(from, pf,filename, filename_len,&binHex);
	if(read_len != filename_len)
		goto err1;
	//cout<<"FileName_len:"<<filename_len<<endl;
	//cout<<"FileName:"<<filename<<endl;
	//SHould rename this file.
   /*{
      
		ofstream* of = (ofstream*)pt;
		of->open(filename);
		if(!of->is_open())
			goto err1;
	}*/
   {
	   WE_INT version = decodeget(from,pf,&binHex);
	   WE_INT32 type;
      WE_INT32 creator;
	   word flags;
      read_len = read(from,pf, (WE_CHAR*)&type,sizeof(WE_INT32),&binHex);
	   if(read_len != sizeof(WE_INT32))
		   goto err1;
	   //cout<<"Version:"<<version<<endl;
	   reverse((byte*)&type, sizeof(WE_INT32));
	   //cout<<"Type:"<<type<<endl;

	   read_len = read(from, pf, (WE_CHAR*)&creator,sizeof(WE_INT32),&binHex);
	   if(read_len != sizeof(WE_INT32))
		   goto err1;
	   reverse((byte*)&creator, sizeof(WE_INT32));
	   //cout<<"Creator:"<<creator<<endl;
	   
	   read_len = read(from,pf, (WE_CHAR*)&flags,sizeof(word),&binHex);
	   if(read_len != sizeof(word))
		   goto err1;
	   reverse((byte*)&flags, sizeof(word));
	   //cout<<"Flags:"<<flags<<endl;

      version = version;
   }
	
	read_len = read(from, pf,(WE_CHAR*)&datafork_len,sizeof(WE_INT32),&binHex);
	if(read_len != sizeof(WE_INT32))
		goto err1;
	reverse((byte*)&datafork_len, sizeof(WE_INT32));
	//cout<<"DataFork_len:"<<datafork_len<<endl;

   {
      WE_INT32 resourcefork_len;
	   read_len = read(from, pf,(WE_CHAR*)&resourcefork_len,sizeof(WE_INT32),&binHex);
	   if(read_len != sizeof(WE_INT32))
		   goto err1;
	   reverse((byte*)&resourcefork_len, sizeof(WE_INT32));
	   //cout<<"ResourceFork_Len:"<<resourcefork_len<<endl;
   }
	
   {
      word crc1;
	   read_len = read(from, pf, (WE_CHAR*)&crc1,sizeof(word),&binHex);
	   if(read_len != sizeof(word))
		   goto err1;
	   reverse((byte*)&crc1,sizeof(word));
	   //cout<<"Crc1:"<<crc1<<endl;
   }
	
   {
      pUser = (St_BinHex*)EME_UTILS_MALLOC(NULL, sizeof(St_BinHex));
      if(!pUser)
         goto err1;
      EME_UTILS_MEMSET(pUser, 0x00, sizeof(St_BinHex));

      pUser->from = from;
      pUser->pf = pf;
      pUser->pt = pt;
      pUser->to = to;
      pUser->result = 0;
      pUser->len = 0;
      pUser->cx = -1;
      pUser->datafork_len = datafork_len;
      pUser->binhex = binHex;
      pUser->pvCBMgr = pvCBMgr;
      pUser->notify = notify;
      pUser->pNotify = pNotify;
      realParse(pUser);
   }

#if 0
	word crc2;
	read_len = read(from, pf, (WE_CHAR*)&crc2,sizeof(word),&binHex);
	if(read_len != sizeof(word))
		goto err1;
	reverse((byte*)&crc1,sizeof(word));
	//cout<<"Crc2:"<<crc2<<endl;
#endif

	EME_UTILS_FREE(NULL,filename);
	return TRUE;

err1:
	EME_UTILS_FREEIF(NULL,filename)
   //pUser->result = -2;
	//cerr<<"Some error occours"<<endl;
	return FALSE;
}
