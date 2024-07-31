#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmiphone_export.h"

#include "mmi_filemgr.h"
#include "version.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "mmieng_internal.h"
#include "mmisms_app.h"
#include "mmi_module.h"
#include "dal_time.h"
#include "mmk_authen.h"
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "json_parser.h"
#include "mbedtls/pem.h"
#include "mbedtls/base64.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/md5.h"
#include "mbedtls/cipher.h"

#include "sfr_carrier_manager.h"
#include "sfr_json_maker.h"
#include "sfr_dm_interface.h"

typedef struct _SFR_CMCC_CONTEXT
{
    int flag;
    char *url_buff;
    char *package_buff;
} SFR_CMCC_CONTEXT;

#define CMCC_FLAG (0xc8ccf1a9)

#define CMCC_CHECK(_ctx)   (_ctx != NULL && ((SFR_CMCC_CONTEXT *)_ctx)->flag==CMCC_FLAG)

/*
{
    "sdkVersion":"1.0.51",
    "deviceId":"86740002072540840:45:da:b9:fb:33",
    "imei1":"867400020725408",
    "imei2":"867400020725416",
    "meid":"867400020725408",
    "firmwareVer":"01",
    "systemVer":"5.1",
    "iccid1":"898600910916f8054177",
    "iccid2":"898600a10913f1076473",
    "imsi1":"460007844177269",
    "imsi2":"460077101280524",
    "mac":"40:45:da:b9:fb:33",
    "cellId":"27271949",
    "lac":"6334",
    "channel":"18",
    "dataCard":"0",
    "masterStatus":"0",
    "soltQuantity":"2",
    "dataCard2":"0",
    "soltService1":"5",
    "soltService2":"5",
    "soltNetwork1":"13",
    "soltNetwork2":"0",
    "cellId2":"37283",
    "lac2":"6334",
    "volte":"1",
    "volteShow":"unknown",
    "niticeContent":"unknown",
    "volte2":"-1",
    "volteShow2":"unknown",
    "niticeContent2":"unknown",
    "sendTime":"1488163611694"
}
*/

#define CMCC_TAG_SDK    "sdkVersion"
#define CMCC_TAG_DEVICEID    "deviceId"
#define CMCC_TAG_BRAND    "brand"
#define CMCC_TAG_MODEL    "model"
#define CMCC_TAG_IMEI1    "imei1"
#define CMCC_TAG_IMEI2    "imei2"
#define CMCC_TAG_MEID    "meid"
#define CMCC_TAG_TYPE    "type"
#define CMCC_TAG_FWVER    "firmwareVer"
#define CMCC_TAG_SYSVER    "systemVer"
#define CMCC_TAG_ICCID1    "iccid1"
#define CMCC_TAG_ICCID2    "iccid2"
#define CMCC_TAG_IMSI1    "imsi1"
#define CMCC_TAG_IMSI2    "imsi2"
#define CMCC_TAG_MAC    "mac"
#define CMCC_TAG_CELLID    "cellId"
#define CMCC_TAG_LAC    "lac"
#define CMCC_TAG_CHANNEL    "channel"
#define CMCC_TAG_DATA_CARD    "dataCard"
#define CMCC_TAG_MASTER_STATUS    "masterStatus"
#define CMCC_TAG_SOLT_QUANTITY    "soltQuantity"
#define CMCC_TAG_DATA_CARD2    "dataCard2"
#define CMCC_TAG_SOLT_SERVICE1    "soltService1"
#define CMCC_TAG_SOLT_SERVICE2    "soltService2"
#define CMCC_TAG_SOLT_NETWORK1    "soltNetwork1"
#define CMCC_TAG_SOLT_NETWORK2    "soltNetwork2"
#define CMCC_TAG_CELLID2    "cellId2"
#define CMCC_TAG_LAC2    "lac2"
#define CMCC_TAG_VOLTE    "volte"
#define CMCC_TAG_VOLTE_SHOW    "volteShow"
#define CMCC_TAG_NITICE_CONTENT    "niticeContent"
#define CMCC_TAG_VOLTE2    "volte2"
#define CMCC_TAG_VOLTE_SHOW2    "volteShow2"
#define CMCC_TAG_NITICE_CONTENT2    "niticeContent2"
#define CMCC_TAG_SEND_TIME    "sendTime"

/////////////////////////////////////////////////////////////////////////////////////////////

static void *cmcc_constructor(void)
{
    SFR_CMCC_CONTEXT *_ctx = NULL;
    uint8 i_sim = 0;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i_sim)
			&& SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i_sim)
			&& MMISFR_IsChinaMobile(i_sim)
			&& MMISFR_IsICCIDChanged(i_sim))
        {
            _ctx = SCI_ALLOCAZ(sizeof(SFR_CMCC_CONTEXT));
	     if (_ctx != NULL)
	     {
	         _ctx->flag = CMCC_FLAG;
		  _ctx->package_buff = NULL;
	     }
            return (void *)_ctx;
        }
    }
    return NULL;
}

static void cmcc_saver(void *ctx)
{
    uint8 i_sim = 0;

    if (!CMCC_CHECK(ctx)) return;

    for(i_sim = 0; i_sim < MMI_DUAL_SYS_MAX; i_sim++)
    {
        MMISFR_SaveICCID(i_sim);
    }
}

static void cmcc_clean(void *ctx)
{
    if (CMCC_CHECK(ctx))
    {
        SFR_CMCC_CONTEXT *_ctx = (SFR_CMCC_CONTEXT *)ctx;
        if (_ctx->package_buff != NULL)
        {
            SCI_FREE(_ctx->package_buff);
        }
    }
}

static char *cmcc_get_url(void *ctx, int *len)
{
    if (CMCC_CHECK(ctx))
    {
        *len = -1;
        return "https://b.fxltsbl.com/accept/featureService?func=tsdk:postotherreg&appkey=A100007867";
        //return "https://b.fxltsbl.com/accept/featureService";
    }
}

static char *cmcc_get_accept(void *ctx, int *len)
{
    if (CMCC_CHECK(ctx))
    {
        *len = -1;
        return "text/javascript, */*";
    }
}

static char *cmcc_get_agent(void *ctx, int *len)
{
    if (CMCC_CHECK(ctx))
    {
        *len = -1;
        return "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)";
    }
}

static char *cmcc_get_bodytype(void *ctx, int *len)
{
    if (CMCC_CHECK(ctx))
    {
        *len = -1;
        //return "application/encrypted-json";
        return "application/json;charset=utf-8";

    }
}

static char *cmcc_encrpt(SFR_CMCC_CONTEXT *_ctx, char *src_data, int data_len, int *out_len);

static char *cmcc_get_package(void *ctx, int *len)
{
    if (CMCC_CHECK(ctx))
    {
        char *p_json;
        int p_json_len;
        SFR_CMCC_CONTEXT *_ctx = (SFR_CMCC_CONTEXT *)ctx;

        sfr_json_buff_init();
    
        sfr_add_json_char(JSON_START);

        // - SDK Version
        if (sfr_add_json_item(CMCC_TAG_SDK, "1.0"))
            sfr_add_json_char(JSON_SPLIT);
        // - Device ID
        if (sfr_add_json_item(CMCC_TAG_DEVICEID, sfr_dm_get_imei1()))
            sfr_add_json_char(JSON_SPLIT);
        // - BRAND
        if (sfr_add_json_item(CMCC_TAG_BRAND, "LT"))
            sfr_add_json_char(JSON_SPLIT);
        // - Model
        if (sfr_add_json_item(CMCC_TAG_MODEL, sfr_dm_get_model()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMEI 1
        if (sfr_add_json_item(CMCC_TAG_IMEI1, sfr_dm_get_imei1()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMEI 2
        if (sfr_add_json_item(CMCC_TAG_IMEI2, sfr_dm_get_imei2()))
            sfr_add_json_char(JSON_SPLIT);
        // - MEID
        if (sfr_add_json_item(CMCC_TAG_MEID, sfr_dm_get_meid()))
            sfr_add_json_char(JSON_SPLIT);
        // - TYPE
        if (sfr_add_json_item(CMCC_TAG_TYPE, "1"))
            sfr_add_json_char(JSON_SPLIT);
        // - Frimware Version
        if (sfr_add_json_item(CMCC_TAG_FWVER, "1.0"))
            sfr_add_json_char(JSON_SPLIT);
        // - System Version
        if (sfr_add_json_item(CMCC_TAG_SYSVER, sfr_dm_get_os_version()))
            sfr_add_json_char(JSON_SPLIT);
        // - ICCID1
        if (sfr_add_json_item(CMCC_TAG_ICCID1, sfr_dm_get_sim1_iccid()))
            sfr_add_json_char(JSON_SPLIT);
        // - ICCID2
        if (sfr_add_json_item(CMCC_TAG_ICCID2, sfr_dm_get_sim2_iccid()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMSI1 
        if (sfr_add_json_item(CMCC_TAG_IMSI1, sfr_dm_get_sim1_lte_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - IMSI2 
        if (sfr_add_json_item(CMCC_TAG_IMSI2, sfr_dm_get_sim2_lte_imsi()))
            sfr_add_json_char(JSON_SPLIT);
        // - MACID
        //if (sfr_add_json_item(CMCC_TAG_MAC, "40:45:da:b9:fb:33"))
        //    sfr_add_json_char(JSON_SPLIT);
        // - Cell ID 
        if (sfr_add_json_item(CMCC_TAG_CELLID, sfr_dm_get_sim1_cell_id()))
            sfr_add_json_char(JSON_SPLIT);
        // - LAC
        if (sfr_add_json_item(CMCC_TAG_LAC, sfr_dm_get_sim1_lac()))
            sfr_add_json_char(JSON_SPLIT);
        // - Channel
        if (sfr_add_json_item(CMCC_TAG_CHANNEL, "4G"))
            sfr_add_json_char(JSON_SPLIT);
        // - DataCard
        if (sfr_add_json_item(CMCC_TAG_DATA_CARD, sfr_dm_get_data_sim()))
            sfr_add_json_char(JSON_SPLIT);
        // - Master Status
        if (sfr_add_json_item(CMCC_TAG_MASTER_STATUS, "0"))
            sfr_add_json_char(JSON_SPLIT);
        // - Solt Quantity
        if (sfr_add_json_item(CMCC_TAG_SOLT_QUANTITY, "2"))
            sfr_add_json_char(JSON_SPLIT);
        // - DataCard2
        if (sfr_add_json_item(CMCC_TAG_DATA_CARD2, "0"))
            sfr_add_json_char(JSON_SPLIT);
        // - Solt Service1
        if (sfr_add_json_item(CMCC_TAG_SOLT_SERVICE1, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - Solt Service2
        if (sfr_add_json_item(CMCC_TAG_SOLT_SERVICE2, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - Solt NetWork1
        if (sfr_add_json_item(CMCC_TAG_SOLT_NETWORK1, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - Solt NetWork2
        if (sfr_add_json_item(CMCC_TAG_SOLT_NETWORK2, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - Cell ID 2
        if (sfr_add_json_item(CMCC_TAG_CELLID2, sfr_dm_get_sim2_cell_id()))
            sfr_add_json_char(JSON_SPLIT);
        // - LAC
        if (sfr_add_json_item(CMCC_TAG_LAC2, sfr_dm_get_sim1_lac()))
            sfr_add_json_char(JSON_SPLIT);
        // - VoLTE
        if (sfr_add_json_item(CMCC_TAG_VOLTE, sfr_dm_get_sim1_volte_sw()))
            sfr_add_json_char(JSON_SPLIT);
        // - VoLTE Show
        if (sfr_add_json_item(CMCC_TAG_VOLTE_SHOW, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - niticeContent
        if (sfr_add_json_item(CMCC_TAG_NITICE_CONTENT, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - VoLTE2
        if (sfr_add_json_item(CMCC_TAG_VOLTE2, sfr_dm_get_sim1_volte_sw()))
            sfr_add_json_char(JSON_SPLIT);
        // - VoLTE Show2
        if (sfr_add_json_item(CMCC_TAG_VOLTE_SHOW2, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - niticeContent2
        if (sfr_add_json_item(CMCC_TAG_NITICE_CONTENT2, "unknown"))
            sfr_add_json_char(JSON_SPLIT);
        // - Send Time
        if (sfr_add_json_item(CMCC_TAG_SEND_TIME, sfr_dm_get_time_total_mseconds()))
            sfr_add_json_char(JSON_SPLIT);

        sfr_remove_last_split();
        sfr_add_json_char(JSON_END);

        p_json = sfr_get_json_string();
        p_json_len = strlen(p_json);
    
        SCI_TraceLow("[Self Reg] CMCC ::  json package:: (%d)", p_json_len);
        SFR_LongTrace(p_json, p_json_len);

#if 0
    	{
            char _test_buff[128] = "aabbccdd1234";
            cmcc_encrpt(_ctx, _test_buff, 12, len);
            SCI_TraceLow("[Self Reg] entry test :: (%s) --(%s)", _test_buff, _ctx->package_buff);
            SCI_Free(_ctx->package_buff);
            _ctx->package_buff= NULL;
    	}
#endif

        cmcc_encrpt(_ctx, p_json, p_json_len, len);

        sfr_json_buff_term();


        return _ctx->package_buff;
    }
}

#define CMCC_KEY "ltkj1234ltkj9876"
#define CMCC_KEYLEN (16)

static void cmcc_md5(char *in, int in_len, char *out)
{
    mbedtls_md5_context md5_ctx = {0};

    mbedtls_md5_init( &md5_ctx );
    mbedtls_md5_starts( &md5_ctx );
    mbedtls_md5_update( &md5_ctx, in, in_len);
    mbedtls_md5_finish( &md5_ctx, out);
    mbedtls_md5_free( &md5_ctx );

    return;
}

static int cmcc_string_padding(char *data, int data_len)
{
    int i = 0;
    int padding_len = 0;

    padding_len = ((data_len % 16) == 0 ? 16 : 16 - (data_len % 16));

    for(i = 0; i < padding_len; i++)
    {
        *(data + data_len + i) = padding_len;
    }
    *(data + data_len + padding_len) = 0;

    return data_len + padding_len;
}

static void cmcc_aes_encrypt(char *key, int key_len, char *data, int data_len)
{
    mbedtls_aes_context aes_ctx = {0};
    unsigned char md5key[16] = {0};
    //char aes_iv[16] = {0};

    cmcc_md5(CMCC_KEY, CMCC_KEYLEN, md5key);

    SCI_TraceLow("[Self Reg] CMCC MD5 ::  (%s) ->->-> (%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x%.2x)", CMCC_KEY, 
		md5key[0], md5key[1], md5key[2], md5key[3], md5key[4], md5key[5], md5key[6], md5key[7],
		md5key[8], md5key[9], md5key[10], md5key[11], md5key[12], md5key[13], md5key[14], md5key[15]);

    mbedtls_aes_init( &aes_ctx );

    mbedtls_aes_setkey_enc( &aes_ctx, md5key, 128);

    //memset(&aes_iv, '0' , 16);

    //mbedtls_aes_crypt_cbc( &aes_ctx, MBEDTLS_AES_ENCRYPT, data_len, aes_iv, data, data);

    while(data_len > 0)
    {
        mbedtls_aes_crypt_ecb( &aes_ctx, MBEDTLS_AES_ENCRYPT, data, data);
        data_len -= 16;
        data += 16;
    }

    mbedtls_aes_free( &aes_ctx );

    return;
}

static char *cmcc_encrpt(SFR_CMCC_CONTEXT *_ctx, char *src_data, int data_len, int *out_len)
{
    int _base64_max = 0;
    int _base64_rel = 0;
    int _aligned_len = 0;

    _aligned_len = cmcc_string_padding(src_data, data_len);

    SCI_TraceLow("[Self Reg] CMCC padding len (%d)-- (%d)", data_len, _aligned_len);
    SFR_LongTrace(src_data, _aligned_len);

    cmcc_aes_encrypt(CMCC_KEY, CMCC_KEYLEN, src_data, _aligned_len);

    SCI_TraceLow("[Self Reg] CMCC AES Encrpt ::");
    SFR_LongTrace(src_data, _aligned_len);

    _base64_max = (_aligned_len / 3 + 2) * 4;
    _ctx->package_buff = SCI_ALLOC_APPZ(_base64_max);
    if (_ctx->package_buff != NULL)
    {
        mbedtls_base64_encode(_ctx->package_buff, _base64_max, &_base64_rel, src_data, _aligned_len);
	 *out_len = _base64_rel;
        SCI_TraceLow("[Self Reg] CMCC MD5-> AES Encrpt -> Base64::");
        SFR_LongTrace(_ctx->package_buff, _base64_rel);
    }
    return _ctx->package_buff;
}

typedef enum 
{
    JSON_TAG_NONE = 0,
    JSON_TAG_RET_CODE,		
    JSON_TAG_MAX
} SFR_JSON_TAG_E;

typedef struct
{
    SFR_JSON_TAG_E cur_tag;
    int retCode;
} SFR_JSON_PARSR_CTX;

LOCAL const char* const s_json_tags_str[] = 
{
    "",
    "resultCode",
};

LOCAL int _Json_Parser_CB(void* ctx, int type, const JSON_value* val)
{
    int res;

    SFR_JSON_PARSR_CTX *parseCtx = (SFR_JSON_PARSR_CTX *)ctx;
    switch(type) 
    {
        case JSON_T_INTEGER:
            if (parseCtx->cur_tag == JSON_TAG_RET_CODE)
            {
                parseCtx->retCode = (*val).vu.integer_value;
                SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [int ret code : %d]", parseCtx->retCode);
            }
	     res = 1;
            break;
        case JSON_T_STRING:         // 10
            if (parseCtx->cur_tag == JSON_TAG_RET_CODE)
            {
                parseCtx->retCode = atoi((((*val).vu).str).value);
                SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [str ret code : %d]", parseCtx->retCode);
            }
	     res = 1;
            break;
        case JSON_T_KEY:            // 11
        {
            SFR_JSON_TAG_E j_tag = JSON_TAG_MAX;
            while(--j_tag)
            {
                if(strcmp((((*val).vu).str).value, s_json_tags_str[j_tag]) == 0)
                {
                    parseCtx->cur_tag = j_tag;
                    SCI_TRACE_LOW("[Self Reg] _Json_Parser_CB [%s]", s_json_tags_str[j_tag]);
                    break;
                }
            }
	     res = 1;
            break;
        }
        default:
            res = -1;
            break;
    } 
    return res;
}

#define CMCC_RETURN_CODE_OK  (1)

BOOLEAN cmcc_return_parser (void *ctx, char *string, int len)
{
    //return TRUE -- self register return ok
    //return FALSE -- self register return error
    int offset;
    JSON_config js_cfg = {0};
    JSON_parser jp_ptr = PNULL;
    SFR_JSON_PARSR_CTX _ctx;

    init_JSON_config(&js_cfg);

    js_cfg.callback = _Json_Parser_CB;
    js_cfg.allow_comments = TRUE;
    js_cfg.handle_floats_manually = TRUE;
    js_cfg.callback_ctx = &_ctx;

    jp_ptr = new_JSON_parser(&js_cfg); 
    if(PNULL == jp_ptr)
    {
        SCI_TRACE_LOW("[Self Reg] ctcc_return_parser PNULL == jp_ptr");
        return;
    }

    for(offset = 0; offset < len; offset++)
    {
        int c = (*(string + offset)) & 0x000000ff;

        if(0 == JSON_parser_char(jp_ptr, c))
        {
            SCI_TRACE_LOW("[Self Reg] ctcc_return_parser parse error!");
            break;
        }
    }
    delete_JSON_parser(jp_ptr);
	
    return (CMCC_RETURN_CODE_OK ==_ctx.retCode ? TRUE : FALSE);
}

SFR_CARRIER sfr_carrier_cmcc = 
{
    "CMCC",
    NULL,
    cmcc_constructor,
    cmcc_saver,
    cmcc_clean,
    cmcc_get_url,
    cmcc_get_accept,
    cmcc_get_agent,
    cmcc_get_bodytype,
    cmcc_get_package,
    cmcc_return_parser
};
