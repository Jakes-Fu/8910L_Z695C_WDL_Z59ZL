/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMCommwsp.c
文件描述:WSP通讯方式绑定支持
文件说明:提供WSP通讯方式的绑定，实现在WSP下的通讯。
开发人员:刘刚
首次开发日期:2005-7-9
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-7-9              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/
  /*  包含头文件*/
#ifdef WIN32
#include "std_header.h"
#endif
#define _PIMCOMMWSP_C_
  
//#include "../h/pimcommwsp.h"
  
#ifdef HS_FEATURE_COMM_WSP_SUPPORT

  /*  数据结构声明*/
  
  /*  全局变量声明*/

  /*  功能函数声明*/

  /******************************************************************* 
  ** 函数名:HS_WSP_Prepare
  ** 功能描述：初始化WSP 相关数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_WSP_Prepare(CPim* pMe)
  {
  	return HS_SML_ERR_OK;
  }

  /******************************************************************* 
  ** 函数名:HS_WSP_Release
  ** 功能描述：释放WSP 相关数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_WSP_Release(CPim* pMe)
  {
  	return HS_SML_ERR_OK;
  }

  /******************************************************************* 
  ** 函数名:HS_WSP_SendData
  ** 功能描述：发送数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_WSP_SendData(short id,char* sendcache, uint32 sendlength, void*information, CPim* pMe)
  {
  	return HS_SML_ERR_OK;
  }

  /******************************************************************* 
  ** 函数名:HS_WSP_RecData
  ** 功能描述：接收数据。
  ** 输  出: 返回操作状态
  ** 作  者:刘刚
  ** 日  期:2005-11-14
  ** 版本: ver 1.0
  *******************************************************************/
  short  HS_WSP_RecData(short id,char* receivecache, uint32 recevielength, void*information, CPim* pMe)
  {
  	return HS_SML_ERR_OK;
  }
#endif
