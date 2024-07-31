/******************************************************************************/
// File Name:       McfStdlib.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     ���������׼�⺯���Ľӿ�                                                         
/******************************************************************************/
#ifndef _MCFSTDLIB_H_
#define _MCFSTDLIB_H_
#ifdef __cplusplus   
extern "C"{   
#endif // __cplusplus

/******************************************************************************/
//                      include files
/******************************************************************************/

#include "mcfcommon.h"
/******************************************************************************/
//                      Macro Declaration
/******************************************************************************/

/******************************************************************************/
//                      Type and constant
/******************************************************************************/

/******************************************************************************/
//                      Local variables Declaration
/******************************************************************************/

/******************************************************************************/
//                      Local function extern
/******************************************************************************/

/******************************************************************************/
//                      function declare
/******************************************************************************/
/******************************************************************************/
//	FUNCTION: McfStdC_strlen
//	Descrption: ��ȡ�ַ����ĳ���
//	Param:
//	IN:   pStr:��Ҫ��ȡ���ȵ��ַ���
//	OUT:  
//	Return: MUINT ���ص�ǰ�ַ����ĳ���.
//  Note:   pStrΪ��ʱ����Ϊ0
/******************************************************************************/
MUINT McfStdC_strlen(const MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_strcmp
//	Descrption: �Ƚ������ַ���
//	Param:
//	IN:         pStr0:��Ҫ�Ƚϵ��ַ���1
//              pStr1:��Ҫ�Ƚϵ��ַ���2
//	OUT:  
//	Return:     <0:pStr0<pStr1  0:pStr0==pStr1 >0: pStr0>pStr1
//  Note:       ��pStr0��pStr1������һ��Ϊ��ʱ������-1
/******************************************************************************/
MINT McfStdC_strcmp(const MCHAR* pStr0,  const MCHAR* pStr1);
/******************************************************************************/
//	FUNCTION: McfStdC_stricmp
//	Descrption: ��������ĸ��Сд�Ƚ������ַ���
//	Param:
//	IN:         pStr0:��Ҫ�Ƚϵ��ַ���1
//              pStr1:��Ҫ�Ƚϵ��ַ���2
//	OUT:  
//	Return:     MINT ���ص�ǰ�ַ����ıȽϽ��
//  Note:   <0:pStr0<pStr1  0:pStr0==pStr1 >0: pStr0>pStr1
/******************************************************************************/
MINT McfStdC_stricmp(const MCHAR* pStr0, const MCHAR* pStr1);
/******************************************************************************/
//	FUNCTION: McfStdC_strcat
//	Descrption: �����ַ�������
//	Param:
//	IN:         pStr0:��Ҫ�Ƚϵ��ַ���1
//              pStr1:��Ҫ�Ƚϵ��ַ���2
//	OUT:        pStr0:�µ��ַ������ݵ�ָ��
//	Return:     �����µ��ַ������ݵĵ�ַ
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strcat(MCHAR* pStrDest,  const MCHAR* pStrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_strcpy
//	Descrption: �����ַ�������
//	Param:
//	IN:         pStr0:�ַ���1
//              pStr1:�ַ���2
//	OUT:        pStr0:�µ��ַ������ݵ�ָ��
//	Return:     �����µ��ַ������ݵĵ�ַ
//  Note:      
/******************************************************************************/
MCHAR* McfStdC_strcpy(MCHAR* pStrDest, const MCHAR* pStrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_strncpy
//	Descrption: ָ�����ȵ��ַ����Ŀ���
//	Param:
//	IN:         pStr0:�ַ���1
//              pStr1:�ַ���2
//              nSize:��������
//	OUT:        pStr0:�µ��ַ������ݵ�ָ��
//	Return:     �����µ��ַ������ݵĵ�ַ
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strncpy(MCHAR* pStrDest, const MCHAR* pStrSrc, MUINT nSize);
/******************************************************************************/
//	FUNCTION: McfStdC_strchr
//	Descrption: ���ַ����в���ָ���ַ�
//	Param:
//	IN:         pStr:[in] �����ҵ��ַ���
//              chr:[in] ���ҵ��ַ�
//	OUT:        
//	Return:     ����ɹ��򷵻ص�һ�β��ҵ���λ�õ�ָ�룬���򷵻�MNULL
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strchr(const MCHAR* pStr, MINT chr);

MCHAR*	McfStdC_strrchr(const MCHAR* pDest, MINT nChr);
/******************************************************************************/
//	FUNCTION: McfStdC_strchr
//	Descrption: �����ַ������ַ������׸�λ��
//	Param:
//	IN:         pStr:�ַ���
//              pStrSearch:�����ҵ����ַ���
//	OUT:        
//	Return:     ����ɹ��򷵻ص�һ�β��ҵ���λ�õ�ָ�룬���򷵻�MNULL
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strstr(const MCHAR* pStr, const MCHAR* pStrSearch);
/******************************************************************************/
//	FUNCTION: McfStdC_atoi
//	Descrption: ���ַ���תΪ����
//	Param:
//	IN:         pStr:�ַ���
//	OUT:        
//	Return:     �����ַ�����Ӧ����ֵ
//  Note:    
/******************************************************************************/
MINT McfStdC_atoi(const MCHAR* pStr);
/******************************************************************************/
//	FUNCTION: McfStdC_strlwr
//	Descrption: �ַ����еĴ�д��ĸת����Сд��ĸ
//	Param:
//	IN:         pStr:�ַ���
//	OUT:        
//	Return:     ����ת���ɺ���ַ���
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strlwr(MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_strupr
//	Descrption: �ַ����е�Сд��ĸת���ɴ�д��ĸ
//	Param:
//	IN:         pStr:�ַ���
//	OUT:        
//	Return:     ����ת���ɺ���ַ���
//  Note:      
/******************************************************************************/
MCHAR* McfStdC_strupr(MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_wcslen
//	Descrption: ��ȡ���ַ����ĳ���
//	Param:
//	IN:         pStr:�ַ���
//	OUT:        
//	Return:     ���ص�ǰ�ַ����ĳ���
//  Note:       ��pStr Ϊ��ֵʱ����0
/******************************************************************************/
MUINT McfStdC_wcslen(const MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscmp
//	Descrption: �Ƚ����ַ����Ĵ�С
//	Param:
//	IN:         pwstr0:�ַ���0
//	            pwstr1:�ַ���1
//	OUT:        
//	Return:     -1:pwstr0<pwstr1  0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       �ǿ�> ��:����:pwstr0==MNULL ,pwstr1!=MNULL ,pwstr0<pwstr1
/******************************************************************************/
MINT McfStdC_wcscmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsicmp
//	Descrption: ��������ĸ��Сд�رȽ����ַ����Ĵ�С
//	Param:
//	IN:         pwstr0:�ַ���0
//	            pwstr1:�ַ���1
//	OUT:        
//	Return:     -1:pwstr0<pwstr1  0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       �ǿ�> ��:����:pwstr0==MNULL ,pwstr1!=MNULL ,pwstr0<pwstr1
/******************************************************************************/
MINT McfStdC_wcsicmp(const MWCHAR* pwstr0, const MWCHAR* pwstr1);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscat
//	Descrption: �ַ����ļ���
//	Param:
//	IN:         pwstrDest:�ַ���0
//	            pwstrSrc:�ַ���1
//	OUT:        
//	Return:     pwstrSrc ���Ӻ���ַ����ĵ�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcscat(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscpy
//	Descrption: �ַ����Ŀ���
//	Param:
//	IN:         pwstrDest:�ַ���0
//	            pwstrSrc:�ַ���1
//	OUT:        
//	Return:     pwstrSrc ��������ַ����ĵ�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcscpy(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wcschr
//	Descrption: �ַ��������ַ����ֵ��׸���ַ
//	Param:
//	IN:         pwstr:�ַ���
//	            wchar:�ַ�
//	OUT:        
//	Return:     wchar���ֵ��׸���ַ
//  Note:       ��pwstrΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcschr(const MWCHAR* pwstr,MWCHAR wchar);

/******************************************************************************/
//	FUNCTION: McfStdC_wcsncmp
//	Descrption: �ַ���ָ��λ���ıȽ�
//	Param:
//	IN:         pwstr0:�ַ���0
//	            pwstr1:�ַ���1
//              icount:�Ƚ�λ��
//	OUT:        
//	Return:     ���رȽϽ��-1: pwstr0<pwstr1 0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       ��pwstrΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MINT McfStdC_wcsncmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsncpy
//	Descrption: �ַ���ָ��λ���Ŀ���
//	Param:
//	IN:         pwstr0:�ַ���0
//	            pwstr1:�ַ���1
//              icount:����λ��
//	OUT:        pwstrDest
//	Return:     �����µ��ַ�������׵�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcsncpy(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsncpy
//	Descrption: �ַ���ָ��λ���ļ���
//	Param:
//	IN:         pwstr0:�ַ���0
//	            pwstr1:�ַ���1
//              icount:����λ��
//	OUT:        pwstrDest
//	Return:     �����µ��ַ�������׵�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcsncat(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsstr
//	Descrption: �ַ����������ַ������׵�ַ
//	Param:
//	IN:         pStr:�ַ���
//	            pStrSearch:���ַ���
//	OUT:        
//	Return:     ���ز��ҵ������ַ����е��׵�ַ
//  Note:       ��pStrΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_wcsstr(const MWCHAR* pStr,const MWCHAR* pStrSearch);
/******************************************************************************/
//	FUNCTION: McfStdC_wcslwr
//	Descrption: ת���ַ����еĴ�д��ĸ��Сд��ĸ
//	Param:
//	IN:         pwstr:�ַ���
//	OUT:        
//	Return:     �����µ��ַ������׵�ַ
//  Note:       
/******************************************************************************/
MWCHAR* McfStdC_wcslwr(MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsupr
//	Descrption: ת���ַ����е�Сд��ĸ�ɴ�д��ĸ
//	Param:
//	IN:         pwstr:�ַ���
//	OUT:        
//	Return:     �����µ��ַ������׵�ַ
//  Note:      
/******************************************************************************/
MWCHAR* McfStdC_wcsupr(MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_strtowstr
//	Descrption:��acsii�ַ���ת�������Ӧ��unicode�ַ���
//	Param:
//	IN:         pstrSrc:acsii�ַ���
//	OUT:        pwstrDest:uincode�ַ���
//	Return:     �����µ��ַ������׵�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MWCHAR* McfStdC_strtowstr(MWCHAR* pwstrDest, const MCHAR* pstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wstrtostr
//	Descrption:��unicode�ַ���ת�������Ӧ��acsii�ַ���
//	Param:
//	IN:         pwstrSrc:uincode�ַ���
//	OUT:        pstrDest:acsii�ַ���
//	Return:     �����µ��ַ������׵�ַ
//  Note:       ��pwstrDestΪ��ֵ�Ƿ��ؿ�ֵ
/******************************************************************************/
MCHAR* McfStdC_wstrtostr(MCHAR* pstrDest, const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wsprintf
//	Descrption:��ʽ����ӡ
//	Param:
//	IN:         pwstrFmt:��ʽ����
//	OUT:        pwstrDest:��ʽ���ַ����Ĵ洢��ַ
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_wsprintf(MWCHAR* pwstrDest, const MCHAR* pwstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_sprintf
//	Descrption:��ʽ����ӡ
//	Param:
//	IN:         pwstrFmt:��ʽ����
//	OUT:        pwstrDest:��ʽ���ַ����Ĵ洢��ַ
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_sprintf(MCHAR* pstrDest, const MCHAR* pstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_nsprintf
//	Descrption:��ʽ����ӡ
//	Param:
//	IN:         pwstrFmt:��ʽ����
//	OUT:        pwstrDest:��ʽ���ַ����Ĵ洢��ַ
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_nsprintf(MCHAR* pstrDest, MINT ilengthDest, const MCHAR* pstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_vsnprintf
//	Descrption:��ʽ����ӡ
//	Param:
//	IN:         pwstrFmt:��ʽ����
//	OUT:        pwstrDest:��ʽ���ַ����Ĵ洢��ַ
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_vsnprintf(MCHAR* pstrDest, MUINT nSize, const MCHAR* pstrFmt, va_list aList);
/******************************************************************************/
//	FUNCTION: McfStdC_memcmp
//	Descrption:�Ƚ������ڴ�����ǰcount���ַ����ݣ�ascii��ֵ����ַ��Ƚ�
//	Param:
//	IN:         buf1--[in] Դ�ڴ�����1
//			buf2--[in] Դ�ڴ�����2
//			count--[in] Ҫ�Ƚϵ��ַ�����
//	OUT:      
//	Return:     0  -- buf1�� buf2���
//                   >0 -- buf1���� buf2
//                   <0 -- buf1С�� buf2
//  Note:       
/******************************************************************************/
MINT McfStdC_memcmp(const MVOID* buf1, const MVOID* buf2, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memcpy
//	Descrption�ڴ濽��
//	Param:
//	IN:           pSrc:Դ��ַ
//                   count:������С
//	OUT:        pDst:Ҫ�������ĵ�ַ
//	Return:     pDst 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memcpy(MVOID* pDst, const MVOID* pSrc, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memset
//	Descrption�ڴ渳ֵ
//	Param:
//	IN:         buf1:Դ��ַ
//              count:buff��С
//              value: ֵ
//	OUT:        buf1:
//	Return:     buf1�� 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memset(MVOID* buf1, MINT set_value, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memchr
//	Descrption�ڴ����
//	Param:
//	IN:         buf1:Դ��ַ
//              count:buff��С
//              chr: ������ֵ
//	OUT:        
//	Return:     ���ҵ���λ�ã� 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memchr(const MVOID* buf1, MINT chr, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memchr
//	Descrption:�ƶ�Դ�ڴ�����ǰcount���ַ���Ŀ���ڴ���
//	Param:
//	IN:      pDst--[in] Ŀ���ڴ���
//		   pSrc--[in] Դ�ڴ���
//		   count--[in] Ҫ�ƶ����ַ�����
//	OUT:        pDst
//	Return:     Ŀ���ڴ���pDst
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memmove(MVOID* pDst, const MVOID* pSrc, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_srand
//	Descrption
//	Param:
//	IN:         seed:
//	OUT:        
//	Return:     
//  Note:       
/******************************************************************************/
MVOID McfStdC_srand (MUINT seed); 
/******************************************************************************/
//	FUNCTION: McfStdC_rand
//	Descrption
//	Param:
//	IN:         VOID:
//	OUT:        
//	Return:     MINT
//  Note:       
/******************************************************************************/
MINT McfStdC_rand(MVOID);

/******************************************************************************/
#ifdef __cplusplus   
}   
#endif // __cplusplus

#endif//#ifndef _MCFSTDLIB_H_

