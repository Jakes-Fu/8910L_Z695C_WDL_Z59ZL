#include "std_header.h"
#ifdef KURO_SUPPORT
/********************************************************************************
* File: kdbparser.c
* By  : cmzs
* Version: 1> v1.00     first version     2007-09-10 8:28
*******************************************************************************
*/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
//#include "mmimultim.h"
#include "mmiidle_subwintab.h"
#include "mmicc_export.h"
#include "mmi_default.h"
#include "bswap.h"
#include "kdbparser.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define KDB_LANG_COUNT 4   //the max language counts that  kdb supported
#define KDB_SONG_MAX_COUNT 500
#define KDB_TYPES_MAX_COUNT 500
#define KDB_CLASS_MAX_COUNT 200

#define KDB_TAG_BLOCKLEN_SIZE 4
#define KDB_TAG_SONGCOUNT_SIZE 2
#define KDB_TAG_LINELEH_SIZE 2
#define KDB_LIST_BUFFER_SIZE   (10 * 1024)
#define KDB_SONG_PATH_BUF_SIZE 512
#define KDB_LANG_TAG_SIZE 3
#define KDB_TMP_BUFFER_LEN 256
#define MMIKDB_DEFAULT_DISK 'E'
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMIFILE_HANDLE fp_kdb = 0;
LOCAL int *kdb_block_off = {0};
LOCAL int *kdb_block_len = {0};
LOCAL int *kdb_songs_off = {0};
LOCAL int *kdb_types_off = {0};//用于记录每项type项在文件中的偏移,最后一项指向0
LOCAL int *kdb_classes_type_index = {0}; //用于指向是曲风类型的kdb_types_off

LOCAL uint16 *songid = {0};   //用于临时记录songid的数组
LOCAL uint16 kdb_songs_count = 0;
LOCAL int kdb_types_count = 0;
LOCAL int kdb_file_len = 0;
LOCAL char *kdb_list_buf = {0};
LOCAL uint32 s_nTmpLen = 0;
LOCAL int s_kdb_classes_count = 0;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

static const char *KDB_LANG_TAG[]=	
	{
		"LRC",
		"LR1",
		"LR2",
		"LR3",
	};


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : alloc memory for kdb parse
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL void MMIKDB_allocmemory(void);
/*****************************************************************************/
// 	Description :free memory for kdb parse
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL void MMIKDB_freememory(void);
/*****************************************************************************/
// 	Description :根据KDB文件中的语言tag获取某种语言序号
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL int MMIKDB_getlangtagid(char *tagname);
/*****************************************************************************/
// 	Description :获取所有语言在文件中的偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getalllangoffset(MMIFILE_HANDLE fp_file);

/*****************************************************************************/
// 	Description :获取所有歌曲信息的偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getallsongsoffset(int ntagid,MMIFILE_HANDLE fp_file);
/*****************************************************************************/
// 	Description :获取所有子分类在文件中的位置(歌手-->张学友，专辑-->某某...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getalltypesoffset(MMIFILE_HANDLE fp_file,int noffsetbegin,int nblocklen);
/*****************************************************************************/
// 	Description :获取所有分类在文件中的位置(歌手，专辑...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getallclassesoffset(MMIFILE_HANDLE fp_file);
/*****************************************************************************/
// 	Description :分析kdb文件，获取所有分类及歌曲的文件偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getkdbinfos(int ntagid,MMIFILE_HANDLE fp_file);
/*****************************************************************************/
// 	Description :获取某一子类的名称(如 歌手,张学友，则返回张学友,如空则返回NULL)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL char * MMIKDB_getclasstypename(int ntypeid);

/*****************************************************************************/
// 	Description : _allocmemory
//	Global resource dependence : none
//  Author: cmzs
//	Note:
/*****************************************************************************/
LOCAL void MMIKDB_allocmemory(void)
{
	if(kdb_block_off == PNULL)
    {    
		kdb_block_off = (int*)SCI_ALLOC_APP(sizeof(int) * KDB_LANG_COUNT);
    }
	if(kdb_block_len == PNULL)
    {    
		kdb_block_len = (int *)SCI_ALLOC_APP(sizeof(int) * KDB_LANG_COUNT);
    }
	if(kdb_songs_off == PNULL)
    {    
		kdb_songs_off = (int *)SCI_ALLOC_APP(sizeof(int) * KDB_SONG_MAX_COUNT);
    }
	if(kdb_types_off == PNULL)
    {    
		kdb_types_off = (int *)SCI_ALLOC_APP(sizeof(int) * KDB_TYPES_MAX_COUNT);
    }
	if(kdb_classes_type_index == PNULL)
    {    
		kdb_classes_type_index = (int *)SCI_ALLOC_APP(sizeof(int) * KDB_CLASS_MAX_COUNT);
    }
	if(songid == PNULL)
    {    
		songid = (uint16 *)SCI_ALLOC_APP(sizeof(uint16) * KDB_SONG_MAX_COUNT);
    }
	if(kdb_list_buf == PNULL)
    {    
		kdb_list_buf = (char *)SCI_ALLOC_APP(KDB_LIST_BUFFER_SIZE);
    }
}

/*****************************************************************************/
// 	Description : _freememory
//	Global resource dependence : none
//  Author: cmzs
//	Note:
/*****************************************************************************/
LOCAL void MMIKDB_freememory(void)
{
	if(kdb_block_off != PNULL)
	{
		SCI_FREE(kdb_block_off);
		kdb_block_off = 0;
	}
	if(kdb_block_len != PNULL)
	{
		SCI_FREE(kdb_block_len);
		kdb_block_len = 0;
	}
	if(kdb_songs_off != PNULL)
	{
		SCI_FREE(kdb_songs_off);
		kdb_songs_off = 0;
	}
	if(kdb_types_off != PNULL)
	{
		SCI_FREE(kdb_types_off);
		kdb_types_off = 0;
	}
	if(kdb_classes_type_index != PNULL)
	{
		SCI_FREE(kdb_classes_type_index);
		kdb_classes_type_index = 0;
	}
	if(songid != PNULL)
	{
		SCI_FREE(songid);
		songid = 0;
	}
	if(kdb_list_buf != PNULL)
	{
		SCI_FREE(kdb_list_buf);
		kdb_list_buf = 0;
	}
}

/*****************************************************************************/
// 	Description : _getlangtagid
//	Global resource dependence : none
//  Author: cmzs
//	Note:
/*****************************************************************************/
LOCAL int MMIKDB_getlangtagid(char *tagname)
{
	int nRet = -1;
	uint32 i = 0;
	for(i = 0; i < KDB_LANG_COUNT; i++)
	{
		if(strcmp(tagname,KDB_LANG_TAG[i]) == 0)
		{
			nRet = i;
			break;
		}
	}
	return nRet;
}

/*****************************************************************************/
// 	Description :获取所有语言在文件中的偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getalllangoffset(MMIFILE_HANDLE fp_file)   
{
	int ncuroffset = 0,nblocklength = 0,nlangtagid = 0;
    char langtag[KDB_LANG_TAG_SIZE + 1] = {0};

	if(SFS_INVALID_HANDLE == fp_file)
    {    
		return FALSE;
    }

	
	MMIAPIFMM_SetFilePointer(fp_file,0,MMIFILE_SEEK_BEGIN);
	while(1)
	{
		s_nTmpLen = KDB_LANG_TAG_SIZE;
		MMIAPIFMM_ReadFile(fp_file, langtag,s_nTmpLen,&s_nTmpLen, PNULL);
		langtag[KDB_LANG_TAG_SIZE] = '\0';
		nlangtagid = MMIKDB_getlangtagid(langtag);
		if(nlangtagid >= 0)
		{
			kdb_block_off[nlangtagid] = ncuroffset + KDB_LANG_TAG_SIZE;
			s_nTmpLen = sizeof(nblocklength);
			MMIAPIFMM_ReadFile(fp_file, &nblocklength,s_nTmpLen,&s_nTmpLen, PNULL);
			nblocklength = INT4_H2N(nblocklength);
			kdb_block_len[nlangtagid] = nblocklength;
			if(nblocklength + ncuroffset + KDB_LANG_TAG_SIZE >= kdb_file_len)
            {            
				break;
            }
			MMIAPIFMM_SetFilePointer(fp_kdb,nblocklength,MMIFILE_SEEK_CUR);
			//ncuroffset = S_FS_FTell(fp_file);
			ncuroffset = MMIAPIFMM_GetFilePointer(fp_file, MMIFILE_SEEK_BEGIN);
		}
		else
		{
			break;
		}
	}
	return TRUE;
}
/*****************************************************************************/
// 	Description :获取所有歌曲信息的偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getallsongsoffset(int ntagid,MMIFILE_HANDLE fp_file)
{
	int nblocklen = 0,ncuroffset = 0;
    uint32 i = 0;
	uint16 nsonglen = 0;
	
	if(kdb_block_off[ntagid] <= 0)
    {    
		return FALSE;
    }
	if(fp_file == 0)
    {    
		return FALSE;
    }
	
	MMIAPIFMM_SetFilePointer(fp_file,kdb_block_off[ntagid],MMIFILE_SEEK_BEGIN);

	s_nTmpLen = sizeof(nblocklen);
	MMIAPIFMM_ReadFile(fp_file, &nblocklen,s_nTmpLen,&s_nTmpLen, PNULL);
	nblocklen = INT4_H2N(nblocklen);

	s_nTmpLen = sizeof(kdb_songs_count);
	MMIAPIFMM_ReadFile(fp_file, &kdb_songs_count,s_nTmpLen,&s_nTmpLen, PNULL);
	kdb_songs_count = INT2_H2N(kdb_songs_count);
	if(kdb_songs_count > KDB_SONG_MAX_COUNT)
    {    
		kdb_songs_count = KDB_SONG_MAX_COUNT;
    }
	ncuroffset = kdb_block_off[ntagid] + KDB_TAG_BLOCKLEN_SIZE + KDB_TAG_SONGCOUNT_SIZE;
	for(i = 0; i < kdb_songs_count; i++)
	{
		kdb_songs_off[i] = ncuroffset;

		s_nTmpLen = KDB_TAG_LINELEH_SIZE;
		MMIAPIFMM_ReadFile(fp_file, &nsonglen,s_nTmpLen,&s_nTmpLen, PNULL);
		nsonglen = INT2_H2N(nsonglen);

		ncuroffset += nsonglen;
		MMIAPIFMM_SetFilePointer(fp_kdb,nsonglen - KDB_TAG_LINELEH_SIZE, MMIFILE_SEEK_CUR);
	}
	return TRUE;
}
/*****************************************************************************/
// 	Description :获取所有子分类在文件中的位置(歌手-->张学友，专辑-->某某...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getalltypesoffset(MMIFILE_HANDLE fp_file,int noffsetbegin,int nblocklen)
{
	int nreadlen = 0,nindex = 0,ncuroffset = 0;
	uint16 nlinelen = 0;
	if(fp_file == 0)
    {   
		return FALSE;
    }

	MMIAPIFMM_SetFilePointer(fp_file,noffsetbegin,MMIFILE_SEEK_BEGIN);
	ncuroffset = noffsetbegin;
	
	while(1)
	{
		s_nTmpLen = KDB_TAG_LINELEH_SIZE;
		MMIAPIFMM_ReadFile(fp_file, &nlinelen,s_nTmpLen,&s_nTmpLen, PNULL);
		nlinelen = INT2_H2N(nlinelen);
		
		nreadlen += nlinelen;
		if(nreadlen > nblocklen)
        {        
			break;
        }
		kdb_types_off[nindex++] = ncuroffset;
		if(nindex >= KDB_TYPES_MAX_COUNT)
        {        
			break;
        }
		ncuroffset += nlinelen;
		MMIAPIFMM_SetFilePointer(fp_file,nlinelen - 2,MMIFILE_SEEK_CUR);
	}
	kdb_types_count = nindex;
	return TRUE;
}

/*****************************************************************************/
// 	Description :获取所有分类在文件中的位置(歌手，专辑...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getallclassesoffset(MMIFILE_HANDLE fp_file)
{
	uint32 nIndex = 0;
    int i = 0;
	char chRead = 0;
	if(kdb_types_count == 0)
    {
		return FALSE;
    }
	for(i = 0; i < kdb_types_count; i++)
	{
		MMIAPIFMM_SetFilePointer(fp_file,kdb_types_off[i] + KDB_TAG_LINELEH_SIZE,MMIFILE_SEEK_BEGIN);
		
		s_nTmpLen = 1;
		MMIAPIFMM_ReadFile(fp_file, &chRead,s_nTmpLen,&s_nTmpLen, PNULL);

		if(chRead != ',')
		{
			kdb_classes_type_index[nIndex++] = i;//kdb_types_off[i];
		}
    }
	s_kdb_classes_count = nIndex;
	return TRUE;
}
/*****************************************************************************/
// 	Description :分析kdb文件，获取所有分类及歌曲的文件偏移量
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKDB_getkdbinfos(int ntagid,MMIFILE_HANDLE fp_file)
{
	int ncuroffset = 0;
	if(MMIKDB_getalllangoffset(fp_file))
	{
		if(MMIKDB_getallsongsoffset(ntagid,fp_file))
		{
			ncuroffset = MMIAPIFMM_GetFilePointer(fp_file, MMIFILE_SEEK_BEGIN);//S_FS_FTell(fp_file);
			MMIKDB_getalltypesoffset(fp_file,ncuroffset,kdb_block_len[ntagid]-(ncuroffset - kdb_block_off[ntagid]));
			MMIKDB_getallclassesoffset(fp_file);
			return TRUE;
		}
	}
	return FALSE;
}
/*****************************************************************************/
// 	Description :获取某一子类的名称(如 歌手,张学友，则返回张学友,如空则返回NULL)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
LOCAL char * MMIKDB_getclasstypename(int ntypeid)
{
	char *bufptr = (char*)kdb_list_buf;
	uint32 nTmpLen = 0,nstrb = 0,nstre = 0;
	uint32 j = 0;
    char buftmp[KDB_TMP_BUFFER_LEN] = {0};

 	MMIAPIFMM_SetFilePointer(fp_kdb, kdb_types_off[ntypeid] + KDB_TAG_LINELEH_SIZE,MMIFILE_SEEK_BEGIN);

	nTmpLen = KDB_TMP_BUFFER_LEN;
	MMIAPIFMM_ReadFile(fp_kdb, buftmp,nTmpLen,&nTmpLen, PNULL);

	for(j = 0; j < KDB_TMP_BUFFER_LEN; j++)
	{
		if(buftmp[j] == ',')
        {        
			nstrb = j + 1;
        }
		if(buftmp[j] == ':')
		{
			nstre = j;
			break;
		}
	}
	if(nstre != nstrb)
	{
		buftmp[nstre] = 0;
		strcpy(bufptr,&buftmp[nstrb]);	
		return bufptr;
	}
	
	return PNULL;
}
/*****************************************************************************/
// 	Description :初始化kdb模块
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbinit(void)
{
    const wchar file_name[] = { 'E', ':', '\\', 'k', 'u', 'r', 'o', '\\', 'k', 'u', 'r', 'o', '.', 'k', 'd', 'b', 0 }; 
    const wchar file_name_1[] = { 'D', ':', '\\', 'k', 'u', 'r', 'o', '\\', 'k', 'u', 'r', 'o', '.', 'k', 'd', 'b', 0}; 
    //uint16  ucs2_file_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
	
	uint32 i = 0;

    //GUI_GB2UCS(ucs2_file_name, gb_file_name, (uint16)(sizeof(gb_file_name)));
	fp_kdb = MMIAPIFMM_CreateFile( file_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
	if(fp_kdb == 0)
	{
		//GUI_GB2UCS(ucs2_file_name, gb_file_name_1, (uint16)(sizeof(gb_file_name_1)));
		fp_kdb = MMIAPIFMM_CreateFile( file_name_1, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
	}
	if(fp_kdb != 0)
	{
		MMIKDB_allocmemory();
		for(i = 0; i < KDB_LANG_COUNT;i++)
        {        
			kdb_block_off[i] = 0;
        }
		kdb_file_len = MMIAPIFMM_GetFileSize(fp_kdb);
		MMIKDB_getkdbinfos(2,fp_kdb);
		
	}
	return (fp_kdb != SFS_INVALID_HANDLE);
}
/*****************************************************************************/
// 	Description :释放kdb模块所分配的资源
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbuninit(void)
{
	if(0 != fp_kdb)
    {    
		MMIAPIFMM_CloseFile(fp_kdb);
    }
	MMIKDB_freememory();
	return TRUE;
}

/*****************************************************************************/
// 	Description :获取所有曲风类型名称(歌手，本周精选...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char** MMIKDB_get_classlist(void)
{
	char **out_buf = (char **)kdb_list_buf;
	char* bufptr = (char*)&out_buf[s_kdb_classes_count + 1];

	int i = 0,j = 0;

	SCI_MEMSET(out_buf,0,s_kdb_classes_count);
	out_buf[s_kdb_classes_count] = 0;
    for(i = 0;i < s_kdb_classes_count; i++)
 	{
 		MMIAPIFMM_SetFilePointer(fp_kdb, kdb_types_off[kdb_classes_type_index[i]] + KDB_TAG_LINELEH_SIZE,MMIFILE_SEEK_BEGIN);
		
		if(bufptr - kdb_list_buf > KDB_LIST_BUFFER_SIZE - KDB_TMP_BUFFER_LEN)
        {        
			break;
        }

		s_nTmpLen = KDB_TMP_BUFFER_LEN;
		MMIAPIFMM_ReadFile(fp_kdb, bufptr,s_nTmpLen,&s_nTmpLen, PNULL);

		for(j = 0; j < KDB_TMP_BUFFER_LEN; j++)
		{
			if(bufptr[j] == ',')
			{
				out_buf[i] = bufptr;
				if(j > MMIKUR_NAME_MAX_LEN)
                {                
					j = MMIKUR_NAME_MAX_LEN;
                }
				bufptr[j] = '\0';
				bufptr = &bufptr[j+1];
				break;
			}
		}
 	}
	return out_buf;
}
/*****************************************************************************/
// 	Description :获取所有曲风类型数量(歌手，本周精选...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_classescount()
{
	return s_kdb_classes_count;
}

/*****************************************************************************/
// 	Description :获取某一曲风类型的子项列表(如所有歌手)
//		nclassid:曲风类型的编号
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_typelist(int nclassid)
{
	uint32 ncount = 0,i = 0,nIndex = 0,j = 0,nstrb = 0,nstre = 0;
	char **out_buf = (char **)kdb_list_buf;
	char *list_buf = 0;
	ncount = MMIKDB_get_class_typecount(nclassid);
	if(ncount == 0)
    {    
		return 0;
    }

	SCI_MEMSET(out_buf,0,ncount);
	list_buf = (char *)&out_buf[ncount + 1];
	out_buf[ncount] = 0;
	for(i = 0,nIndex = kdb_classes_type_index[nclassid]; i < ncount; i++,nIndex++)
	{
		MMIAPIFMM_SetFilePointer(fp_kdb,kdb_types_off[nIndex] + KDB_TAG_LINELEH_SIZE,MMIFILE_SEEK_BEGIN);

		s_nTmpLen = KDB_TMP_BUFFER_LEN;
		if(list_buf - kdb_list_buf > (int)(KDB_LIST_BUFFER_SIZE - s_nTmpLen))
        {       
			break;
        }
		MMIAPIFMM_ReadFile(fp_kdb, list_buf,s_nTmpLen,&s_nTmpLen, PNULL);
		for(j = 0; j < KDB_TMP_BUFFER_LEN; j++)
		{
			if(list_buf[j] == ',')
            {           
				nstrb = j + 1;
            }
			if(list_buf[j] == ':')
			{
				nstre = j;
				break;
			}
		}
		if(nstre - nstrb > MMIKUR_NAME_MAX_LEN)
			nstre = nstrb + MMIKUR_NAME_MAX_LEN;
		SCI_MEMCPY(list_buf,&list_buf[nstrb],nstre - nstrb);
		list_buf[nstre - nstrb] = 0;
		out_buf[i] = list_buf;
		list_buf = list_buf + nstre - nstrb + 1;
	}
	return out_buf;
}
/*****************************************************************************/
/*
	获取某一曲风类型的子项数量(如所有歌手)
	nclassid:曲风类型的编号
*/
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_class_typecount(uint32 nclassid)
{
	int nbegintype = 0,nendtype = 0,nCount = 0;
	
	if((int)nclassid >= s_kdb_classes_count)
    {    
		return 0;
    }
	nbegintype = kdb_classes_type_index[nclassid];
	if((int)nclassid == s_kdb_classes_count - 1)
    {    
		nendtype = kdb_types_count;
    }
	else
    {    
		nendtype = kdb_classes_type_index[nclassid + 1];
    }
	nCount = nendtype - nbegintype;

	if(nCount == 1)
	{
		if(MMIKDB_getclasstypename(nbegintype) == PNULL)
        {        
			return 0;
        }
	}

	if(nCount > KDB_TYPES_MAX_COUNT)
    {    
		nCount = KDB_TYPES_MAX_COUNT;
    }
	return nCount;
}

/*****************************************************************************/
/*
	获取某一曲风类型某一子项的歌曲列表(如某个歌手的所有歌曲)
	ntypeid:曲风类型子项的编号
*///	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_songslist(int nclassid,int ntypeid,int* nsongscount)
{
	uint32 i = 0,nstrb = 0,nstre = 0,j = 0;
	uint16 nlinelen = 0,nsongcount = 0;

	char **out_buf = (char **)kdb_list_buf;
	char *list_buf = kdb_list_buf;
	
	ntypeid = kdb_classes_type_index[nclassid] + ntypeid;
	if(ntypeid >= kdb_types_count)
    {   
		return 0;
    }
	
	MMIAPIFMM_SetFilePointer(fp_kdb,kdb_types_off[ntypeid],MMIFILE_SEEK_BEGIN);

	s_nTmpLen = sizeof(nlinelen);
	MMIAPIFMM_ReadFile(fp_kdb, &nlinelen,s_nTmpLen,&s_nTmpLen, PNULL);
	nlinelen = INT2_H2N(nlinelen);

	s_nTmpLen = nlinelen - KDB_TAG_LINELEH_SIZE;
	MMIAPIFMM_ReadFile(fp_kdb, list_buf,s_nTmpLen,&s_nTmpLen, PNULL);

	for(i = KDB_TAG_LINELEH_SIZE; i < nlinelen; i++)
	{
		if(list_buf[i] == ':')
        {       
			break;
        }
	}

	
	nsongcount = (nlinelen - i - 1 - KDB_TAG_LINELEH_SIZE) / 2;
	SCI_MEMCPY(&nsongcount,&list_buf[i+1],KDB_TAG_SONGCOUNT_SIZE);
	nsongcount = INT2_H2N(nsongcount);

	if(nsongcount > KDB_SONG_MAX_COUNT)
    {    
		nsongcount = KDB_SONG_MAX_COUNT;
    }
	
	*nsongscount = nsongcount;

	SCI_MEMCPY(songid,&list_buf[i+1+KDB_TAG_SONGCOUNT_SIZE],nsongcount * 2);
	
	for(i = 0; i < nsongcount; i++)
	{
		songid[i] = INT2_H2N(songid[i]);
	}
	
	list_buf = (char *)&out_buf[nsongcount + 1];
	SCI_MEMSET(out_buf,0,nsongcount);
	out_buf[nsongcount] = 0;

	for(i = 0; i < nsongcount; i++)
	{
		MMIAPIFMM_SetFilePointer(fp_kdb,kdb_songs_off[songid[i]],MMIFILE_SEEK_BEGIN);

		s_nTmpLen = sizeof(nlinelen);
		MMIAPIFMM_ReadFile(fp_kdb, &nlinelen,s_nTmpLen,&s_nTmpLen, PNULL);
		nlinelen = INT2_H2N(nlinelen);

		s_nTmpLen = nlinelen - KDB_TAG_LINELEH_SIZE;
		if(list_buf - kdb_list_buf > (int)(KDB_LIST_BUFFER_SIZE - s_nTmpLen))
        {        
			break;
        }
		MMIAPIFMM_ReadFile(fp_kdb, list_buf,s_nTmpLen,&s_nTmpLen, PNULL);
		for(j = 0; j < nlinelen- KDB_TAG_LINELEH_SIZE; j++)
		{
			if(list_buf[j] == '|')
			{
				nstrb = j + 1;
				break;
			}
		}
		nstre = nlinelen- KDB_TAG_LINELEH_SIZE;
		if(nstre - nstrb > MMIKUR_NAME_MAX_LEN)
        {        
			nstre = nstrb + MMIKUR_NAME_MAX_LEN;
        }
		SCI_MEMCPY(list_buf,&list_buf[nstrb],nstre - nstrb);
		list_buf[nstre - nstrb] = 0;
		out_buf[i] = list_buf;
		list_buf +=  nstre - nstrb + 1;
	}
	return out_buf;
}
/*****************************************************************************/
/*
	根据typeid及选中的歌曲Index 获取歌曲路径
*///  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_get_songpath(int nclassid,int ntypeid,int nindex,char *psongpath,uint32 npathlen)
{
	uint32 i = 0,j = 0;
	uint16 nlinelen = 0,nsongcount = 0;
	BOOLEAN result = FALSE;
    char list_buf[KDB_SONG_PATH_BUF_SIZE] = {0};

	ntypeid = kdb_classes_type_index[nclassid] + ntypeid;
	if(ntypeid >= kdb_types_count)
    {   
		return FALSE;
    }
	
	MMIAPIFMM_SetFilePointer(fp_kdb,kdb_types_off[ntypeid],MMIFILE_SEEK_BEGIN);

	s_nTmpLen = sizeof(nlinelen);
	MMIAPIFMM_ReadFile(fp_kdb, &nlinelen,s_nTmpLen,&s_nTmpLen, PNULL);
	nlinelen = INT2_H2N(nlinelen);

	s_nTmpLen = nlinelen - KDB_TAG_LINELEH_SIZE;
	if(s_nTmpLen > KDB_SONG_PATH_BUF_SIZE)
    {    
		s_nTmpLen = KDB_SONG_PATH_BUF_SIZE;
    }
	MMIAPIFMM_ReadFile(fp_kdb, list_buf,s_nTmpLen,&s_nTmpLen, PNULL);

	for(i = KDB_TAG_LINELEH_SIZE; i < nlinelen; i++)
	{
		if(list_buf[i] == ':')
        {        
			break;
        }
	}
	
	nsongcount = (nlinelen - i - 1 - KDB_TAG_LINELEH_SIZE) / 2;
	SCI_MEMCPY(&nsongcount,&list_buf[i+1],KDB_TAG_SONGCOUNT_SIZE);
	nsongcount = INT2_H2N(nsongcount);
	if(nsongcount > KDB_SONG_MAX_COUNT)
    {    
		nsongcount = KDB_SONG_MAX_COUNT;
    }

	SCI_MEMCPY(songid,&list_buf[i+1+KDB_TAG_SONGCOUNT_SIZE],nsongcount * 2);
	for(i = 0; i < nsongcount; i++)
	{
		songid[i] = INT2_H2N(songid[i]);
	}

//begin to read path info
	if(nindex < nsongcount)
	{
		MMIAPIFMM_SetFilePointer(fp_kdb,kdb_songs_off[songid[nindex]],MMIFILE_SEEK_BEGIN);
		s_nTmpLen = sizeof(nlinelen);
		MMIAPIFMM_ReadFile(fp_kdb, &nlinelen,s_nTmpLen,&s_nTmpLen, PNULL);
		nlinelen = INT2_H2N(nlinelen);

		s_nTmpLen = nlinelen - KDB_TAG_LINELEH_SIZE;
		MMIAPIFMM_ReadFile(fp_kdb, list_buf,s_nTmpLen,&s_nTmpLen, PNULL);

		if(s_nTmpLen > KDB_SONG_PATH_BUF_SIZE)
        {      
			s_nTmpLen = KDB_SONG_PATH_BUF_SIZE;
        }

		for(j = 0; j < s_nTmpLen; j++)
		{
			if(list_buf[j] == '|')
			{
				list_buf[j] = '\0';
				break;
			}
		}
		if(npathlen <= strlen(list_buf))
        {        
			result = FALSE;
        }
		else
		{
			result = TRUE;
			strcpy(psongpath,list_buf);
#ifndef WIN32
//由于有些机型并不是从"D:\\"开始计算盘符，这里需要调整一下
/*lint -save -e506 */
			//if(TOUPPER('d') != TOUPPER(MMIKDB_DEFAULT_DISK))
			{
				psongpath[0] = TOUPPER(psongpath[0]) + TOUPPER(MMIKDB_DEFAULT_DISK) - TOUPPER('d');
			}
/*lint -restore */
#endif
		}
	}
	return result;
}


/*****************************************************************************/
// 	Description : 从kdb文件中获取song路径
//	Global resource dependence : none
//  Author: cmzs
//	Note:
/*****************************************************************************/
PUBLIC void MMIKUR_GetKdbSongPath(uint16 *uc2s_song_path,uint16 *song_path_len,uint16 nclassindex,uint16 ntypeindex,uint16 nsongindex)
{
    char pSongPath[KDB_MAX_PATH_LEN] = {0};
	uint16 nUCLen = 0;
	if(MMIKDB_kdbinit())
	{
		if(MMIKDB_get_songpath(nclassindex,ntypeindex,nsongindex,pSongPath,KDB_MAX_PATH_LEN))
		{
			nUCLen = GUI_GBToWstr((uint16 *)uc2s_song_path, (uint8*)pSongPath, strlen(pSongPath));
			*song_path_len = nUCLen;
		}
		MMIKDB_kdbuninit();
	}
}
#endif

