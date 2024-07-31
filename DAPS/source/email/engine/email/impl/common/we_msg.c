//pclint
/*lint -e767 */
/*lint -e722 */
/*lint -e779 */

#include "we_msg.h"
#include "eme_utils_api.h"

#define WE_MSG_RESOLVED_SIZE               (40)
#define WE_MSG_TECH                        (0x68636554)/*Tech*/
#define WE_MSG_SOFT                        (0x74666f53)/*Soft*/
#define WE_MSG_HEADER_MASK                 (0x10000001)
#define WE_MSG_PART_MASK                   (0x20000002)

static WE_UINT wemsg_cal_crc(WE_UCHAR *ptr, WE_UINT len);
static WE_BOOL wemsg_check_msg_buffer_valied(WE_UCHAR *ptr, WE_UINT len, WE_UINT32 *puiCrc);

typedef void*  IWEMem;
typedef uint16 WE_AECHAR;


#define IWEMEM_Free              EME_UTILS_FREEIF
#define IWEMEM_Malloc            EME_UTILS_MALLOC
#define IWEMEM_Strdup            EME_UTILS_STRDUP
#define IWEMEM_WStrdup           EME_UTILS_WSTRDUP
#define WE_MEMSET                EME_UTILS_MEMSET
#define WE_MEMCPY                EME_UTILS_MEMCPY
#define WE_STRLEN                EME_UTILS_STRLEN
#define WE_WSTRLEN               EME_UTILS_WSTRLEN



typedef struct ls_head ls_head;
struct ls_head
{
   struct ls_head *prev;
   struct ls_head *next;
};

typedef struct ls_part ls_part;
struct ls_part
{
   ls_head head;
   IWEMsgValue *pIMsgPart;
};

typedef struct ls_opt ls_opt;
struct ls_opt
{
   ls_head  head;
   WEMsgOpt opt;
};

struct IWEMsg
{
   WE_VTBL(IWEMsg) *pvt;
   WE_VTBL(IWEMsg) svt;
   WE_UINT32       uiRef;
   WE_UINT32       uiCrc;
   IWEMem          *pIMem;

   /*header handler*/
   IWEMsgValue    *pIMsgHeader;

   /*part handler list*/
   ls_head         part_head;
   WE_UINT32       part_cnt;

   WE_UCHAR        *pucBuff;
};

struct IWEMsgValue
{
   WE_VTBL(IWEMsgValue) *pvt;
   WE_VTBL(IWEMsgValue) svt;
   WE_UINT32            uiRef;
   IWEMem               *pIMem;
   IWEMsg               *pIWEMsg;

   /*opts value list*/
   ls_head              opt_head;
   WE_UINT32            opt_cnt;
};

static WE_INT wemsg_delpartvalue(IWEMsg *p, WE_INT32 nPos);
static WE_INT wemsgvalue_removeopt(IWEMsgValue *p, WEMsgOptId iOptId, WE_INT32 nPos);
static ls_opt* ls_opt_find(ls_head *head, WEMsgOptId iOptId, WE_INT32 nPos);

static WE_VOID ls_init(ls_head *list)
{
   list->next = list;
   list->prev = list;
}

static  WE_VOID ls_add(ls_head *fresh, ls_head *prev, ls_head *next)
{
   next->prev = fresh;
   fresh->next = next;
   fresh->prev = prev;
   prev->next = fresh;
}

static WE_BOOL ls_empty(ls_head *head)
{
   return (head == head->next);
}

static ls_head* ls_find(ls_head *head, WE_INT32 *pnPos)
{
   ls_head *tmp = head->next;
   WE_INT32 iIndex = 0;

   if(ls_empty(head))
   {
      return NULL;
   }

   if(*pnPos < 0)
   {
      *pnPos = WEMSG_END_POSTION;
      return head->prev;
   }

   while(tmp != head)
   {
      if(iIndex == *pnPos)
      {
         return tmp;
      }

      tmp = tmp->next;
      iIndex++;
   }

   *pnPos = WEMSG_END_POSTION;
   return head->prev;
}

static WE_VOID ls_del(ls_head *prev, ls_head *next)
{
   next->prev = prev;
   prev->next = next;
}

static WE_UINT32 wemsg_addref(IWEMsg *p)
{
   return ++p->uiRef;
}

static WE_UINT32 wemsg_release(IWEMsg *p)
{
   if(--p->uiRef > 0)
   {
      return p->uiRef;
   }

   if(p->pIMsgHeader != NULL)
   {
      IWEMSGVALUE_Release(p->pIMsgHeader);
   }

   /*remove all part if exist*/
   while(WE_SUCCESS == wemsg_delpartvalue(p, 0));

   if(NULL != p->pucBuff)
   {
      IWEMEM_Free(pIMem, p->pucBuff);
   }

   IWEMEM_Free(pIMem, p);

   return 0;
}

static WE_INT wemsg_addhdrvalue(IWEMsg *p, IWEMsgValue *pIHeader)
{
   if(NULL == pIHeader)
   {
      return WE_BADPARM;
   }

   /*free old header handler first*/
   if(p->pIMsgHeader != NULL)
   {
      IWEMSGVALUE_Release(p->pIMsgHeader);
   }

   /*add new header handler*/
   p->pIMsgHeader = pIHeader;
   IWEMSGVALUE_AddRef(pIHeader);

   pIHeader->pIWEMsg = p;
   
   return WE_SUCCESS;
}

static WE_INT wemsg_delhdrvalue(IWEMsg *p)
{
   if(p->pIMsgHeader != NULL)
   {
      IWEMSGVALUE_Release(p->pIMsgHeader);
      p->pIMsgHeader = NULL;
   }

   return WE_SUCCESS;
}

static WE_INT wemsg_gethdrvalue(IWEMsg *p, IWEMsgValue **ppIHeader)
{
   if(NULL == ppIHeader)
   {
      return WE_BADPARM;
   }

   if(NULL == p->pIMsgHeader)
   {
      return WE_UNSUPPORT;
   }

   *ppIHeader = p->pIMsgHeader;
   IWEMSGVALUE_AddRef(p->pIMsgHeader);
   
   return WE_SUCCESS;
}

static WE_INT wemsg_addpartvalue(IWEMsg *p, IWEMsgValue *pIPart, WE_INT32 *pnPos)
{
   ls_part *pstPart = NULL;
   WE_INT  iRet = WE_SUCCESS;

   if(NULL == pIPart || NULL == pnPos)
   {
      return WE_BADPARM;
   }

   do 
   {
      ls_head *tmp = NULL;

      pstPart = IWEMEM_Malloc(p->pIMem, sizeof(ls_part));
      WE_BREAKNULL(pstPart, iRet, WE_NOMEMORY, "no memory");
      WE_MEMSET(pstPart, 0x00, sizeof(ls_part));

      if(NULL != (tmp = ls_find(&p->part_head, pnPos)))
      {
         if(*pnPos == WEMSG_END_POSTION)
         {
            ls_add(&pstPart->head, tmp, tmp->next);
         }
         else
         {
            ls_add(&pstPart->head, tmp->prev, tmp);
         }
      }
      else
      {
         ls_add(&pstPart->head, &p->part_head, p->part_head.next);
         *pnPos = 0;
      }

      pstPart->pIMsgPart = pIPart;
      IWEMSGVALUE_AddRef(pIPart);

      pIPart->pIWEMsg = p;
      
      p->part_cnt++;

   } while (0);

   return iRet;
}

static WE_INT wemsg_delpartvalue(IWEMsg *p, WE_INT32 nPos)
{
   ls_head *tmp = NULL;

   if(ls_empty(&p->part_head))
      return WE_FAILED;

   if(nPos >= (WE_INT32)p->part_cnt)
      return WE_FAILED;

   if(NULL != (tmp = ls_find(&p->part_head, &nPos)))
   {
      ls_part *part = (ls_part*)tmp;
      ls_del(tmp->prev, tmp->next);
      if(part->pIMsgPart)
      {
         IWEMSGVALUE_Release(part->pIMsgPart);
      }
      IWEMEM_Free(p->pIMem, part);
      p->part_cnt--;
      return WE_SUCCESS;
   }

   return WE_FAILED;
}

static WE_INT wemsg_getpartvalue(IWEMsg *p, IWEMsgValue **ppIPart, WE_INT32 nPos)
{
   ls_head *tmp = NULL;
   ls_part *part = NULL;

   if(ls_empty(&p->part_head))
   {
      return WE_FAILED;
   }

   if(nPos >= (WE_INT32)p->part_cnt)
   {
      return WE_FAILED;
   }

   if(NULL != (tmp = ls_find(&p->part_head, &nPos)))
   {
      part = (ls_part*)tmp;
      *ppIPart = part->pIMsgPart;
      IWEMSGVALUE_AddRef(part->pIMsgPart);
      return WE_SUCCESS;
   }

   return WE_FAILED;
}

static WE_INT wemsg_getpartcount(IWEMsg *p, WE_UINT32 *pcount)
{
   if(NULL == pcount)
      return WE_BADPARM;

   *pcount = p->part_cnt;
   return WE_SUCCESS;
}

static WE_UINT32 wemsg_encode_value(IWEMsgValue *p, WE_UCHAR *pucBuff)
{
   ls_opt    *lsopt = NULL;
   WE_UINT32 uiIndex = 0;
   WE_UINT32 uiSize = 0;
   WE_UINT32 uiOptSize = 0;
   WE_UINT32 uiOptNum = 0;
   WE_UCHAR  *pucTmp = pucBuff;

   if(NULL == p)
   {
      return 0;
   }

   uiSize += sizeof(WE_UINT32); /*opt size*/
   uiSize += sizeof(WE_UINT32); /*opt numb*/

   if(NULL != pucTmp)
   {
      /*skip opt size*/
      pucTmp += sizeof(WE_UINT32);

      /*skip opt num*/
      pucTmp += sizeof(WE_UINT32);
   }

   while(uiIndex < p->opt_cnt)
   {
      lsopt = ls_opt_find(&p->opt_head, WEMSG_OPT_ANY, (WE_INT32)uiIndex);

      if(lsopt == NULL)
      {
         uiIndex++;
         continue;
      }

      uiOptSize += (sizeof(WE_UINT32) + sizeof(WE_UINT32) + sizeof(WE_UINT32));
      uiOptSize += lsopt->opt.uiSize;

      if(NULL != pucTmp)
      {
         WE_MEMCPY(pucTmp, &lsopt->opt.iId, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);

         WE_MEMCPY(pucTmp, &lsopt->opt.uiType, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);

         WE_MEMCPY(pucTmp, &lsopt->opt.uiSize, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);

         if(lsopt->opt.iId >= WEMSG_OPT_32BIT && lsopt->opt.iId <= WEMSG_OPT_32BIT_LAST)
         {
            WE_UINT32 uiVal = (WE_UINT32)lsopt->opt.pVal;
            WE_MEMCPY(pucTmp, &uiVal, lsopt->opt.uiSize);
         }
         else
         {
            WE_MEMCPY(pucTmp, lsopt->opt.pVal, lsopt->opt.uiSize);
         }
         pucTmp += lsopt->opt.uiSize;
      }

      uiOptNum++;
      uiIndex++;
   }

   uiSize += uiOptSize;

   if(NULL != pucBuff)
   { 
      pucTmp = pucBuff;
      WE_MEMCPY(pucTmp, &uiSize, sizeof(WE_UINT32));
      pucTmp += sizeof(WE_UINT32);
      WE_MEMCPY(pucTmp, &uiOptNum, sizeof(WE_UINT32));
   }

   return uiSize;
}

static WE_UINT32 wemsg_decode_value(IWEMsgValue *p, WE_UCHAR *pucBuff)
{
   WE_INT32  iRet = WE_SUCCESS;
   WE_INT32  uiSize = 0;
   WE_UINT32 uiRetrun = 0;
   WE_UINT32 uiCnt = 0;
   WE_UINT32 uiIndex = 0;
   WE_UCHAR  *pucTmp = pucBuff;
   WEMsgOpt  stOpt[2] = {0};
   WE_CHAR   *pucCopyBuff = NULL;

   if(NULL == pucBuff || NULL == p)
   {
      return 0;
   }

   /*opt size*/
   WE_MEMCPY(&uiSize, pucTmp, sizeof(WE_UINT32));
   pucTmp += sizeof(WE_UINT32);
   uiRetrun = (WE_UINT32)uiSize;

   /*opt num*/
   WE_MEMCPY(&uiCnt, pucTmp, sizeof(WE_UINT32));
   pucTmp += sizeof(WE_UINT32);

   while(uiIndex < uiCnt && uiSize > 0)
   {
      WE_MEMSET(stOpt, 0x00, sizeof(WEMsgOpt) * 2);

      /*id*/
      WE_MEMCPY(&stOpt[0].iId, pucTmp, sizeof(WE_UINT32));
      pucTmp += sizeof(WE_UINT32);
      uiSize -= sizeof(WE_UINT32);

      /*type*/
      WE_MEMCPY(&stOpt[0].uiType, pucTmp, sizeof(WE_UINT32));
      pucTmp += sizeof(WE_UINT32);
      uiSize -= sizeof(WE_UINT32);

      /*size*/
      WE_MEMCPY(&stOpt[0].uiSize, pucTmp, sizeof(WE_UINT32));
      pucTmp += sizeof(WE_UINT32);
      uiSize -= sizeof(WE_UINT32);

      /*value*/

      if(stOpt[0].iId >= WEMSG_OPT_32BIT && stOpt[0].iId <= WEMSG_OPT_32BIT_LAST)
      {
         WE_UINT32 uiVal = 0;
         WE_MEMCPY(&uiVal, pucTmp, sizeof(WE_UINT32));
         stOpt[0].pVal = (WE_VOID*)uiVal;
      }
      else
      {
         pucCopyBuff = IWEMEM_Malloc(p->pIMem, stOpt[0].uiSize);
         WE_BREAKNULL(pucCopyBuff, iRet, WE_NOMEMORY, "no memory!!");
         WE_MEMSET(pucCopyBuff, 0x00, stOpt[0].uiSize);
         WE_MEMCPY(pucCopyBuff, pucTmp, stOpt[0].uiSize);
         stOpt[0].pVal = pucCopyBuff;
      }
      pucTmp += stOpt[0].uiSize;
      uiSize -= stOpt[0].uiSize;

      IWEMSGVALUE_AddOpt(p, stOpt);

      IWEMEM_Free(p->pIMem, pucCopyBuff);
      uiIndex++;
   }

   if(WE_SUCCESS != iRet)
   {
      return 0;
   }

   return uiRetrun;
}


static WE_INT wemsg_encodemsg(IWEMsg *p, WE_UCHAR **ppBuf, WE_UINT32 *puiBufSize)
{
   WE_INT    iRet = WE_SUCCESS;
   WE_UINT32 uiSize = 0;
   WE_UINT32 uiCount = 0;
   WE_UCHAR  *pucTmp = NULL;

   WE_UINT32 uiMask = 0;

   WE_UINT32 uiCRC = 0;

   ls_head   *head_tmp = NULL;
   ls_part   *part_tmp = NULL;

   if(NULL == ppBuf || NULL == puiBufSize)
   {
      return WE_BADPARM;
   }

   do 
   {
      uiSize += WE_MSG_RESOLVED_SIZE;

      /*msg header size*/
      if(NULL != p->pIMsgHeader)
      {
         uiSize += sizeof(WE_UINT32); /*msg header mask*/
         uiSize += wemsg_encode_value(p->pIMsgHeader, NULL); /*msg header value*/
      }

      /*msg part size*/
      while(uiCount < p->part_cnt)
      {
         head_tmp = ls_find(&p->part_head, (WE_INT32*)&uiCount);
         if(NULL == head_tmp)
         {
            uiCount++;
            continue;
         }
         
         part_tmp = (ls_part*)head_tmp;

         uiSize += sizeof(WE_UINT32); /*msg part mask*/
         uiSize += wemsg_encode_value(part_tmp->pIMsgPart, NULL);

         uiCount++;
      }

      /*free old buffer*/
      IWEMEM_Free(p->pIMem, p->pucBuff);
      p->pucBuff = NULL;

      /*malloc new buffer*/
      p->pucBuff = IWEMEM_Malloc(p->pIMem, uiSize);
      WE_BREAKNULL(p->pucBuff, iRet, WE_NOMEMORY, "no memory");
      WE_MEMSET(p->pucBuff, 0x00, uiSize);
      
      pucTmp = p->pucBuff;

      uiMask = WE_MSG_TECH;
      WE_MEMCPY(pucTmp, &uiMask, sizeof(WE_UINT32));
      uiMask = WE_MSG_SOFT;
      WE_MEMCPY(pucTmp+sizeof(WE_UINT32), &uiMask, sizeof(WE_UINT32));
      pucTmp += WE_MSG_RESOLVED_SIZE;

      /*msg header*/
      if(NULL != p->pIMsgHeader)
      {
         uiMask = WE_MSG_HEADER_MASK;
         WE_MEMCPY(pucTmp, &uiMask, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);
         pucTmp += wemsg_encode_value(p->pIMsgHeader, pucTmp);
      }

      /*msg part*/
      uiCount = 0;
      while(uiCount < p->part_cnt)
      {
         uiMask = WE_MSG_PART_MASK;

         head_tmp = ls_find(&p->part_head, (WE_INT32*)&uiCount);
         if(NULL == head_tmp)
         {
            uiCount++;
            continue;
         }

         part_tmp = (ls_part*)head_tmp;

         WE_MEMCPY(pucTmp, &uiMask, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);
         pucTmp += wemsg_encode_value(part_tmp->pIMsgPart, pucTmp);

         uiCount++;
      }

      uiCRC = wemsg_cal_crc((p->pucBuff + (sizeof(WE_UINT32) * 2)), (uiSize - (sizeof(WE_UINT32) * 2)));

      WE_MEMCPY(p->pucBuff + sizeof(WE_UINT32) * 2, &uiCRC, sizeof(WE_UINT32));
      *ppBuf      = p->pucBuff;
      *puiBufSize = uiSize;

      p->uiCrc = uiCRC;

   } while (0);

   return iRet;
}

static WE_INT wemsg_decodemsg(IWEMsg *p, WE_UCHAR *pBuf, WE_UINT32 uiBufSize)
{
   WE_INT iRet = WE_SUCCESS;

   WE_UINT32 uiDecodeSize = 0;
   WE_UINT32 uiMask = 0;
   WE_UINT32 uiSize = 0;
   WE_UCHAR  *pucTmp = pBuf;
   IWEMsgValue *pIMsgValue = NULL;

   if(NULL == pBuf || 0 == uiBufSize || uiBufSize < WE_MSG_RESOLVED_SIZE)
   {
      return WE_BADPARM;
   }

   /*check buffer valued*/
   if(!wemsg_check_msg_buffer_valied(pBuf, uiBufSize, &p->uiCrc))
   {
      return WE_BADFILE;
   }

   /*clean old value*/
   if(p->pIMsgHeader != NULL)
   {
      IWEMSGVALUE_Release(p->pIMsgHeader);
      p->pIMsgHeader = NULL;
   }
   while(WE_SUCCESS == wemsg_delpartvalue(p, 0));

   /*no msg header and part*/
   if(uiBufSize == WE_MSG_RESOLVED_SIZE)
   {
      return WE_SUCCESS;
   }

   do 
   {
      /*skip resolved data*/
      pucTmp += WE_MSG_RESOLVED_SIZE;
      uiSize += WE_MSG_RESOLVED_SIZE;

      WE_MEMCPY(&uiMask, pucTmp, sizeof(WE_UINT32));
      pucTmp += sizeof(WE_UINT32); 
      uiSize += sizeof(WE_UINT32);

      while((WE_MSG_HEADER_MASK == uiMask || WE_MSG_PART_MASK == uiMask) && uiSize < uiBufSize)
      {
         iRet = wemsgvalue_new(&pIMsgValue);
         WE_BREAKNULL(pIMsgValue, iRet, iRet, "create msg value failed");

         uiDecodeSize = wemsg_decode_value(pIMsgValue, pucTmp);
         if(0 == uiDecodeSize)
         {
            iRet = WE_NOMEMORY;
            WE_BREAKIF(iRet, "no memory!!!");
         }
         else
         {
            uiSize += uiDecodeSize;
         }
         pucTmp = pBuf + uiSize;

         if(uiMask == WE_MSG_HEADER_MASK)
         {
            IWEMSG_AddHdrValue(p, pIMsgValue);
         }
         else
         {
            WE_INT32 nPos = WEMSG_END_POSTION;
            IWEMSG_AddPartValue(p,pIMsgValue,&nPos);
         }

         IWEMSGVALUE_Release(pIMsgValue);

         WE_MEMCPY(&uiMask, pucTmp, sizeof(WE_UINT32));
         pucTmp += sizeof(WE_UINT32);
         uiSize += sizeof(WE_UINT32);
      }

   } while (0);

   if(WE_SUCCESS != iRet)
   {
      wemsg_delhdrvalue(p);
      while(WE_SUCCESS == wemsg_delpartvalue(p, 0));
   }

   return iRet;
}

WE_UINT32 wemsg_getcrc(IWEMsg *p)
{
   return p->uiCrc;
}

WE_INT wemsg_new(IWEMsg **ppObj)
{
   IWEMsg *pstMsg = NULL;
   IWEMem *pIMem = NULL;
   WE_INT iRet = WE_SUCCESS;

   if(NULL == ppObj)
   {
      return WE_BADCLASS;
   }

   do 
   {
      pstMsg = IWEMEM_Malloc(pIMem, sizeof(IWEMsg));
      WE_BREAKNULL(pstMsg, iRet, WE_NOMEMORY, "no memory");
      WE_MEMSET(pstMsg, 0x00, sizeof(IWEMsg));

      pstMsg->uiRef = 1;
      pstMsg->part_cnt = 0;
      ls_init(&pstMsg->part_head);

      pstMsg->pvt = &pstMsg->svt;
      /*wemsgvalue_new(&pstMsg->pIMsgHeader);*/
      pstMsg->pIMsgHeader = NULL;
      pstMsg->pIMem = pIMem;

      pstMsg->svt.AddRef            = wemsg_addref;
      pstMsg->svt.Release           = wemsg_release;
      pstMsg->svt.AddHdrValue       = wemsg_addhdrvalue;
      pstMsg->svt.DelHdrValue       = wemsg_delhdrvalue;
      pstMsg->svt.GetHdrValue       = wemsg_gethdrvalue;
      pstMsg->svt.AddPartValue      = wemsg_addpartvalue;
      pstMsg->svt.DelPartValue      = wemsg_delpartvalue;
      pstMsg->svt.GetPartValue      = wemsg_getpartvalue;
      pstMsg->svt.GetPartCount      = wemsg_getpartcount;
      pstMsg->svt.EncodeMsg         = wemsg_encodemsg;
      pstMsg->svt.DecodeMsg         = wemsg_decodemsg;
      pstMsg->svt.GetCrc            = wemsg_getcrc;

      *ppObj = pstMsg;

   } while (0);

   return iRet;
}

static WE_VOID ls_opt_del(IWEMem *pIMem, ls_opt *lsopt)
{
   if(lsopt->opt.iId >= WEMSG_OPT_SZ && lsopt->opt.iId <= WEMSG_OPT_BINARY_LAST)
   {
      IWEMEM_Free(pIMem, lsopt->opt.pVal);
   }

   IWEMEM_Free(pIMem, lsopt);
}

static WE_INT ls_opt_setvalue(IWEMem *pIMem, ls_opt *newOpt, WEMsgOpt *pOneOpt)
{
   WE_INT iRet = WE_SUCCESS;

   do 
   {
      newOpt->opt.iId      = pOneOpt->iId;
      newOpt->opt.uiType   = pOneOpt->uiType;

      /*string*/
      if(newOpt->opt.iId >= WEMSG_OPT_SZ && newOpt->opt.iId <= WEMSG_OPT_SZ_LAST)
      {
         IWEMEM_Free(pIMem, newOpt->opt.pVal);
         newOpt->opt.pVal   = IWEMEM_Strdup(pIMem, pOneOpt->pVal);
         WE_BREAKNULL(newOpt->opt.pVal, iRet, WE_NOMEMORY, "no memory");
         newOpt->opt.uiSize = WE_STRLEN(newOpt->opt.pVal) + 1;
         return WE_SUCCESS;
      }

      /*wide string*/
      if(newOpt->opt.iId >= WEMSG_OPT_WSZ && newOpt->opt.iId <= WEMSG_OPT_WSZ_LAST)
      {
         IWEMEM_Free(pIMem, newOpt->opt.pVal);
         newOpt->opt.pVal   = IWEMEM_WStrdup(pIMem, pOneOpt->pVal);
         WE_BREAKNULL(newOpt->opt.pVal, iRet, WE_NOMEMORY, "no memory");
         newOpt->opt.uiSize = (WE_WSTRLEN(newOpt->opt.pVal) + 1) * sizeof(WE_AECHAR);
         return WE_SUCCESS;
      }

      /*value string*/
      if(newOpt->opt.iId >= WEMSG_OPT_VARBUFFER && newOpt->opt.iId <= WEMSG_OPT_VARBUFFER_LAST)
      {
         WE_MEMCPY(&newOpt->opt.uiSize, pOneOpt->pVal, sizeof(WE_UINT16));
         newOpt->opt.uiSize += sizeof(WE_UINT16);
         IWEMEM_Free(pIMem, newOpt->opt.pVal);
         newOpt->opt.pVal   = IWEMEM_Malloc(pIMem, newOpt->opt.uiSize);
         WE_BREAKNULL(newOpt->opt.pVal, iRet, WE_NOMEMORY, "no memory");
         WE_MEMSET(newOpt->opt.pVal, 0x00, newOpt->opt.uiSize);
         WE_MEMCPY(newOpt->opt.pVal, pOneOpt->pVal, newOpt->opt.uiSize);
         return WE_SUCCESS;
      }

      /*binary */
      if(newOpt->opt.iId >= WEMSG_OPT_BINARY && newOpt->opt.iId <= WEMSG_OPT_BINARY_LAST)
      {
         IWEMEM_Free(pIMem, newOpt->opt.pVal);
         newOpt->opt.pVal   = IWEMEM_Malloc(pIMem, pOneOpt->uiSize);
         WE_BREAKNULL(newOpt->opt.pVal, iRet, WE_NOMEMORY, "no memory");
         WE_MEMSET(newOpt->opt.pVal, 0x00, pOneOpt->uiSize);
         WE_MEMCPY(newOpt->opt.pVal, pOneOpt->pVal, pOneOpt->uiSize);
         newOpt->opt.uiSize = pOneOpt->uiSize;
         return WE_SUCCESS;
      }

      /*int32*/
      if(newOpt->opt.iId >= WEMSG_OPT_32BIT && newOpt->opt.iId <= WEMSG_OPT_32BIT_LAST)
      {
         newOpt->opt.pVal   = pOneOpt->pVal;
         newOpt->opt.uiSize = sizeof(WEMsgOptSize);
         return WE_SUCCESS;
      }

   } while (0);

   return WE_FAILED;
}

static ls_opt* ls_opt_new(IWEMem *pIMem, WEMsgOpt *pOneOpt)
{
   ls_opt *newOpt = NULL;

   if(NULL == pOneOpt || pOneOpt->iId == WEMSG_OPT_END)
   {
      return NULL;
   }

   newOpt = IWEMEM_Malloc(pIMem, sizeof(ls_opt));
   if(NULL == newOpt)
   {
      return NULL;
   }
   WE_MEMSET(newOpt, 0x00, sizeof(ls_opt));

   if(ls_opt_setvalue(pIMem, newOpt, pOneOpt) == WE_SUCCESS)
   {
      return newOpt;
   }

   IWEMEM_Free(pIMem, newOpt);
   return NULL;
}

static ls_opt* ls_opt_find(ls_head *head, WEMsgOptId iOptId, WE_INT32 nPos)
{
   ls_head *head_tmp = head->next;
   ls_opt  *opt_find = NULL;
   WE_INT32 index = 0;

   if(ls_empty(head) || nPos < 0)
   {
      return NULL;
   }

   while(head_tmp != head)
   {
      opt_find = (ls_opt*)head_tmp;
      if(opt_find->opt.iId == iOptId || iOptId == WEMSG_OPT_ANY)
      {
         if(index == nPos)
         {
            return opt_find;
         }
        index++;
      }
      head_tmp = head_tmp->next;
   }

   return NULL;
}

static WE_UINT32 wemsgvalue_addref(IWEMsgValue *p)
{
   return ++p->uiRef;
}

static WE_UINT32 wemsgvalue_release(IWEMsgValue *p)
{
   if(--p->uiRef > 0)
      return p->uiRef;

   while(wemsgvalue_removeopt(p, WEMSG_OPT_ANY, 0) == WE_SUCCESS);

   IWEMEM_Free(pIMem, p);

   return 0;
}

static WE_INT wemsgvalue_addopt(IWEMsgValue *p, WEMsgOpt *pstOpts)
{
   ls_opt *opt_new = NULL;
   WE_INT iRet = WE_SUCCESS;

   if(WEMSG_OPT_END == pstOpts->iId)
      return WE_BADPARM;

   while(pstOpts->iId != WEMSG_OPT_END)
   {
      opt_new = ls_opt_new(p->pIMem, pstOpts);
      WE_BREAKNULL(opt_new, iRet, WE_NOMEMORY, "no memory");
      ls_add(&opt_new->head, p->opt_head.prev, &p->opt_head);
      p->opt_cnt++;
      pstOpts++;
   }

   return iRet;
}

static WE_INT wemsgvalue_getopt(IWEMsgValue *p, WEMsgOptId iOptId, WE_INT32 nPos, WEMsgOpt *pstOpt)
{
   ls_opt   *opt_find = NULL;

   if(NULL == pstOpt)
   {
      return WE_BADPARM;
   }

   opt_find = ls_opt_find(&p->opt_head, iOptId, nPos);
   if(NULL != opt_find)
   {
      WE_MEMCPY(pstOpt, &opt_find->opt, sizeof(WEMsgOpt));
      return WE_SUCCESS;
   }

   return WE_NOTFOUND;
}

static WE_INT wemsgvalue_removeopt(IWEMsgValue *p, WEMsgOptId iOptId, WE_INT32 nPos)
{
   ls_opt  *opt_find = NULL;

   opt_find = ls_opt_find(&p->opt_head, iOptId, nPos);
   if(opt_find != NULL)
   {
      ls_del(opt_find->head.prev, opt_find->head.next);
      ls_opt_del(p->pIMem, opt_find);
      p->opt_cnt--;
      return WE_SUCCESS;
   }

   return WE_NOTFOUND;
}

static WE_INT wemsgvalue_updateopt(IWEMsgValue *p, WE_INT32 nPos, WEMsgOpt *pstOpt)
{
   ls_opt  *opt_find = NULL;
   WE_INT  iRet = WE_SUCCESS;

   if(NULL == pstOpt || pstOpt->iId == WEMSG_OPT_END)
   {
      return WE_BADPARM;
   }

   while(pstOpt->iId != WEMSG_OPT_END)
   {
      opt_find = ls_opt_find(&p->opt_head, pstOpt->iId, nPos);
      if(NULL != opt_find)
      {
         iRet = ls_opt_setvalue(p->pIMem, opt_find, pstOpt);
         WE_BREAKIF(iRet, "update opt failed");
      }
      else
      {
         WEMsgOpt stOpt[2] = {0};

         stOpt[0].iId = pstOpt->iId;
         stOpt[0].uiType = pstOpt->uiType;
         stOpt[0].uiSize = pstOpt->uiSize;
         stOpt[0].pVal = pstOpt->pVal;
         stOpt[1].iId = WEMSG_OPT_END;

         iRet = wemsgvalue_addopt(p, stOpt);
         WE_BREAKIF(iRet, "add opt failed!!!");
      }
      
      pstOpt++;
   }

   return iRet;
}

static WE_INT wemsgvalue_getoptnum(IWEMsgValue *p, WE_UINT32 *puiNum)
{
   if(NULL == puiNum)
      return WE_BADPARM;

   *puiNum = p->opt_cnt;
   return WE_SUCCESS;
}

static WE_INT wemsgvalue_getiwemsg(IWEMsgValue *p, IWEMsg ** ppIMsg)
{
   if(NULL == ppIMsg)
   {
      return WE_BADPARM;
   }

   if(p->pIWEMsg != NULL)
   {
      *ppIMsg = p->pIWEMsg;
      IWEMSG_AddRef(p->pIWEMsg);
      return WE_SUCCESS;
   }

   return WE_FAILED;
}

WE_INT wemsgvalue_new(IWEMsgValue **ppObj)
{
   IWEMsgValue *pstMsgValue = NULL;
   IWEMem *pIMem = NULL;
   WE_INT iRet = WE_SUCCESS;

   if(NULL == ppObj)
   {
      return WE_BADCLASS;
   }

   do 
   {
      pstMsgValue = IWEMEM_Malloc(pIMem, sizeof(IWEMsgValue));
      WE_BREAKNULL(pstMsgValue, iRet, WE_NOMEMORY, "no memory");
      WE_MEMSET(pstMsgValue, 0x00, sizeof(IWEMsgValue));

      pstMsgValue->uiRef = 1;
      ls_init(&pstMsgValue->opt_head);
      pstMsgValue->opt_cnt = 0;

      pstMsgValue->pvt = &pstMsgValue->svt;
      pstMsgValue->pIMem = pIMem;

      pstMsgValue->svt.AddRef      = wemsgvalue_addref;
      pstMsgValue->svt.Release     = wemsgvalue_release;
      pstMsgValue->svt.AddOpt      = wemsgvalue_addopt;
      pstMsgValue->svt.GetOpt      = wemsgvalue_getopt;
      pstMsgValue->svt.RemoveOpt   = wemsgvalue_removeopt;
      pstMsgValue->svt.UpdateOpt   = wemsgvalue_updateopt;
      pstMsgValue->svt.GetOptNum   = wemsgvalue_getoptnum;
      pstMsgValue->svt.GetIWEMsg   = wemsgvalue_getiwemsg;

      *ppObj = pstMsgValue;

   } while (0);

   return iRet;
}

static WE_UINT wemsg_cal_crc(WE_UCHAR *ptr, WE_UINT len)
{ 
   WE_UINT   crc = 0; 
   WE_UCHAR  da = 0; 
   WE_UINT   crc_ta[16]=
   {/* CRC 余式表 */ 
      0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7, 
      0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef, 
   };

   while(len-- != 0) 
   { 
      da = ((WE_UCHAR)(crc / 256)) / 16;   /* 暂存 CRC 的高四位 */ 

      crc <<= 4;                           /* CRC 右移 4 位，相当于取CRC 的低 12 位）*/ 

      crc ^= crc_ta[da^(*ptr / 16)];       /* CRC的高4位和本字节的前半字节相加后查表计算CRC，然后加上上一次CRC 的余数 */ 

      da = ((WE_UCHAR)(crc / 256)) / 16;   /* 暂存 CRC 的高 4 位 */ 

      crc <<= 4;                           /* CRC 右移 4 位， 相当于 CRC 的低 12 位） */ 

      crc ^= crc_ta[da^(*ptr & 0x0f)];     /* CRC 的高 4 位和本字节的后半字节相加后查表计算CRC， 然后再加上上一次CRC 的余数 */ 

      ptr++; 
   } 

   return(crc); 
} 

static WE_BOOL wemsg_check_msg_buffer_valied(WE_UCHAR *ptr, WE_UINT len, WE_UINT32 *puiCrc)
{
   WE_UINT32 uiTech = 0;
   WE_UINT32 uiSoft = 0;
   WE_UINT32 uiCRC = 0;
   WE_UINT32 uiCRCNew = 0;
   WE_UCHAR  *pcTmp = ptr;

   if(NULL == ptr || 0 == len)
      return FALSE;

   if(len < WE_MSG_RESOLVED_SIZE)
      return FALSE;

   WE_MEMCPY(&uiTech, pcTmp, sizeof(WE_UINT32));
   pcTmp += sizeof(WE_UINT32);
   len   -= sizeof(WE_UINT32);

   if(WE_MSG_TECH != uiTech)
      return FALSE;

   WE_MEMCPY(&uiSoft, pcTmp, sizeof(WE_UINT32));
   pcTmp += sizeof(WE_UINT32);
   len   -= sizeof(WE_UINT32);

   if(WE_MSG_SOFT != uiSoft)
      return FALSE;

   WE_MEMCPY(&uiCRC, pcTmp, sizeof(WE_UINT32));
   pcTmp += sizeof(WE_UINT32);
   len   -= sizeof(WE_UINT32);

   uiCRCNew = wemsg_cal_crc(pcTmp, len);

   if(uiCRCNew != uiCRC)
      return FALSE;

   *puiCrc = uiCRC;

   return TRUE;
}