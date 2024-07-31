#ifndef __TSC_DB_TYPES_H__
#define __TSC_DB_TYPES_H__

/*  */
#define _TSC_GLOBAL_DEFAULT_STARTID     0
#define _TSC_LASTID                     0xFFFFFFFF

typedef enum tsc_db_item_status_enum
{
    /* 
     * 0 0 0 0 0 0 0 0
     *                  USED
     * 1 0 0 0 0 0 0 0  RESERVED
     * 1 1 1 1 1 1 1 0  HEAD
     * 1 1 1 1 1 1 1 1  LAST
     *
     */
    /* 1-bit */
    TSC_DIS_SINGLE = 0x00,
    TSC_DIS_ARGS = 0x01,
    
    /* 8-bit */
    TSC_DIS_USED = 0x00,
    TSC_DIS_RESERVED = 0x80,

    /* Group */
    TSC_DIS_HEAD = 0xFE,
    TSC_DIS_LAST = 0xFF
} tsc_db_item_status_enum;

/* db module type name */
#define tsc_db_mod_struct_name    "tsc_db_mod_struct"

typedef struct tsc_db_mod_struct
{
    /* start's trace id */
    unsigned int start_trace_id;

    /* module's name */
    const char *mod_name;

    /* unit block num */
    unsigned int unit_num;
} tsc_db_mod_struct;

/* db item type name */
#define tsc_db_item_struct_name    "tsc_db_item_struct"

/* db item type */
typedef struct tsc_db_item_struct
{
    /* trace id */
    unsigned int trace_id;

    /* format string */
    const char *trace_format;

    /* status */
    unsigned int /*tsc_db_item_status_enum*/ status;
} tsc_db_item_struct;

typedef struct tsc_db_info_struct
{
    /* data-base name */
    const char *db_name;
    
    /* data-base */
    const tsc_db_item_struct *db;
    unsigned int total;

    /* modules */
    const tsc_db_mod_struct *mod_list;
    unsigned int mod_num;
} tsc_db_info_struct;

#endif /* __TSC_DB_TYPES_H__ */
