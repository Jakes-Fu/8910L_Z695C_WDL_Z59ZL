#ifdef KURO_SUPPORT
/********************************************************************************
*                        USDK130
*                     	music sub
*
* File: kdbpaser.h
* By  : lizx
* Version: 1> v1.00     first version     2007-9-13 8:28
*******************************************************************************
*/
#ifndef __KDBPARSER_H_
#define __KDBPARSER_H_

#define KDB_MAX_PATH_LEN  MMIFILE_FULL_PATH_MAX_LEN 
/*****************************************************************************/
// 	Description :初始化kdb模块
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbinit(void);
/*****************************************************************************/
// 	Description :释放kdb模块所分配的资源
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbuninit(void);
/*****************************************************************************/
// 	Description :获取所有曲风类型名称(歌手，本周精选...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char** MMIKDB_get_classlist(void);
/*****************************************************************************/
// 	Description :获取所有曲风类型数量(歌手，本周精选...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_classescount(void);
/*****************************************************************************/
// 	Description :获取某一曲风类型的子项列表(如所有歌手)
//		nclassid:曲风类型的编号
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_typelist(int nclassid);

/*****************************************************************************/
/*
	获取某一曲风类型某一子项的歌曲列表(如某个歌手的所有歌曲)
	ntypeid:曲风类型子项的编号
*///	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_songslist(int nclassid,int ntypeid,int* nsongscount);
/*****************************************************************************/
/*
	根据typeid及选中的歌曲Index 获取歌曲路径
*///  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_get_songpath(int nclassid,int ntypeid,int nindex,char *psongpath,uint32 npathlen);
/*****************************************************************************/
/*
	获取某一曲风类型的子项数量(如所有歌手)
	nclassid:曲风类型的编号
*/
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_class_typecount(uint32 nclassid);
#endif
#endif

