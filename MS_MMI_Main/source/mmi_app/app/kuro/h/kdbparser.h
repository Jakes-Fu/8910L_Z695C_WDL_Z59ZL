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
// 	Description :��ʼ��kdbģ��
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbinit(void);
/*****************************************************************************/
// 	Description :�ͷ�kdbģ�����������Դ
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_kdbuninit(void);
/*****************************************************************************/
// 	Description :��ȡ����������������(���֣����ܾ�ѡ...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char** MMIKDB_get_classlist(void);
/*****************************************************************************/
// 	Description :��ȡ����������������(���֣����ܾ�ѡ...)
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_classescount(void);
/*****************************************************************************/
// 	Description :��ȡĳһ�������͵������б�(�����и���)
//		nclassid:�������͵ı��
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_typelist(int nclassid);

/*****************************************************************************/
/*
	��ȡĳһ��������ĳһ����ĸ����б�(��ĳ�����ֵ����и���)
	ntypeid:������������ı��
*///	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC char ** MMIKDB_get_songslist(int nclassid,int ntypeid,int* nsongscount);
/*****************************************************************************/
/*
	����typeid��ѡ�еĸ���Index ��ȡ����·��
*///  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKDB_get_songpath(int nclassid,int ntypeid,int nindex,char *psongpath,uint32 npathlen);
/*****************************************************************************/
/*
	��ȡĳһ�������͵���������(�����и���)
	nclassid:�������͵ı��
*/
//	Global resource dependence : none
//  Author: cmzs
//	Note: 
/*****************************************************************************/
PUBLIC int MMIKDB_get_class_typecount(uint32 nclassid);
#endif
#endif

