/**************************************************************************
 hanzi_asso.h
 
     Description:
     
     Author:      ke wen (kew@cstar.com.cn)
     Date Time:   12/18/2005 9:27:40 AM

     Copyright (C) 2000-2005 Beijing Chinese Star Cyber Technology Ltd.
     All rights reserved.

 **************************************************************************/

#ifndef __HANZI_ASSO_H__
#define __HANZI_ASSO_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 以下三个函数与上面的三个相同，为避免命名冲突才出现。上面三个用于数字键盘放式，下面三个用于全键盘的触摸屏 */
CS_HANDLE HanziAssoFull_Initialize(void);
void      HanziAssoFull_Terminate(CS_HANDLE hAsso);

CS_RESULT   HanziAssoFull_QueryAsso(CS_HANDLE hAsso,		/* init 返回的句柄*/
								CS_PCWSTR pwzContext,	/* 上下文内容unicode字符串，如“开始” */
								CS_INT nLen,			/* 上下文内容的长度，如 1 */
								CS_INT pos,				/* 在上下文中的插入位置，如 1*/
                                CS_PWSTR pwzAsso,		/* 返回的联想候选序列*/
								CS_INT *pnAsso);		/* 输入为所要的联想最大个数，返回的是实际候选个数*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__HANZI_ASSO_H__*/
