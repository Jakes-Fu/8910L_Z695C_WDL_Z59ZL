/******************************************************************************
 *
 ** File Name:      sprd_codec_ap_phy_sc2720_headset.h                                *
 ** Author:         yuzhen.long                                                   *
 ** DATE:           15/04/2021                                                 *
 ** Copyright:      2021 unisoc, Incoporated. All Rights Reserved.            *
 ** Description:    codec ap control  for sc2720 headset                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                DESCRIPTION                               *
 ** 15/04/2021        yuzhen.long            Create.                                 *
 *****************************************************************************/
#include "chip_plf_export.h"
#ifndef __SPRD_CODEC_AP_PHY_SC2720_HEADSET_H___
#define __SPRD_CODEC_AP_PHY_SC2720_HEADSET_H___

#if 1
typedef struct {
	uint32 A;
	uint32 B;
	uint32 E1;
	uint32 E2;
	uint32 cal_type;
} SC2720_HEADSET_AUXADC_CAL_T;

typedef enum {
    UNKNOWN_MIC_TYPE,
    HEADSET_NO_MIC,
    HEADSET_4POLE_NORMAL,
    HEADSET_4POLE_NOT_NORMAL,
    HEADSET_APPLE,
    HEADSET_TYPE_ERR = -1,
}SC2720_HEADSET_TYPE_T;

typedef struct HeadPhoneSt{
    uint32 Connected;
    SC2720_HEADSET_TYPE_T MicType;
}T_HEADPHONE_STATUS;

typedef struct{
	uint32 adc_min;
	uint32 adc_max;
	uint32 code;
}SC2720_HEADSET_BUTTONS_T;
typedef struct
{
        uint32	headset_stable_max_value;	// 2.65v
        uint32	headset_one_half_adc_gnd;//0.15V
        uint32	headset_adc_threshold_3pole_detect;//0.9V
        uint32	headset_half_adc_gnd;//0.05V
        uint32	headset_adc_gnd;// 0.1v
        uint32	headset_key_send_min_vol;
        uint32	headset_key_send_max_vol;	
        uint32	headset_key_volup_min_vol;	
        uint32	headset_key_volup_max_vol;	
        uint32	headset_key_voldown_min_vol;	
        uint32	headset_key_voldown_max_vol;	
        uint32   coefficient;   
}HEADSET_INFO_T,*HEADSET_INFO_T_PTR;
typedef struct {
	int				id;

	int				modes;
	int				currentmode;
	#if 0
	struct device			dev;

	struct iio_event_interface	*event_interface;

	struct iio_buffer		*buffer;
	struct list_head		buffer_list;
	int				scan_bytes;
	struct mutex			mlock;

	const unsigned long		*available_scan_masks;
	unsigned			masklength;
	const unsigned long		*active_scan_mask;
	bool				scan_timestamp;
	unsigned			scan_index_timestamp;
	struct iio_trigger		*trig;
	struct iio_poll_func		*pollfunc;
	struct iio_poll_func		*pollfunc_event;

	struct iio_chan_spec const	*channels;
	int				num_channels;

	struct list_head		channel_attr_list;
	struct attribute_group		chan_attr_group;
	const char			*name;
	const struct iio_info		*info;
	struct mutex			info_exist_lock;
	const struct iio_buffer_setup_ops	*setup_ops;
	struct cdev			chrdev;
#define IIO_MAX_GROUPS 6
	const struct attribute_group	*groups[IIO_MAX_GROUPS + 1];

	int				groupcounter;

	unsigned long			flags;
#if defined(CONFIG_DEBUG_FS)
	struct dentry			*debugfs_dentry;
	unsigned			cached_reg_addr;
#endif
#endif
}SC2720_II0_DEV_T ;//TODO

typedef struct {
	int			channel;
	int			channel2;
	unsigned long		address;
	int			scan_index;

}SC2720_IIO_CHAN_SPEC_T ;//TODO



typedef struct {
	SC2720_II0_DEV_T *indio_dev;
	const SC2720_IIO_CHAN_SPEC_T *channel;
	void *data;
}SC2720_IIO_CHANNEL_T;

enum {
	HDST_GPIO_DET_L = 0,
	HDST_GPIO_DET_H,
	HDST_GPIO_DET_MIC,
	HDST_GPIO_DET_ALL,
	HDST_GPIO_BUTTON,
	HDST_GPIO_MAX
};

typedef struct {
	uint32 gpio_switch;
	uint32 jack_type;
	uint32 gpios[HDST_GPIO_MAX];
	uint32 dbnc_times[HDST_GPIO_MAX]; /* debounce times */
	uint32 irq_trigger_levels[HDST_GPIO_MAX];
	uint32 adc_threshold_3pole_detect;
	uint32 irq_threshold_button;
	uint32 voltage_headmicbias;
	uint32 sprd_adc_gnd;
	uint32 sprd_half_adc_gnd;
	uint32 sprd_one_half_adc_gnd;
	uint32 sprd_stable_value;
	uint32 coefficient;
	SC2720_HEADSET_BUTTONS_T *headset_buttons;
	uint32 nbuttons;
	uint8 do_fm_mute;
}SC2720_HEADSET_PLATFORM_DATA_T;

typedef struct {
	int headphone;
	int irq_detect;
	int irq_button;
	int irq_detect_l;
	int irq_detect_h;
	int irq_detect_mic;
	int irq_detect_all;
#if 0
	struct platform_device *pdev;
	//struct sprd_headset_platform_data pdata;
	struct delayed_work det_work;
	struct workqueue_struct *det_work_q;
	struct delayed_work det_all_work;
	struct workqueue_struct *det_all_work_q;
	struct work_struct btn_work;
	struct workqueue_struct *btn_work_q;
	struct snd_soc_codec *codec;
	struct sprd_headset_power power;
	struct semaphore sem;
	struct snd_soc_jack hdst_jack;
	struct snd_soc_jack btn_jack;
	enum snd_jack_types hdst_status;
	enum snd_jack_types btns_pressed;
	//struct iio_channel *adc_chan;
	struct mutex irq_btn_lock;
	struct mutex irq_det_lock;
	struct mutex irq_det_all_lock;
	struct mutex irq_det_mic_lock;
	struct delayed_work reg_dump_work;
	struct workqueue_struct *reg_dump_work_q;
	struct work_struct fc_work; /* for fast charge */
#ifdef ADPGAR_BYP_SELECT
	/* used for adpgar bypass selecting. */
	struct delayed_work adpgar_work;
	struct workqueue_struct *adpgar_work_q;
#endif
#endif
	int debug_level;
	int det_err_cnt; /* detecting error count */
	int gpio_det_val_last; /* detecting gpio last value */
	int gpio_btn_val_last; /* button detecting gpio last value */

	int btn_stat_last; /* 0==released, 1==pressed */
	/* if the hardware detected a headset is
	 * plugged in, set plug_state_last = 1
	 */
	int plug_stat_last;
	int report;

	//struct wake_lock det_wakelock;
	//struct wake_lock det_all_wakelock;
	//struct wake_lock btn_wakelock;
}SC2720_HEADSET_T;
#endif

#endif //__SPRD_CODEC_DP_PHY_V0_H___
