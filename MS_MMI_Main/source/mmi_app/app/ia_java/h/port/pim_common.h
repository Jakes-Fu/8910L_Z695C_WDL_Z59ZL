// -- tag_jbom2_20071026_RC
#ifndef ___DEVPIMCOMMON_H
#define ___DEVPIMCOMMON_H

#include "sci_types.h"
#include "mmipb_export.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*Constant Definition*/
#define     PIM_LISTNAME_LEN                64
#define     PIM_LABEL_LEN                   64
#define     PIM_CATEGORY_LEN                   (MMIPB_NAME_MAX_LEN -1)
#define 	  PIM_FIELD_LEN					(MMIPB_NAME_MAX_LEN << 1)
//#define 	PIM_MAX_CATEGORY_NUM			MMIPB_MAX_GROUP_NUM + 1
#define	PIM_INVALID_MAX_ENTRY_ID	    0xffff

#define LISTTYPE(connectionId)      ((connectionId >> 8) & 0x000000ff)
#define LISTID(connectionId)        ((connectionId >> 16) & 0x000000ff)
#define LISTMODE(connectionId)      ((connectionId ) & 0x000000ff)
/*
 * PIM List Structure Information
 */
typedef struct {
    JKSint              listID;             /* This is Spreadtrum's ID */
    JKPIMListType       listType;
    JKWChar             listName[PIM_LISTNAME_LEN+1];
    JKWChar*            tableName;
    JKSint              maxCategoryForList;
    JKSint              maxCategoryForItem;
    JKSint              accessibility;
    JKSint              size;
} PimListInfo;

/*
 *ArraList definition structure
 */
typedef struct {
    JKSint              index;
    JKUint8*            label;
} PimStringArrayInfo;

/*Field Structure Definition information
 */
typedef struct {
    JKSint              listID;             /* This is Spreadtrum's ID */
    JKSint              fieldID;
    JKSint              fieldLabelID;       /* This is Spreadtrum's filed ID */
    JKPIMDataType       fieldDataType;
    PimStringArrayInfo* stringArrayInfo;
    JKSint              arraySize;
    JKSint              maxValues;
    JKUint32            attributes;
} PimFieldInfo;


/*
 *Attribute Structure Definition information
 */
typedef struct {
    JKSint              listID;
    JKSint              attributeID;
    JKSint              attributeLabelID;   /* This is Spreadtrum's ID */
} PimAttributeInfo;

enum {
    PIM_SEARCH_TYPE_ALL = 0,
    PIM_SEARCH_TYPE_CATEGORY,
    PIM_SEARCH_TYPE_ITEM,
    PIM_SEARCH_TYPE_STRING,
    //PIM_SEARCH_TYPE_EVENT,
    PIM_SEARCH_TYPE_MAX
};
enum {
    PIM_CONTACT_LIST_SIM = 0,
    PIM_CONTACT_LIST_PHONE,
    PIM_CONTACT_LIST_MAX
};

typedef struct {
    JKSint              index;
    JKUint16            size;
    JKSint32            connectionId;
    JKSint32            searchType;     /* byAll, byCategory, byItem, byString */
    union {
        JKSint32        groupId;        /* byCategory */
        JKPIMItem      *item;           /* byItem */
        JKPIMString    *string;         /* byString */
//        PIM_EVENT_SEARCH *event;

        
    } searchData;
} PimResultSetInfo;

void dumpbuf(void *buf, int len);
JKSint getListIdByName(JKWChar *name, JKSint32 length);
JKSint32 PIM_GetUcs2Len( const JKWChar *wcs );
uint16 PIM_MakeIntoUcs2(
						uint8 *ucsPtr,//ucs2 big endian string pointer
						uint16 MaxUcs2Len,//ucs2 big endian string buffer len int byte
						BOOLEAN isUcs2,
						const uint8 *utf8Ptr,//utf8 string pongiter
						uint16 utf8Len//utf8 string len int byte
					   );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ___DEVPIMCOMMON_H */

