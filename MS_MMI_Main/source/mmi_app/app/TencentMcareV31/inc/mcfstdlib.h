/******************************************************************************/
// File Name:       McfStdlib.h                                           
// Author:          wilsonzhu wiltan                                 
// DATE:            10/01/2011                                  
// Modifed:         haijunqiu 09/26/2011
// Copyright:       2011 Tencent, Incoporated. All Rights Reserved.
// Description:     用来定义标准库函数的接口                                                         
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
//	Descrption: 获取字符串的长度
//	Param:
//	IN:   pStr:需要获取长度的字符串
//	OUT:  
//	Return: MUINT 返回当前字符串的长度.
//  Note:   pStr为空时长度为0
/******************************************************************************/
MUINT McfStdC_strlen(const MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_strcmp
//	Descrption: 比较两个字符串
//	Param:
//	IN:         pStr0:需要比较的字符串1
//              pStr1:需要比较的字符串2
//	OUT:  
//	Return:     <0:pStr0<pStr1  0:pStr0==pStr1 >0: pStr0>pStr1
//  Note:       当pStr0或pStr1当中有一个为空时，返回-1
/******************************************************************************/
MINT McfStdC_strcmp(const MCHAR* pStr0,  const MCHAR* pStr1);
/******************************************************************************/
//	FUNCTION: McfStdC_stricmp
//	Descrption: 不区分字母大小写比较两个字符串
//	Param:
//	IN:         pStr0:需要比较的字符串1
//              pStr1:需要比较的字符串2
//	OUT:  
//	Return:     MINT 返回当前字符串的比较结果
//  Note:   <0:pStr0<pStr1  0:pStr0==pStr1 >0: pStr0>pStr1
/******************************************************************************/
MINT McfStdC_stricmp(const MCHAR* pStr0, const MCHAR* pStr1);
/******************************************************************************/
//	FUNCTION: McfStdC_strcat
//	Descrption: 剪接字符串内容
//	Param:
//	IN:         pStr0:需要比较的字符串1
//              pStr1:需要比较的字符串2
//	OUT:        pStr0:新的字符串内容的指针
//	Return:     返回新的字符串内容的地址
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strcat(MCHAR* pStrDest,  const MCHAR* pStrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_strcpy
//	Descrption: 拷贝字符串内容
//	Param:
//	IN:         pStr0:字符串1
//              pStr1:字符串2
//	OUT:        pStr0:新的字符串内容的指针
//	Return:     返回新的字符串内容的地址
//  Note:      
/******************************************************************************/
MCHAR* McfStdC_strcpy(MCHAR* pStrDest, const MCHAR* pStrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_strncpy
//	Descrption: 指定长度的字符串的拷贝
//	Param:
//	IN:         pStr0:字符串1
//              pStr1:字符串2
//              nSize:拷贝长度
//	OUT:        pStr0:新的字符串内容的指针
//	Return:     返回新的字符串内容的地址
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strncpy(MCHAR* pStrDest, const MCHAR* pStrSrc, MUINT nSize);
/******************************************************************************/
//	FUNCTION: McfStdC_strchr
//	Descrption: 将字符串中查找指定字符
//	Param:
//	IN:         pStr:[in] 待查找的字符串
//              chr:[in] 查找的字符
//	OUT:        
//	Return:     如果成功则返回第一次查找到的位置的指针，否则返回MNULL
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strchr(const MCHAR* pStr, MINT chr);

MCHAR*	McfStdC_strrchr(const MCHAR* pDest, MINT nChr);
/******************************************************************************/
//	FUNCTION: McfStdC_strchr
//	Descrption: 查找字符串在字符串的首个位置
//	Param:
//	IN:         pStr:字符串
//              pStrSearch:待查找的子字符串
//	OUT:        
//	Return:     如果成功则返回第一次查找到的位置的指针，否则返回MNULL
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strstr(const MCHAR* pStr, const MCHAR* pStrSearch);
/******************************************************************************/
//	FUNCTION: McfStdC_atoi
//	Descrption: 将字符串转为整形
//	Param:
//	IN:         pStr:字符串
//	OUT:        
//	Return:     返回字符串对应的数值
//  Note:    
/******************************************************************************/
MINT McfStdC_atoi(const MCHAR* pStr);
/******************************************************************************/
//	FUNCTION: McfStdC_strlwr
//	Descrption: 字符串中的大写字母转换成小写字母
//	Param:
//	IN:         pStr:字符串
//	OUT:        
//	Return:     返回转换成后的字符串
//  Note:       
/******************************************************************************/
MCHAR* McfStdC_strlwr(MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_strupr
//	Descrption: 字符串中的小写字母转换成大写字母
//	Param:
//	IN:         pStr:字符串
//	OUT:        
//	Return:     返回转换成后的字符串
//  Note:      
/******************************************************************************/
MCHAR* McfStdC_strupr(MCHAR* pStr);

/******************************************************************************/
//	FUNCTION: McfStdC_wcslen
//	Descrption: 获取宽字符串的长度
//	Param:
//	IN:         pStr:字符串
//	OUT:        
//	Return:     返回当前字符串的长度
//  Note:       当pStr 为空值时返回0
/******************************************************************************/
MUINT McfStdC_wcslen(const MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscmp
//	Descrption: 比较两字符串的大小
//	Param:
//	IN:         pwstr0:字符串0
//	            pwstr1:字符串1
//	OUT:        
//	Return:     -1:pwstr0<pwstr1  0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       非空> 空:例如:pwstr0==MNULL ,pwstr1!=MNULL ,pwstr0<pwstr1
/******************************************************************************/
MINT McfStdC_wcscmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsicmp
//	Descrption: 不区分字母大小写地比较两字符串的大小
//	Param:
//	IN:         pwstr0:字符串0
//	            pwstr1:字符串1
//	OUT:        
//	Return:     -1:pwstr0<pwstr1  0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       非空> 空:例如:pwstr0==MNULL ,pwstr1!=MNULL ,pwstr0<pwstr1
/******************************************************************************/
MINT McfStdC_wcsicmp(const MWCHAR* pwstr0, const MWCHAR* pwstr1);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscat
//	Descrption: 字符串的剪接
//	Param:
//	IN:         pwstrDest:字符串0
//	            pwstrSrc:字符串1
//	OUT:        
//	Return:     pwstrSrc 剪接后的字符串的地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcscat(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wcscpy
//	Descrption: 字符串的拷贝
//	Param:
//	IN:         pwstrDest:字符串0
//	            pwstrSrc:字符串1
//	OUT:        
//	Return:     pwstrSrc 拷贝后的字符串的地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcscpy(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wcschr
//	Descrption: 字符串查找字符出现的首个地址
//	Param:
//	IN:         pwstr:字符串
//	            wchar:字符
//	OUT:        
//	Return:     wchar出现的首个地址
//  Note:       当pwstr为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcschr(const MWCHAR* pwstr,MWCHAR wchar);

/******************************************************************************/
//	FUNCTION: McfStdC_wcsncmp
//	Descrption: 字符串指定位数的比较
//	Param:
//	IN:         pwstr0:字符串0
//	            pwstr1:字符串1
//              icount:比较位数
//	OUT:        
//	Return:     返回比较结果-1: pwstr0<pwstr1 0:pwstr0==pwstr1 1:pwstr0>pwstr1
//  Note:       当pwstr为空值是返回空值
/******************************************************************************/
MINT McfStdC_wcsncmp(const MWCHAR* pwstr0,const MWCHAR* pwstr1,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsncpy
//	Descrption: 字符串指定位数的拷贝
//	Param:
//	IN:         pwstr0:字符串0
//	            pwstr1:字符串1
//              icount:拷贝位数
//	OUT:        pwstrDest
//	Return:     返回新的字符串后的首地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcsncpy(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsncpy
//	Descrption: 字符串指定位数的剪接
//	Param:
//	IN:         pwstr0:字符串0
//	            pwstr1:字符串1
//              icount:剪接位数
//	OUT:        pwstrDest
//	Return:     返回新的字符串后的首地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcsncat(MWCHAR* pwstrDest,const MWCHAR* pwstrSrc,MINT icount);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsstr
//	Descrption: 字符串查找子字符串的首地址
//	Param:
//	IN:         pStr:字符串
//	            pStrSearch:子字符串
//	OUT:        
//	Return:     返回查找到的在字符串中的首地址
//  Note:       当pStr为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_wcsstr(const MWCHAR* pStr,const MWCHAR* pStrSearch);
/******************************************************************************/
//	FUNCTION: McfStdC_wcslwr
//	Descrption: 转换字符串中的大写字母成小写字母
//	Param:
//	IN:         pwstr:字符串
//	OUT:        
//	Return:     返回新的字符串的首地址
//  Note:       
/******************************************************************************/
MWCHAR* McfStdC_wcslwr(MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_wcsupr
//	Descrption: 转换字符串中的小写字母成大写字母
//	Param:
//	IN:         pwstr:字符串
//	OUT:        
//	Return:     返回新的字符串的首地址
//  Note:      
/******************************************************************************/
MWCHAR* McfStdC_wcsupr(MWCHAR* pwstr);
/******************************************************************************/
//	FUNCTION: McfStdC_strtowstr
//	Descrption:把acsii字符串转换成相对应的unicode字符串
//	Param:
//	IN:         pstrSrc:acsii字符串
//	OUT:        pwstrDest:uincode字符串
//	Return:     返回新的字符串的首地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MWCHAR* McfStdC_strtowstr(MWCHAR* pwstrDest, const MCHAR* pstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wstrtostr
//	Descrption:把unicode字符串转换成相对应的acsii字符串
//	Param:
//	IN:         pwstrSrc:uincode字符串
//	OUT:        pstrDest:acsii字符串
//	Return:     返回新的字符串的首地址
//  Note:       当pwstrDest为空值是返回空值
/******************************************************************************/
MCHAR* McfStdC_wstrtostr(MCHAR* pstrDest, const MWCHAR* pwstrSrc);
/******************************************************************************/
//	FUNCTION: McfStdC_wsprintf
//	Descrption:格式化打印
//	Param:
//	IN:         pwstrFmt:格式参数
//	OUT:        pwstrDest:格式化字符串的存储地址
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_wsprintf(MWCHAR* pwstrDest, const MCHAR* pwstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_sprintf
//	Descrption:格式化打印
//	Param:
//	IN:         pwstrFmt:格式参数
//	OUT:        pwstrDest:格式化字符串的存储地址
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_sprintf(MCHAR* pstrDest, const MCHAR* pstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_nsprintf
//	Descrption:格式化打印
//	Param:
//	IN:         pwstrFmt:格式参数
//	OUT:        pwstrDest:格式化字符串的存储地址
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_nsprintf(MCHAR* pstrDest, MINT ilengthDest, const MCHAR* pstrFmt,...);
/******************************************************************************/
//	FUNCTION: McfStdC_vsnprintf
//	Descrption:格式化打印
//	Param:
//	IN:         pwstrFmt:格式参数
//	OUT:        pwstrDest:格式化字符串的存储地址
//	Return:     0 
//  Note:       
/******************************************************************************/
MINT McfStdC_vsnprintf(MCHAR* pstrDest, MUINT nSize, const MCHAR* pstrFmt, va_list aList);
/******************************************************************************/
//	FUNCTION: McfStdC_memcmp
//	Descrption:比较两个内存区的前count个字符内容，ascii数值逐个字符比较
//	Param:
//	IN:         buf1--[in] 源内存内容1
//			buf2--[in] 源内存内容2
//			count--[in] 要比较的字符个数
//	OUT:      
//	Return:     0  -- buf1与 buf2相等
//                   >0 -- buf1大于 buf2
//                   <0 -- buf1小于 buf2
//  Note:       
/******************************************************************************/
MINT McfStdC_memcmp(const MVOID* buf1, const MVOID* buf2, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memcpy
//	Descrption内存拷贝
//	Param:
//	IN:           pSrc:源地址
//                   count:拷贝大小
//	OUT:        pDst:要拷贝到的地址
//	Return:     pDst 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memcpy(MVOID* pDst, const MVOID* pSrc, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memset
//	Descrption内存赋值
//	Param:
//	IN:         buf1:源地址
//              count:buff大小
//              value: 值
//	OUT:        buf1:
//	Return:     buf1， 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memset(MVOID* buf1, MINT set_value, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memchr
//	Descrption内存查找
//	Param:
//	IN:         buf1:源地址
//              count:buff大小
//              chr: 待查找值
//	OUT:        
//	Return:     查找到的位置， 
//  Note:       
/******************************************************************************/
MVOID* McfStdC_memchr(const MVOID* buf1, MINT chr, MUINT count);
/******************************************************************************/
//	FUNCTION: McfStdC_memchr
//	Descrption:移动源内存区的前count个字符到目标内存区
//	Param:
//	IN:      pDst--[in] 目标内存区
//		   pSrc--[in] 源内存区
//		   count--[in] 要移动的字符个数
//	OUT:        pDst
//	Return:     目标内存区pDst
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

