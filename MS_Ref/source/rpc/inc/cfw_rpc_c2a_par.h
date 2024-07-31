// AUTO GENERATED

#define TAG_CFW_GetIMEIFromAP 0x62e05df9
struct IPAR_CFW_GetIMEIFromAP
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetIMEIFromAP));

struct OPAR_CFW_GetIMEIFromAP
{
    uint8_t pImei[16];
    uint8_t pImeiLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetIMEIFromAP));


// AUTO GENERATED END
