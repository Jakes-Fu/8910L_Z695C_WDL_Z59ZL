/*****************************************************************
 **  File Name: 	mbbms_sg.h					        *
 **  Author:		Yelianna.zhao										*
 **  Date:		23/09/2008										        *
 **  Copyright:	2001 Spreadtrum, Incorporated. All Rights Reserved.		*
 **  Description:	This file defines api for receiving service guide.		    *
*************************************************************************
*************************************************************************
**  Edit History													    *
**----------------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				            *
 **  23/09/2008		Yelianna.zhao		Create. 						*
************************************************************************/
#ifndef  MBBMS_SG_H
#define  MBBMS_SG_H



#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
#include "mbbms_sg_common.h"

#define MBBMS_SG_FILE_NAME_LEN SFS_MAX_PATH

#define MBBMS_INVALID_FILE_ID    0xFFFFFFFF


//signal type,temp
#if 0 //REmoved the temp def.
typedef enum
{
    MBBMS_SG_SG_RCV_IND,//signal to mmi
    MBBMS_SG_STOP_SEARCH_SG_SIGNAL,//stop searching SG
   
    MBBMS_SIGNAL_MAX

}MBBMS_SG_SIGNAL_E;
#endif


//file name 
typedef struct mbbms_sg_file_name_tag
{
   uint16  file_name_len;
   uint16  file_name[MBBMS_SG_FILE_NAME_LEN];
   
}MBBMS_SG_FILE_NAME_T;

typedef struct fragment_file_name_tag
{
    uint32 type;
    uint32  transport_id;
	 uint32 pos;
    uint32 len;
}MBBMS_SG_FRAGMENT_FILE_NAME_T;

#if 0
//list node
typedef struct node {
    void *data;
    uint32 node_cnt;   // valid in head node
    struct node *next;
} SG_NODE_T, *SG_LIST_PTR_T ;



/***********************************************************************/
// 	Description : This function is used to initialize the SG module.
//	Global resource dependence : 
//  Author:
//	Note:
/***********************************************************************/
void  MBBMS_SGInit(void);
/***********************************************************************/
// 	Description : This function is used to exit the SG module.
//	Global resource dependence : 
//  Author:
//	Note:
/***********************************************************************/
void  MBBMS_SGExit(void);

/***********************************************************************/
// 	Description :  search service guide
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MBBMS_SearchServiceGuide(void);
/***********************************************************************/
// 	Description :  stop searching service guide
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MBBMS_StopSearchingSG(void);

/***********************************************************************/
// 	Description : This function is used to set sg uri .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
void MBBMS_SetSGURI(char* uri_ptr);
/***********************************************************************/
// 	Description : This function is used to set sg wap .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/

void MBBMS_SGSetWap(char *wap_ptr, uint16 port);
#endif
/***********************************************************************/
// 	Description : This function is used to get SGDU file.
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
BOOLEAN MBBMS_GetSGDUFile(MBBMS_SG_FILE_NAME_T *file_name_ptr);
/***********************************************************************/
// 	Description : This function is used to get operators ad film  .
//	Global resource dependence :
//  Author:
//	Note:
/***********************************************************************/
BOOLEAN MBBMS_GetAdFilm(uint8 *data_ptr, uint32* data_len_ptr);

/***********************************************************************/
//  Description : This function is used to get SGDU BCAST file .
//  Global resource dependence :
//  Author:
//  Note:
/***********************************************************************/
BOOLEAN MBBMS_GetSGDUBCASTFile(MBBMS_SG_FILE_NAME_T *file_name_ptr);


/*****************************************************************************/
// 	Description : free memory for bsm selector
//	Global resource dependence : 
//  Author:  
//	Note:
/*****************************************************************************/ 
void MBBMS_FreeBSMSelector(void);
/*****************************************************************************/
// 	Description : get bsmselector
//	Global resource dependence : 
//  Author:  
//	Note:structure of BSMSelectorIdList is anyURI.
//           App needs to call MBBMS_FreeBSMSelector to release memory 
//          
/*****************************************************************************/ 
SG_LIST_PTR_T  MBBMS_GetBSMSelector(void);

/***********************************************************************/
//     Description :  get fragment file name
//    Global resource dependence :
//  Author:
//    Note:
/***********************************************************************/
void GetFragmentFileName(uint32   transport_id, 
                         uint32   type,
                         uint8   *full_path_ptr,     //out
                         uint16  *full_path_len_ptr  //out
                         );

/*****************************************************************************/
//  Description : 根据文件名称前缀查找XML数据文件
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC SFS_HANDLE MBBMS_GetFindHandle(SFS_FIND_DATA_T  *find_data);

/*****************************************************************************/
//  Description : 根据find_data获得完整的文件名
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MBBMS_GetFindName(SFS_FIND_DATA_T  *find_data,
                                uint8            *full_name_ptr);

/*****************************************************************************/
//  Description : 根据文件名获得对应的ID
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MBBMS_GetFileID(uint16  *fname_ptr);

/*****************************************************************************/
//  Description : 根据file id获得完整的文件名
//  Global resource dependence : none
//  Author: Bill.Ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MBBMS_GetFileNameById(uint32  file_id, uint8  *full_name_ptr);

/****************************************************************************/
//  Description: 获取SG索引文件数据
//  Global resource dependence
//  Auther:Bill.Ji
//  Note: 需调用者释放读到的数据
/****************************************************************************/
PUBLIC uint8 *MBBMS_GetIdxFileData(uint32  *idx_len);
PUBLIC BOOLEAN MBBMS_GetSGPath(uint8 *path, uint16 *sg_path_len);

#endif
