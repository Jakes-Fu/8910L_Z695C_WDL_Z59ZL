/*****************************************************************************
** File Name:      gui.h                                                  *
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
#ifndef MQQ_H
#define MQQ_H


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

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
 
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef struct Mqq Mqq;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
Mqq*		Mqq_Create(void);
void		Mqq_Free(Mqq* me); 
void		Mqq_TimerNotify(Mqq* me);

void		Mqq_Start(Mqq* me); 
void		Mqq_Redraw(Mqq* me);
void		Mqq_Suspend(Mqq* me); 
void		Mqq_Resume(Mqq* me); 
void        Mqq_Eixt(Mqq* me);
void		Mqq_HandleKey(Mqq* me,unsigned short keyCode, unsigned short keyType); 
void		Mqq_HandlePen(Mqq* me,unsigned short penType,unsigned short x,unsigned short y);		

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif		
#endif
