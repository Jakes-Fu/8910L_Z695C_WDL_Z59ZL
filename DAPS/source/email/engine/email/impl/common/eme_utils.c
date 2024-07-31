/*=====================================================================================
FILE NAME   : eme_utils.c
MODULE NAME : Email engine

    GENERAL DESCRIPTION
      Dispatch signals to proper FSM according to signal id.
      
    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
    =======================================================================================
    Revision History
    
    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                  create eme_utils.c
          
=====================================================================================*/

/*===================================================================
* 
*                     Include Files
* 
====================================================================*/
/*Include System head file*/
#include "eme_utils_api.h"
#include "we_def.h"
/*Include Program Global head file*/

/*Include Module head file*/

/*Include Private head file*/


/*=============================================================================
* 
*     DEFINITIONS AND CONSTANTS(It is not recommended)
* It is recommended that this section is defined to private head file.
* 
=============================================================================*/

//-------------------------------memory related--------------------///

#define DW_CC_PARAM_INT        0xcdcdcdcd
#define DW_CC_PARAM_BYTE       0xcd

#define BYTE_AFTER_FREE        0xEE
/*=============================================================================
*
*   Static Function Prototype Declare Section
*
=============================================================================*/



static const WE_UCHAR   aucUtf8BytePerChar[] = 
{
   1, 1, 1, 1, 1, 1, 1, 1,
   0, 0, 0, 0, 2, 2, 3, 4
}; // aucUtf8BytePerChar

static const WE_UINT32  aunUtf8Offset[] =
{
   0, 0, 0x00003080, 0x000e2080UL, 0x03c82080UL
}; // aunUtf8Offset


static WE_BOOL WE_CSC_Ucs2ToUtf8(WE_CHAR *pcSrc, 
                                 WE_LONG *plSrcLen, 
                                 WE_CHAR *pcDst, 
                                 WE_LONG *plDstLen, 
                                 WE_BOOL bBigEndian);
static WE_BOOL WE_CSC_Utf8ToUcs2(WE_CHAR *pcSrc, 
                                 WE_LONG *plSrcLen, 
                                 WE_CHAR *pcDst, 
                                 WE_LONG *plDstLen, 
                                 WE_BOOL bBigEndian);

static WE_INT32 EME_CharsetUcs2ToUtf8Len
(
 const WE_CHAR *pcSrc, 
 WE_LONG *plSrcLen, 
 WE_LONG  *plDstLen,
 WE_INT32  iBigEndian
 );

/*=============================================================================
*
*   Global Variable Declare Section
*
=============================================================================*/

/*=============================================================================
*
*   File Static Variable Define Section
*
=============================================================================*/


 /*===================================================================
 * 
 *                    Functions
 * 
* ==================================================================*/
/* --------------------------------------------------------------------
* 
*    FUNCTION        EME_Utils_Malloc()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. Initialize memory list node , the node consists of (node + data content + extra memory)
*              3. update the current memory we used.
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
void* EME_Utils_Malloc(St_EME_MemHandle* pMemHdr, unsigned long nSize,const char * pFileName,int dwLine)
{
   struct St_EME_Mem_Link * temp = NULL;
   int iResult = EME_SUCCESS;
   unsigned char* dwp = NULL;//extra memory
   
   unsigned int iLen = 0;
   
   //check input parameters
   if(NULL == pFileName)
   {
      EME_UTILS_LOG_INFO(("OEMP_UTILS_MALLOC, invalid param."));
      return NULL;
   }
   
   do 
   {
      if(NULL == pMemHdr)
      {
         return (void*)WE_SCL_MALLOC(nSize);
      }
      
      if(NULL == pMemHdr->pTail)
      {
         pMemHdr->pTail = (struct St_EME_Mem_Link*)&pMemHdr->stHeader; 
      }
      /************************************************************************/
      /*Initialize memory list node , the node consists of (node + data content + extra memory)*/
      /************************************************************************/
      temp = (struct St_EME_Mem_Link *)WE_SCL_MALLOC(nSize + sizeof(struct St_EME_Mem_Link) + sizeof(unsigned long) * 2);
      EME_UTILS_BREAKNULL(temp, iResult,EME_FAILED, "failed to malloc");
      WE_SCL_MEMSET(temp, 0, nSize + sizeof(struct St_EME_Mem_Link) + sizeof(unsigned long) * 2);
      /************************************************************************/
      /*  initialize node                                                     */                
      /************************************************************************/
      temp->dwCC1 = temp->dwCC2 = DW_CC_PARAM_INT;
      temp->dwID = 0;
      temp->dwLength = nSize;
      temp->dwLine = dwLine;
      temp->pcFileName = WE_SCL_STRDUP(pFileName);
      
      /* for other mod free and no mem-header free */
      temp->pvMemMgr = pMemHdr;

      pMemHdr->pTail->next = temp;
      temp->next = NULL;
      temp->last = pMemHdr->pTail;
      pMemHdr->pTail = temp;
      /************************************************************************/
      /* initialize 2 extra memory block                                      */                              
      /************************************************************************/
      dwp = (unsigned char*)((unsigned long)temp + sizeof(struct St_EME_Mem_Link) + nSize);
      
      /************************************************************************/
      /*  avoid init address value using integer cos it will lead to memory alignment problem*/
      /************************************************************************/
      for(iLen = 0; iLen< sizeof(unsigned long) * 2; iLen++)
      {
         *(dwp + iLen) = 0xcd;
      }
      
      /************************************************************************/
      /* update the current memory we used                                    */                            
      /************************************************************************/
      pMemHdr->stStatus.dwLLibMemUsed += nSize;
      if(pMemHdr->stStatus.dwLLibMemMax < pMemHdr->stStatus.dwLLibMemUsed) 
      {
         pMemHdr->stStatus.dwLLibMemMax = pMemHdr->stStatus.dwLLibMemUsed;
      } 
      pMemHdr->stStatus.dwLLibMemObj ++;
      
   } while(0);
   
   //exception handling
   if(EME_SUCCESS != iResult)
   {
      WE_SCL_FREEIF(temp);
      return NULL;
   }


   EME_UTILS_LOG_INFO(("**************************************************************************************"));
   EME_UTILS_LOG_INFO(("Malloced memory, Address: %x, FILE", (unsigned long)temp + sizeof(struct St_EME_Mem_Link)));
   EME_UTILS_LOG_INFO(("Truely Address: %x", temp));
   EME_UTILS_LOG_INFO(("%s",pFileName));
   EME_UTILS_LOG_INFO(("%d",dwLine));
   EME_UTILS_LOG_INFO(("Size = %d", nSize));
   EME_UTILS_LOG_INFO(("**************************************************************************************"));
   
   return (void*)((unsigned long)temp + sizeof(struct St_EME_Mem_Link));
}


/* --------------------------------------------------------------------
* 
*    FUNCTION        EME_Utils_Realloc()
*
*    Pseudo Code:    
*              1. check input parameters
*              2. Initialize memory list node , the node consists of (node + data content + extra memory)
*              3. update the current memory we used.
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
void* EME_Utils_Realloc(St_EME_MemHandle* pMemHdr,void *pSrc, unsigned long nSize,const char * pFileName,int dwLine)
{
   struct St_EME_Mem_Link * temp = NULL;
   struct St_EME_Mem_Link * temp2 = NULL;
   unsigned long nSizeOfpSrc = 0;
   int iResult = EME_SUCCESS;
   unsigned char* dwp = NULL;//extra memory
   void *pNewTemp = NULL;
   unsigned int iLen = 0;
   
   
   //check input parameters
   if(NULL == pFileName)
   {
      EME_UTILS_LOG_INFO(("OEMP_UTILS_MALLOC, invalid param."));
      return NULL;
   }
   
   do 
   {
      if(NULL == pMemHdr)
      {
         return (void*)TF3RD_REALLOC(pSrc,nSize);
      }
      
      if (NULL == pSrc)
      {
         return (void *)EME_UTILS_MALLOC(pMemHdr,nSize);
      }

      /*compute the size of  pSrc*/
      temp2 = (struct St_EME_Mem_Link *)((unsigned long)pSrc -  sizeof(struct St_EME_Mem_Link));
      nSizeOfpSrc = temp2->dwLength;

      /*error handling : reallocate buffer smaller the the source buff  */
      if (nSize < nSizeOfpSrc)
      {
         iResult = EME_FAILED;
         break;
      }


      if(NULL == pMemHdr->pTail)
      {
         pMemHdr->pTail = (struct St_EME_Mem_Link*)&pMemHdr->stHeader; 
      }
      /************************************************************************/
      /*Initialize memory list node , the node consists of (node + data content + extra memory)*/
      /************************************************************************/
      temp = (struct St_EME_Mem_Link *)WE_SCL_MALLOC(nSize + sizeof(struct St_EME_Mem_Link) + sizeof(unsigned long) * 2);
      EME_UTILS_BREAKNULL(temp, iResult,EME_FAILED, "failed to malloc");
      EME_UTILS_MEMSET(temp, 0, nSize + sizeof(struct St_EME_Mem_Link) + sizeof(unsigned long) * 2);
      /************************************************************************/
      /*  initialize node                                                     */                
      /************************************************************************/
      temp->dwCC1 = temp->dwCC2 = DW_CC_PARAM_INT;
      temp->dwID = 0;
      temp->dwLength = nSize;
      temp->dwLine = dwLine;
      temp->pcFileName = WE_SCL_STRDUP(pFileName);

      /* for other mod free and no mem-header free */
      temp->pvMemMgr = pMemHdr;
      
      pMemHdr->pTail->next = temp;
      temp->next = NULL;
      temp->last = pMemHdr->pTail;
      pMemHdr->pTail = temp;
      /************************************************************************/
      /* initialize 2 extra memory block                                      */                              
      /************************************************************************/
      dwp = (unsigned char*)((unsigned long)temp + sizeof(struct St_EME_Mem_Link) + nSize);
      
      /************************************************************************/
      /*  avoid init address value using integer cos it will lead to memory alignment problem*/
      /************************************************************************/
      for(iLen = 0; iLen< sizeof(unsigned long) * 2; iLen++)
      {
         *(dwp + iLen) = 0xcd;
      }
      
      /************************************************************************/
      /* update the current memory we used                                    */                            
      /************************************************************************/
      pMemHdr->stStatus.dwLLibMemUsed += nSize;
      if(pMemHdr->stStatus.dwLLibMemMax < pMemHdr->stStatus.dwLLibMemUsed) 
      {
         pMemHdr->stStatus.dwLLibMemMax = pMemHdr->stStatus.dwLLibMemUsed;
      } 
      pMemHdr->stStatus.dwLLibMemObj ++;
     
      /*new allocate memory*/
      pNewTemp =(void *)((unsigned long)temp + sizeof(struct St_EME_Mem_Link));

      /*copy source data to new buff*/
      EME_UTILS_MEMCPY((void *)pNewTemp,(const void *)pSrc,nSizeOfpSrc);
      
      /*clear source data buffer*/
      EME_UTILS_FREEIF(pMemHdr,pSrc);
      temp2 = NULL;
      
   } while(0);
   

   //exception handling
   if(EME_SUCCESS != iResult)
   {
      WE_SCL_FREEIF(temp);
      return NULL;
   }
   
   EME_UTILS_LOG_INFO(("**************************************************************************************"));
   EME_UTILS_LOG_INFO(("Malloced memory, Address: %x, FILE", (unsigned long)temp + sizeof(struct St_EME_Mem_Link)));
   EME_UTILS_LOG_INFO(("Truely Address: %x", temp));
   EME_UTILS_LOG_INFO(("%s",pFileName));
   EME_UTILS_LOG_INFO(("%d",dwLine));
   EME_UTILS_LOG_INFO(("**************************************************************************************"));
   
   return (void*)pNewTemp;

}

void EME_UTILS_OutputMemUsed(St_EME_MemHandle* pMemHdr)
{
   struct St_EME_Mem_Link * p = NULL;
   
   if(pMemHdr->stStatus.dwLLibMemObj > 0)
   {
      EME_UTILS_LOG_INFO(("memory leak, there are %d blocks and %u bytes not released", 
         pMemHdr->stStatus.dwLLibMemObj, pMemHdr->stStatus.dwLLibMemUsed));
      
      /************************************************************************/
      /* check the list for the memory unreleased                             */                                        
      /************************************************************************/
      for(p = pMemHdr->stHeader.next; p != NULL; p = p->next)
      {
         if(p->pcFileName == NULL)
         {
            EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("unknown memory leak, %u bytes, the %d times allocate", p->dwLength, p->dwID));
            EME_UTILS_LOG_INFO(("Memory leak, Address: %x", (unsigned long)p + sizeof(struct St_EME_Mem_Link)));
            EME_UTILS_LOG_INFO(("Truely Address: %x", p));    
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"));
            EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
            
         }
         else
         {
            EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));            
            EME_UTILS_LOG_INFO(("memory leak, %s(%d): %u bytes, the %d times allocate", 
               p->pcFileName,
               p->dwLine,
               p->dwLength,
               p->dwID));
            EME_UTILS_LOG_INFO(("Memory leak, Address: %x", (unsigned long)p + sizeof(struct St_EME_Mem_Link)));
            EME_UTILS_LOG_INFO(("Truely Address: %x", p));               
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
            EME_UTILS_LOG_INFO(("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
            EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
         }
      }
   }
   else
   {
      EME_UTILS_LOG_INFO(("maximum %d(%d k)bytes have been used ", 
         pMemHdr->stStatus.dwLLibMemMax,
         pMemHdr->stStatus.dwLLibMemMax/1024));
   }
   
}


/*============================================================================ 
*    FUNCTION        EXAConform_InitIsComplete()
* 
*    DESCRIPTION     Initializes the EXAConform Services    
* 
*    DEPENDENCIES    Must be called prior to any other EXAConform
*                    function calls  
* 
*    PARAMETERS      None
* 
*    RETURN VALUE    boolean:  TRUE if initialization was successful
*                               FALSE if initialization failed   
*     
*    SIDE EFFECTS    None   
* 
*===========================================================================*/

int EME_Utils_Free(St_EME_MemHandle* pMemHdr, void * pMem,const char * pFileName,int dwLine)
{
   struct St_EME_Mem_Link * pTemp = NULL;
   unsigned char* dwp = NULL;
   unsigned int   i = 0;
   int iResult = EME_SUCCESS;
   
   if(NULL == pMem)
   {
      return EME_FAILED;
   }
   
   do 
   {
      if(pMemHdr == NULL)
      {
         WE_SCL_FREEIF(pMem);
         return EME_SUCCESS;
      }
      
      pTemp = (struct St_EME_Mem_Link*)((unsigned long)pMem - sizeof(struct St_EME_Mem_Link));//point to the node
      EME_UTILS_BREAKNULL(pTemp, iResult, EME_FAILED, "failed to malloc");
      //check the node
      //check the length of actual data content
      
      /* for no mem-header free */
      if ((unsigned long)pMemHdr == EME_UTILS_MEMMGR_FOR_FREE)
      {
         pMemHdr = (void*)pTemp->pvMemMgr;
      }

      if(pTemp->dwCC1 != DW_CC_PARAM_INT || pTemp->dwCC2 != DW_CC_PARAM_INT)
      {
         iResult = EME_FAILED;
         pTemp->dwCC1 = pTemp->dwCC2 = 0xcccccccc;//memory not initialized
      }
      if (iResult != EME_SUCCESS)
      {
         EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("Free Memory Error! Address: %x",pMem));
         EME_UTILS_LOG_INFO(("Truely Address: %x", pTemp));
         EME_UTILS_LOG_INFO(("%s",pFileName));
         EME_UTILS_LOG_INFO(("Line: %d",dwLine));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
         EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
         break;
         //OEMP_UTILS_BREAKIF(iResult, "Free Memory Error!Address: %x, FILE: %s, LINE: %d",pMem,pFileName,dwLine);
      }
      /************************************************************************/
      /*check the last two checksum                                           */                           
      /************************************************************************/
      dwp = (unsigned char*)((unsigned long)pMem + pTemp->dwLength);
      for (i = 0; i< sizeof(unsigned long) * 2; i++)
      {
         if (*dwp != DW_CC_PARAM_BYTE)
         {
            iResult = EME_FAILED;
            break;
         }
         
         dwp++;
      }
      
      if (iResult != EME_SUCCESS)
      {
         EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));     
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));                 
         EME_UTILS_LOG_INFO(("Free Memory Error! Address: %x",pMem));
         EME_UTILS_LOG_INFO(("Truely Address: %x", pTemp));
         EME_UTILS_LOG_INFO(("%s",pFileName));
         EME_UTILS_LOG_INFO(("Line: %d",dwLine));
         EME_UTILS_LOG_INFO(("The Malloc Info:",pMem));
         EME_UTILS_LOG_INFO(("%s",pTemp->pcFileName));
         EME_UTILS_LOG_INFO(("Line: %d",pTemp->dwLine));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));     
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
         EME_UTILS_LOG_INFO(("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO")); 
         EME_UTILS_LOG_INFO(("**********************************************************************************************************"));
         break;
      }
      /************************************************************************/
      /* in case the node to delete is the last node                          */                                           
      /************************************************************************/
      if(pTemp == pMemHdr->pTail)
      {
         pMemHdr->pTail = pTemp->last;
         pMemHdr->pTail->next = NULL;
      }
      /************************************************************************/
      /*  in case the node to delete is in the middle of reverse list         */                                                           
      /************************************************************************/
      else
      {
         pTemp->last->next = pTemp->next;
         pTemp->next->last = pTemp->last;
      }
      pMemHdr->stStatus.dwLLibMemObj--;
      pMemHdr->stStatus.dwLLibMemUsed -= pTemp->dwLength;
      WE_SCL_FREEIF(pTemp->pcFileName);
      EME_UTILS_MEMSET(pMem, BYTE_AFTER_FREE, pTemp->dwLength);

      WE_SCL_FREEIF(pTemp);   
   } while(0);
   
   if(EME_SUCCESS != iResult)
   {
   }
   return iResult;
}


/* --------------------------------------------------------------------
* 
*    FUNCTION       
*
*    Pseudo Code:    
*              1. check input parameters
*              2. sub code block function
*              3. sub code block 2 function
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
char *EME_UTILS_WstrToStr(St_EME_MemHandle* pMemHdr, AECHAR *pwcFrom, const char* pFileName, int dwLine)
{
   WE_UINT32   uiStrLen = 0;
   WE_CHAR     *pcTo = NULL;
   
   if (pwcFrom == NULL)
   {
      return NULL;
   }
   
   uiStrLen = EME_UTILS_WSTRLEN(pwcFrom);
   
   pcTo = (char*)EME_UTILS_MALLOC(pMemHdr, (uiStrLen + 1) * sizeof(char));
   if (pcTo == NULL)
   {
      return NULL;
   }
   EME_UTILS_MEMSET(pcTo, 0, (uiStrLen + 1) * sizeof(char));
   
   if (!WE_SCL_WSTRTOSTR(pwcFrom, pcTo, (uiStrLen + 1) * sizeof(char)))
   {
      EME_UTILS_FREEIF(pMemHdr, pcTo);
      return NULL;
   }
   
   return pcTo;
}
/* --------------------------------------------------------------------
* 
*    FUNCTION       
*
*    Pseudo Code:    
*              1. check input parameters
*              2. sub code block function
*              3. sub code block 2 function
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
AECHAR *EME_UTILS_StrToWstr(St_EME_MemHandle* pMemHdr, char *pcFrom, const char* pFileName, int dwLine)
{
   WE_UINT32   uiStrLen = 0;
   AECHAR     *pwcTo = NULL;
   
   if (pcFrom == NULL)
   {
      return NULL;
   }
   
   uiStrLen = EME_UTILS_STRLEN(pcFrom);
   
   pwcTo = (AECHAR*)EME_UTILS_MALLOC(pMemHdr, (uiStrLen + 1) * sizeof(AECHAR));
   if (pwcTo == NULL)
   {
      return NULL;
   }
   EME_UTILS_MEMSET(pwcTo, 0, (uiStrLen + 1) * sizeof(AECHAR));
   
   if (!TF3RD_STR2WSTR(pcFrom, pwcTo, (uiStrLen + 1) * sizeof(AECHAR)))
   {
      EME_UTILS_FREEIF(pMemHdr, pwcTo);
      return NULL;
   }
   
   return pwcTo;
}

/* --------------------------------------------------------------------
* 
*    FUNCTION       
*
*    Pseudo Code:    
*              1. check input parameters
*              2. sub code block function
*              3. sub code block 2 function
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
AECHAR *EME_UTILS_Utf8ToWstr(St_EME_MemHandle* pMemHdr, WE_CHAR *pcFrom, const char* pFileName, int dwLine)
{
   WE_UINT32      uiStrLen = 0;
   AECHAR      *pwcTo = NULL;
   St_EME_MemHandle *pstMemHandle = NULL;
   
   if (pcFrom == NULL)
   {
      return NULL;
   }
   /*Get memory handle*/
   pstMemHandle = pMemHdr;
   
   uiStrLen = EME_UTILS_STRLEN(pcFrom);
   if (uiStrLen == 0)
   {
      /*Allocate memory for the WSTR*/
      pwcTo = (AECHAR*)EME_UTILS_MALLOC(pstMemHandle, sizeof(AECHAR));
      if (pwcTo == NULL)
      {
         return NULL;
      }
      EME_UTILS_MEMSET(pwcTo, 0, sizeof(AECHAR));
      return pwcTo;
   }
   
   /*Allocate memory for the WSTR*/
   pwcTo = (AECHAR*)EME_UTILS_MALLOC(pstMemHandle, (uiStrLen + 1) * sizeof(AECHAR));
   if (pwcTo == NULL)
   {
      return NULL;
   }
   EME_UTILS_MEMSET(pwcTo, 0, (uiStrLen + 1) * sizeof(AECHAR));
   
   if (!TF3RD_UTF8TOWSTR((const unsigned char*)pcFrom, uiStrLen, pwcTo, (uiStrLen + 1) * sizeof(AECHAR)))
   {
      EME_UTILS_FREEIF(pstMemHandle, pwcTo);
      return NULL;
   }
   
   return pwcTo;

// 	WE_LONG           lSrcLen = 0;
//    WE_LONG           lDesLen = 0x7fffffff;
//    AECHAR            *pwcTo = NULL;
//    
//    if (pcFrom == NULL)
//    {
//       return NULL;
//    }
//    
//    lSrcLen = EME_UTILS_STRLEN(pcFrom);
//    /* str "" */
//    if (lSrcLen == 0)
//    {
//       /*Allocate memory for the WSTR*/
//       pwcTo = (AECHAR*)EME_UTILS_MALLOC(pMemHdr, sizeof(AECHAR));
//       if (pwcTo == NULL)
//       {
//          return NULL;
//       }
//       EME_UTILS_MEMSET(pwcTo, 0, sizeof(AECHAR));
//       return pwcTo;
//    }
//    
//    if (TRUE != WE_CSC_Utf8ToUcs2(pcFrom, &lSrcLen, NULL, &lDesLen, FALSE))
//    {
//       EME_UTILS_LOG_ERROR(("EME_UTILS_Utf8ToWstr, WE_CSC_Utf8ToUcs2any size failed"));
//       return NULL;
//    }
// 
//    /*Allocate memory for the WSTR*/
//    pwcTo = (AECHAR*)EME_UTILS_MALLOC(pMemHdr, (lDesLen + 1) * sizeof(AECHAR));
//    if (pwcTo == NULL)
//    {
//       return NULL;
//    }
//    EME_UTILS_MEMSET(pwcTo, 0, (lDesLen + 1) * sizeof(AECHAR));
//    
//    if (TRUE != WE_CSC_Utf8ToUcs2(pcFrom, &lSrcLen, (WE_CHAR*)pwcTo, &lDesLen, FALSE))
//    {
//       EME_UTILS_LOG_ERROR(("EME_UTILS_Utf8ToWstr, WE_CSC_Utf8ToUcs2any buf failed"));
//       EME_UTILS_FREEIF(pMemHdr, pwcTo);
//    }
//    
//    return pwcTo;
}
/* --------------------------------------------------------------------
* 
*    FUNCTION       
*
*    Pseudo Code:    
*              1. check input parameters
*              2. sub code block function
*              3. sub code block 2 function
*              4. exception handling
*  
*    
*        
* 
*    ================================================================*/
char *EME_UTILS_WstrToUtf8(St_EME_MemHandle* pMemHdr, AECHAR *pwcFrom, const char* pFileName, int dwLine)
{
  WE_UINT32      uiStrLen = 0;
  WE_UINT32      uiStrBytes = 0;
  char        *pcTo = 0;
  WE_UINT32      uiUtf8Len = 0x7fffffff;
  
  if (pwcFrom == NULL)
  {
     return NULL;
  }
  
  uiStrLen = EME_UTILS_WSTRLEN(pwcFrom);
  if (uiStrLen == 0)
  {
     pcTo = (char*)EME_UTILS_MALLOC(pMemHdr, sizeof(char));
     if (pcTo == NULL)
     {
        return NULL;
     }
     EME_UTILS_MEMSET(pcTo, 0, sizeof(char));
     return pcTo;
  }

  uiStrBytes = uiStrLen * sizeof(AECHAR);
  //uiUtf8Len = uiStrLen * MAX_UTF8_CHAR_LEN + 1;

#ifdef WIN32
  EME_CharsetUcs2ToUtf8Len((char*)pwcFrom, (long*)&uiStrBytes, (long*)&uiUtf8Len, FALSE);
#else
  EME_CharsetUcs2ToUtf8Len((char*)pwcFrom, (long*)&uiStrBytes, (long*)&uiUtf8Len, TRUE);
#endif

  if (uiUtf8Len == 0x7fffffff)
  {
     return NULL;
  }
  uiUtf8Len = uiUtf8Len + 1;  /*Null-terminate*/
  
  pcTo = (char*)EME_UTILS_MALLOC(pMemHdr, uiUtf8Len * sizeof(char));
  if (pcTo == NULL)
  {
     return NULL;
  }
  EME_UTILS_MEMSET(pcTo, 0, uiUtf8Len * sizeof(char));
  
  if (!TF3RD_WSTR2UTF8((const unsigned short*)pwcFrom, uiStrLen, (unsigned char*)pcTo, uiUtf8Len * sizeof(char)))
  {
     EME_UTILS_FREEIF(pMemHdr, pcTo);
     return NULL;
  }
  
  return pcTo;

//    WE_LONG     lSrcLen = 0;
//    WE_LONG     lDesLen = 0x7fffffff;
//    char        *pcTo = 0;
//    
//    if (pwcFrom == NULL)
//    {
//       return NULL;
//    }
//    
//    lSrcLen = EME_UTILS_WSTRLEN(pwcFrom) * sizeof(AECHAR);
//    /* str "" */
//    if (lSrcLen == 0)
//    {
//       pcTo = (char*)EME_UTILS_MALLOC(pMemHdr, sizeof(WE_CHAR));
//       if (pcTo == NULL)
//       {
//          return NULL;
//       }
//       EME_UTILS_MEMSET(pcTo, 0, sizeof(WE_CHAR));
//       return pcTo;
//    }
//    
//    if (TRUE != WE_CSC_Ucs2ToUtf8((WE_CHAR*)pwcFrom, &lSrcLen, NULL, &lDesLen, FALSE))
//    {
//       EME_UTILS_LOG_ERROR(("EME_UTILS_WstrToUtf8, WE_CSC_Ucs2AnyToUtf8 size failed"));
//       return NULL;
//    }
// 
//    pcTo = (char*)EME_UTILS_MALLOC(pMemHdr, (lDesLen + 1) * sizeof(WE_CHAR));
//    if (pcTo == NULL)
//    {
//       return NULL;
//    }
//    EME_UTILS_MEMSET(pcTo, 0, (lDesLen + 1) * sizeof(WE_CHAR));
//    
//    if (TRUE != WE_CSC_Ucs2ToUtf8((WE_CHAR*)pwcFrom, &lSrcLen, pcTo, &lDesLen, FALSE))
//    {
//       EME_UTILS_FREEIF(pMemHdr, pcTo);
//       EME_UTILS_LOG_ERROR(("EME_UTILS_WstrToUtf8, WE_CSC_Ucs2AnyToUtf8 buf failed"));
//    }
//    
//    return pcTo;
}


char EME_SCL_ChrLc(char cChr)
{
   return (char)(((cChr>='A')&&(cChr<='Z')) ? (cChr +('a'-'A')) : cChr);
}

WE_UINT32 EME_Utils_Atox(char *pcStr)
{
   WE_UINT32   uiNum = 0;
   char     cTemp = 0;
   
   if(pcStr == NULL || EME_UTILS_STRLEN(pcStr) > 8)
   {
      return (0);
   }
   
   while (*pcStr != '\0') 
   {
      if ( (*pcStr >= '0') && (*pcStr <= '9') 
         || (*pcStr >= 'a') && (*pcStr <= 'f') 
         || (*pcStr >= 'A' && (*pcStr <= 'F')))                     
      {
         if (*pcStr <= '9')
         {
            uiNum = (16 * uiNum) + (*pcStr - '0');
         }
         else
         {
            cTemp = WE_SCL_CHRLC(*pcStr);
            uiNum = (16 * uiNum) + (cTemp - 87);
         }            
         pcStr++;
      }
      else
      {
         return (0);
      }
   }  
   return (uiNum);
}


WE_INT32 EME_CharsetUcs2ToUtf8Len
(
 const WE_CHAR *pcSrc, 
 WE_LONG *plSrcLen, 
 WE_LONG  *plDstLen,
 WE_INT32  iBigEndian
 )
{
   WE_UINT32   uiTransToUcs = 0;
   WE_UINT32   uiSindex = 0;
   WE_UINT32   uiDindex = 0;
   WE_INT32    iSouNumOfByte = 0; 
   WE_INT32    iDesNumOfByte = 0;
   WE_UINT8    ucMaskOfUtf8 = 0;
   WE_INT32    iShiftFirst = 0;
   WE_INT32    iShiftSecond = 0;
   
   
   if(NULL == pcSrc || NULL == plSrcLen || NULL == plDstLen)
   {
      return FALSE;
   }
   if (iBigEndian)
   {
      iShiftFirst = 0; iShiftSecond = 1; 
   }
   else
   {
      iShiftFirst = 1; iShiftSecond = 0; 
   }
   
   while(1)
   {
      /* First, translate to UCS4 */
      if ((WE_LONG)(uiSindex + 2 )> *plSrcLen)
         break;
      uiTransToUcs = ((WE_UCHAR)pcSrc[uiSindex + iShiftFirst] << 8) |
         (WE_UCHAR)pcSrc[uiSindex + iShiftSecond];
      iSouNumOfByte = 2;
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiTransToUcs < 0x80)
      {
         iDesNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else if (uiTransToUcs < 0x800)
      {
         iDesNumOfByte = 2;
         ucMaskOfUtf8 = 0xc0;
      }
      else 
      {
         iDesNumOfByte = 3;
         ucMaskOfUtf8 = 0xe0;
      }
      
      /* Is there room for this in the destination vector? */
      if ((WE_LONG)(uiDindex + iDesNumOfByte) > *plDstLen)
         break;
      
      uiSindex += iSouNumOfByte;
      uiDindex += iDesNumOfByte;
   }
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;
   
   return TRUE;    
}

char* EME_UTILS_StrStrNC(char *pcToSearch, const char *pcSearchFor)
{
   WE_CHAR *pcOrig = EME_UTILS_STRDUP(NULL, pcToSearch);
   WE_CHAR *pcSearch = EME_UTILS_STRDUP(NULL, pcSearchFor);
   WE_CHAR *pcTmp = NULL;

   if(pcOrig == NULL || pcSearch == NULL)
      return NULL;

   pcOrig = TF3RD_STRLOWER(pcOrig);
   pcSearch = TF3RD_STRLOWER(pcSearch);

   pcTmp = EME_UTILS_STRSTR(pcOrig, pcSearch);
   if(pcTmp != NULL)
   {
      pcTmp = &pcToSearch[pcTmp - pcOrig];
      EME_UTILS_FREEIF(NULL, pcOrig);
      EME_UTILS_FREEIF(NULL, pcSearch);

      return pcTmp;
   }

   EME_UTILS_FREEIF(NULL, pcOrig);
   EME_UTILS_FREEIF(NULL, pcSearch);
   
   return NULL;


#if 0
   WE_UINT32 pos = 0;
   WE_UINT32 i = 0;
   WE_UINT32 j = 0;
   
   if (NULL == pcToSearch || NULL ==pcSearchFor)
   {
      return NULL;
   }
   for (pos = 0; pos < (WE_UINT32)EME_UTILS_STRLEN(pcToSearch); pos++)
   {
      /* set the begin pos for find. */
      j = pos; 

      if ((WE_UINT32)EME_UTILS_STRLEN(pcToSearch)- pos < (WE_UINT32)EME_UTILS_STRLEN(pcSearchFor))
      {
         return NULL;
      }

      for (i = 0; i < (WE_UINT32)EME_UTILS_STRLEN(pcSearchFor); i++)
      {
         if (pcToSearch[j] == pcSearchFor[i])
         {
            j++;         
         }       
         
         else
         {
            if ( pcToSearch[j] < 65 || pcToSearch[j] > 122 || (pcToSearch[j] > 90 && pcToSearch[j] < 97) || /* isn't alpha */
               (pcToSearch[j] >= 65 && pcToSearch[j] <= 90 && pcToSearch[j] + 32 != pcSearchFor[i]) || /* is upper alpha */
               (pcToSearch[j] >= 97 && pcToSearch[j] <= 122 && pcToSearch[j] != pcSearchFor[i] + 32) ) /* is lower alpha */
            {
               break;
            }

            else         
            {
               j++;
            }
         }         
      }
      
      if ( pcSearchFor[i] == '\0' )
      {
          /* here find the pos in the pcToSearch . */
           return &pcToSearch[pos];
      }      
   }

   return NULL;
#endif
}

static char *EME_UTILS_CreateAlphabetTable(void)
{
    char     *pcAlphabet     = NULL;
    char     *pcAlphabetTemp = NULL;
    char     cValueUppercase = 'A';
    char     cValueLowercase = 'a';
    char     cValueDigital   = '0';

    /*Allocate memory to the alphabet table pointer*/
    if (NULL == (pcAlphabet = (char *)WE_SCL_MALLOC(65*sizeof(char))))
    {
        return FALSE;
    }
    EME_UTILS_MEMSET(pcAlphabet, 0, 65*sizeof(char));
    pcAlphabetTemp = pcAlphabet;
    /*'A'~'Z' character*/
    while(cValueUppercase <= 'Z')
    {
        *pcAlphabetTemp = cValueUppercase;
        pcAlphabetTemp++;
        cValueUppercase++;
    }
    /*'a'~'z' character*/
    while (cValueLowercase <= 'z')
    {
        *pcAlphabetTemp = cValueLowercase;
        pcAlphabetTemp++;
        cValueLowercase++;
    }
    /*'0'~'9' character*/
    while (cValueDigital <= '9')
    {
        *pcAlphabetTemp = cValueDigital;
        pcAlphabetTemp++;
        cValueDigital++;
    }
    /*'+','/' character*/
    *pcAlphabetTemp = '+';
    pcAlphabetTemp++;
    
    *pcAlphabetTemp = '/';
    
    return pcAlphabet;
}

/*=====================================================================================
DESCRIPTION:
    Create a base-64 encoding data, and place it in the output buffer.
ARGUMENTS PASSED:
    pcInBuf: the buffer that storage input character 
    iInLen:  the length of the input character
    pcOutBuf:the buffer that storage output character
    piOutLen:the length of output character had encoded.On input, 'piOutLen' should
             hold the maximum size of the output buffer,on output, it will hold
             the length of the string written there.
=====================================================================================*/

int EME_UTILS_Base64Encode
(
    const char   *pcInBuf, 
    int        iInLen, 
    char         *pcOutBuf, 
    int        *piOutLen
)
{
    int        iLen = ((iInLen + 2) / 3) * 4;
    unsigned char        *pucTemp;
    int        iOrigCount;
    int        iEncodedCount;

    unsigned char        *pcBsae64 = NULL;
    
    if (!pcInBuf || !pcOutBuf || !piOutLen || iInLen == 0)
    {
        return -1;
    }
    /* The base64 decode table */
    if (NULL == (pcBsae64 = (unsigned char*)EME_UTILS_CreateAlphabetTable()))
    {
        return -1;
    }
    if (*piOutLen < iLen + 1) 
    {
        /* There is something wrong with the length */
        *piOutLen = 0;
        WE_SCL_FREE(pcBsae64);
        pcBsae64 = NULL;
        return -1;
    }
    *piOutLen = iLen;

    /* Encode 3*n characters */
    for (iOrigCount = 0, iEncodedCount = 0, pucTemp = (unsigned char *)pcOutBuf;
       iOrigCount + 2 < iInLen; iOrigCount += 3, pucTemp += 4, iEncodedCount += 4) 
    {
        pucTemp[0] = pcBsae64[(pcInBuf[iOrigCount] >> 2) & 0x3f];
        pucTemp[1] = pcBsae64[((pcInBuf[iOrigCount] << 4) & 0x30) | ((pcInBuf[iOrigCount + 1] >> 4) & 0x0f)];
        pucTemp[2] = pcBsae64[((pcInBuf[iOrigCount + 1] << 2) & 0x3c) | ((pcInBuf[iOrigCount + 2] >> 6) & 0x03)];
        pucTemp[3] = pcBsae64[pcInBuf[iOrigCount + 2] & 0x3f];
    }/* End for */
   
    /* Check whether the number of original string is 3*n */
    if (iOrigCount < iInLen)
    {
        /* The number of original string is not 3*n */
        pucTemp[0] = pcBsae64[(pcInBuf[iOrigCount] >> 2) & 0x3f];
        /* Supply the encode string with pad */
        if (iOrigCount + 1 < iInLen)
        {
            /* The number of character is (3*n + 2), supply one pad to the end of the encoded string */
            pucTemp[1] = pcBsae64[((pcInBuf[iOrigCount] << 4) & 0x30) | ((pcInBuf[iOrigCount + 1] >> 4) & 0x0f)];
            pucTemp[2] = pcBsae64[((pcInBuf[iOrigCount + 1] << 2) & 0x3c)];
        }
        else
        {
            /* The number of character is (3*n + 1), supply two pad to the end of the encoded string */
            pucTemp[1] = pcBsae64[((pcInBuf[iOrigCount] << 4) & 0x30)];
            pucTemp[2] = '=';
        }
        pucTemp[3] = '=';
    }/* End if */
    pcOutBuf[iLen] = '\0';
    
    WE_SCL_FREE(pcBsae64);
    pcBsae64 = NULL;
    return 0;
}



WE_CHAR  *MMA_UTILS_GetExt(WE_CHAR  *pcFileName)
{
   WE_CHAR  *pcTmp = NULL;
   WE_BOOL  bFound = FALSE;

   if (pcFileName == NULL)
   {
      return NULL;
   }

   pcTmp = (pcFileName) + EME_UTILS_STRLEN(pcFileName);
   while (pcTmp > pcFileName)
   {
      if (*pcTmp == '.')
      {
         if (pcTmp == (pcFileName) + EME_UTILS_STRLEN(pcFileName))
         {
            bFound = FALSE;
            break;
         }
         bFound = TRUE;
         pcTmp++;
         break;
      }
      pcTmp--;
   }

   if (bFound)
   {
      return pcTmp;
   }
   else
   {
      return NULL;
   }
}


AECHAR* EME_UTILS_WSTRDUPREF(void* pMemHdr, AECHAR* p, const char* pFileName, int dwLine)
{
   AECHAR * pTemp = NULL;
   
   if(NULL == p)
   {
      return NULL;
   } 
   
   if(NULL !=  (pTemp = (AECHAR*)EME_UTILS_MALLOC(pMemHdr, sizeof(AECHAR) * (WE_SCL_WSTRLEN(p) + 1))))
   {
      EME_UTILS_MEMSET(pTemp, 0x00, sizeof(AECHAR) * (WE_SCL_WSTRLEN(p) + 1));
      EME_UTILS_MEMCPY(pTemp , p, WE_SCL_WSTRLEN(p) * sizeof(AECHAR));
   }       
   return pTemp;       
}


char* EME_UTILS_STRDUPREF(void* pMemHdr, char* p, const char* pFileName, int dwLine)
{
   char * pTemp = NULL;
//   TF3RD_DBGPRINTF("EME_UTILS_STRDUPREF FileName = %s, dwLine= %d \r\n",pFileName,dwLine);
   if(NULL == p)
   {
      return   NULL;
   }
   
   if(NULL !=  (pTemp = (char*)Tf3rd2_0MallocMemory(sizeof(char) * (WE_SCL_STRLEN(p) + 1), pFileName, dwLine)))
   {
      EME_UTILS_MEMSET(pTemp, 0x00, sizeof(char) * (WE_SCL_STRLEN(p) + 1));
      EME_UTILS_MEMCPY(pTemp , p, (WE_SCL_STRLEN(p) + 1) *sizeof(char));
   }

   return pTemp;
}

void *EME_Utils_StrRealloc(St_EME_MemHandle *pMemHdr, void *pStr, unsigned long nSize, const char *pFileName, int dwLine)
{
   char *pcNew = NULL;
   unsigned long uiSize = 0;

   if(NULL == pStr)
   {
      pcNew = EME_UTILS_MALLOC(pMemHdr, nSize);
      if(NULL != pcNew)
      {
         EME_UTILS_MEMSET(pcNew, 0, nSize);
      }
      return pcNew;
   }

   uiSize = EME_UTILS_STRLEN(pStr);
   if(nSize <= uiSize)
   {
      return NULL;
   }

   pcNew = EME_UTILS_MALLOC(pMemHdr, nSize);
   if(NULL == pcNew)
   {
      return NULL;
   }

   EME_UTILS_MEMSET(pcNew, 0x00, nSize);
   EME_UTILS_MEMCPY(pcNew, pStr, uiSize);
   EME_UTILS_FREEIF(pMemHdr, pStr);
   pStr = NULL;

   return pcNew;
}




static WE_BOOL WE_CSC_Utf8ToUcs2(WE_CHAR *pcSrc, 
                                 WE_LONG *plSrcLen, 
                                 WE_CHAR *pcDst, 
                                 WE_LONG *plDstLen, 
                                 WE_BOOL bBigEndian)
{   
   WE_INT32        iSindex       = 0;
   WE_INT32        iDindex       = 0;
   WE_INT32        iSouNumOfByte = 0; 
   WE_INT32        iDesNumOfByte = 0;
   WE_UINT32       uiResult      = 0;
   WE_UINT32       uiNumFirst    = 0; 
   WE_UINT32       uiNumSecond   = 0;
   
   if (NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }

   if (bBigEndian)
   {
      uiNumFirst = 0; 
      uiNumSecond = 1;
   }
   else
   {
      uiNumFirst = 1; 
      uiNumSecond = 0;
   }
   
   while (iSindex < *plSrcLen)
   {
      /* Read one Unicode character in UTF-8 format: */
      iSouNumOfByte = aucUtf8BytePerChar[(WE_UCHAR)*pcSrc >> 4];
      if (iSindex + iSouNumOfByte > *plSrcLen)
         break;
      
      uiResult = 0;
      switch (iSouNumOfByte) 
      {
      case 0:
         return FALSE;
         
      case 4: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 3: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 2: uiResult += (WE_UCHAR)*pcSrc++; uiResult <<= 6;
      case 1: uiResult += (WE_UCHAR)*pcSrc++;
      }
      
      uiResult -= aunUtf8Offset[iSouNumOfByte];
      
      /* Write one Unicode character in UCS-2 format: */
      uiResult &= 0xffff;
      
      
      
      if (pcDst != NULL) 
      {
         if (iDindex + 2 > *plDstLen)
            break;
         
         pcDst[iDindex + uiNumFirst] = (WE_CHAR)((uiResult >> 8) & 0xff);
         pcDst[iDindex + uiNumSecond] = (WE_CHAR)((uiResult & 0xff));
      }
      
      iDesNumOfByte = 2;
      iSindex += iSouNumOfByte;
      iDindex += iDesNumOfByte;
   }
   
   *plSrcLen = iSindex;
   *plDstLen = iDindex;
       
   return TRUE;
}//   WE_CSC_Utf8ToUcs2any


static WE_BOOL WE_CSC_Ucs2ToUtf8(WE_CHAR *pcSrc, 
                                 WE_LONG *plSrcLen, 
                                 WE_CHAR *pcDst, 
                                 WE_LONG *plDstLen, 
                                 WE_BOOL bBigEndian)
{
   WE_UINT32   uiTransToUcs   = 0;
   WE_UINT32   uiSindex       = 0;
   WE_UINT32   uiDindex       = 0;
   WE_INT32    iSouNumOfByte  = 0; 
   WE_INT32    iDesNumOfByte  = 0;
   WE_UINT8    ucMaskOfUtf8   = 0;
   WE_INT32    iShiftFirst    = 0;
   WE_INT32    iShiftSecond   = 0;
   
   if (NULL == pcSrc || NULL == plSrcLen || *plSrcLen <= 0 || NULL == plDstLen) 
   {
      return FALSE;
   }
   
   if (bBigEndian)
   {
      iShiftFirst = 0; iShiftSecond = 1; 
   }
   else
   {
      iShiftFirst = 1; iShiftSecond = 0; 
   }
   
   while(1)
   {
      /* First, translate to UCS4 */
      if ((WE_LONG)(uiSindex + 2 )> *plSrcLen)
         break;
      uiTransToUcs = ((WE_UCHAR)pcSrc[uiSindex + iShiftFirst] << 8) |
         (WE_UCHAR)pcSrc[uiSindex + iShiftSecond];
      iSouNumOfByte = 2;
      
      /* Determine how many UTF8-bytes are needed for this character,
      * and which bits should be set in the first byte. */
      if (uiTransToUcs < 0x80)
      {
         iDesNumOfByte = 1;
         ucMaskOfUtf8 = 0;
      }
      else if (uiTransToUcs < 0x800)
      {
         iDesNumOfByte = 2;
         ucMaskOfUtf8 = 0xc0;
      }
      else 
      {
         iDesNumOfByte = 3;
         ucMaskOfUtf8 = 0xe0;
      }
      
      /* Only write to destination vector if pcDst != MSF_NULL */
      if (pcDst != NULL) 
      {
         /* Is there room for this in the destination vector? */
         if ((WE_LONG)(uiDindex + iDesNumOfByte) > *plDstLen)
            break;
         
         switch (iDesNumOfByte)
         {
         case 3:
            pcDst[uiDindex + 2] = (WE_UCHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 2:
            pcDst[uiDindex + 1] = (WE_UCHAR) ((uiTransToUcs & 0x3f) | 0x80);
            uiTransToUcs >>= 6;
         case 1:
            pcDst[uiDindex] = (WE_UCHAR) (uiTransToUcs | ucMaskOfUtf8);
         }
      }
      
      uiSindex += iSouNumOfByte;
      uiDindex += iDesNumOfByte;
   }
   *plSrcLen = uiSindex;
   *plDstLen = uiDindex;

   return TRUE;
}//   WE_CSC_Ucs2AnyToUtf8


/*get the current system time*/
WE_UINT32 EME_UTILS_GetTimems(void)
{
   TimeType      jlCurr = {0};

   Tf3rd2_GetDateTime(&jlCurr);
   
   return Tf3rd0_34MKTime(&jlCurr);  
   
}
