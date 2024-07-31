#ifdef KURO_SUPPORT
/********************************************************************************
*                        USDK130
*                       music sub
*
* File: mmikur_musicstyle.h
* By  : lics
* Version: 1> v1.00     first version     2007-10-15
*******************************************************************************
*/
#ifndef __MMIKUR_MUSICSTYLE_H_
#define __MMIKUR_MUSICSTYLE_H_

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
PUBLIC void MMIAPIKUR_OpenKurMusicStyle(void);
PUBLIC void MMKKUR_MUSICSTYLE_CreatePlayListWindow(void);
/*****************************************************************************/
//  Description : 从kdb文件中获取song路径
//  Global resource dependence : none
//  Author: cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_GetKdbSongPath(uint16 *uc2s_song_path,uint16 *song_path_len,uint16 nclassindex,uint16 ntypeindex,uint16 nsongindex);
#endif
#endif

