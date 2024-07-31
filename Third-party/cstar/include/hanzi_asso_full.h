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

/* �������������������������ͬ��Ϊ����������ͻ�ų��֡����������������ּ��̷�ʽ��������������ȫ���̵Ĵ����� */
CS_HANDLE HanziAssoFull_Initialize(void);
void      HanziAssoFull_Terminate(CS_HANDLE hAsso);

CS_RESULT   HanziAssoFull_QueryAsso(CS_HANDLE hAsso,		/* init ���صľ��*/
								CS_PCWSTR pwzContext,	/* ����������unicode�ַ������硰��ʼ�� */
								CS_INT nLen,			/* ���������ݵĳ��ȣ��� 1 */
								CS_INT pos,				/* ���������еĲ���λ�ã��� 1*/
                                CS_PWSTR pwzAsso,		/* ���ص������ѡ����*/
								CS_INT *pnAsso);		/* ����Ϊ��Ҫ�����������������ص���ʵ�ʺ�ѡ����*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__HANZI_ASSO_H__*/
