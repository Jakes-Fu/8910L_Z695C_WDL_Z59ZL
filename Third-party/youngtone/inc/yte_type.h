#ifndef _YTE_TYPE_H_
#define _YTE_TYPE_H_

#ifdef __cplusplus
    extern   "C"
    {
#endif


#if defined(TTS_SUPPORT_YOUNGTONE)
typedef   unsigned int yte_fs_handle;

typedef struct
{
	unsigned int m_info[7];
	
} YTE_INFO_STRUCT1;

typedef struct
{
	char m_info [96];
} YTE_INFO_STRUCT2;

typedef struct
{
	char m_info [1528];
} YTE_INFO_STRUCT3;

typedef struct
{
	YTE_INFO_STRUCT1 yte_info1;
	YTE_INFO_STRUCT2 yte_info2;
	YTE_INFO_STRUCT3 yte_info3;
} YTE_INFO_STRUCT;

#endif

#ifdef   __cplusplus
    }
#endif
#endif