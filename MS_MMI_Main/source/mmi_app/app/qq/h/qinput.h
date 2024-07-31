/*****************************************************************************
** File Name:      qinput.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ GUI function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef _QQ_INPUT_H_
#define _QQ_INPUT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "datatype.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define QQ_EDIT_WIN_TITLE_WLEN          20
#define QQ_EDIT_WIN_BUFFER_WLEN         200

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef void (*pfnLeft)(void* pUser, QU16* pText);

typedef enum
{
    MMIQQ_TM_NUMBER,//����
    MMIQQ_TM_PSWORD,//����
    MMIQQ_TM_ALL,//�������뷨
    MMIQQ_TM_MAX
}MMIQQ_TM_E;

typedef struct
{
//    QU16 title[QQ_EDIT_WIN_TITLE_WLEN+1];//titleĿǰ��δʹ��!!!
//    QU16 title_wlen;//˫�ֽڳ���
    QU16 buffer[QQ_EDIT_WIN_BUFFER_WLEN+1];
    QU16 buffer_wlen;//˫�ֽڳ���
    pfnLeft handleLeftKey;
    void* pUser;
    QU16  maxsize;//�ֽڳ���
    QU16  type;//MMIQQ_TM_E
}MMIQQ_EDIT_WIN_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: ��ʼ���༭����
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QInitEditWin(QU16* title, QU16* buffer, QU16 maxsize, QU16 type, void* pUser, pfnLeft handleLKey);
/*****************************************************************************/
//  Description: ����༭����
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QEntryEditWin(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
