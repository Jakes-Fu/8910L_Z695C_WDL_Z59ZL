/*
 * md_ipc.h - A header file of modem of RDA
 *
 * Copyright (C) 2017 RDA Microelectronics Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef _RDA_MD_IPC_H_
#define _RDA_MD_IPC_H_

#include "sci_types.h"
#include "globals.h"
#include "uix8910_ipc_cfg.h"
#include "./uix8910/mailbox_reg_uix8910.h"
#include "uix8910_reg_base.h"

#ifndef BIT
#define BIT(x) (1 << (x))
#endif


/*
 * SMD Channel
 */
#define SMD_CH_STREAM_TYPE   1
#define SMD_CH_PACKET_TYPE   2
#define SMD_CH_QUEUE_TYPE    3
#define SMD_CH_SYSCMD_TYPE   4

#define SMD_CH_AT            1
#define SMD_CH_AT1           1
#define SMD_CH_AT2           2
#define SMD_CH_PS            3
#define SMD_CH_A2C_CTRL      4
#define SMD_CH_AUD_CTRL      5

#define SMD_CH_MAX           (SMD_CH_AUD_CTRL + 1)

/*
 * Channel Event
 */
#define CH_READ_AVAIL        0x1
#define CH_WRITE_AVAIL       0x2
#define CH_OPENED            0x4
#define CH_RW_MASK           (CH_READ_AVAIL | CH_WRITE_AVAIL)

/*
 * PS Channel Info
 */
#define PS_STAT_UL_BUSY      0x1
#define PS_STAT_UL_IDLE      0x2

#define PS_BUF_FULL          BIT(0) // no free buf now
#define PS_UL_BUSY           BIT(1) // cp set ul busy flag

/*
 * Share Memory Layout
 */
//#define PS_BUF_LEN_B         1600
//#define PS_BUF_LEN_L         300
//#define PS_BUF_RESERVED_LEN  100  /* only for ul */
//#define PS_LIT_BUF_MAX_LEN   100

/* PS Packet Header define */
#define PS_HEADER_SIZE       20
#define PS_UL_ROHC_LEN       18
#define PS_UL_PDCP_LEN       2
#define PS_RNDIS_H_LEN       58
#define PS_DL_ROHC_LEN       128
#define PS_UL_RESERVED       0
#define PS_DL_RESERVED       4 // ps_header->id for dl debug

#define PS_UL_HDR_LEN        \
	(PS_HEADER_SIZE+PS_UL_ROHC_LEN+PS_UL_PDCP_LEN+PS_UL_RESERVED)
#define PS_DL_HDR_LEN_B      \
	(PS_HEADER_SIZE+PS_RNDIS_H_LEN+PS_DL_RESERVED)
#define PS_DL_HDR_LEN_L      \
	(PS_HEADER_SIZE+PS_RNDIS_H_LEN+PS_DL_ROHC_LEN+PS_DL_RESERVED)

/* PS Packet Data define */
#define PS_DATA_LEN_B        1500
#define PS_DATA_LEN_L        100

/* PS Packet For AP-CP */
#define PS_BUF_UL_LEN_B      (PS_UL_HDR_LEN+PS_DATA_LEN_B+28) 	    // 1540->1568
#define PS_BUF_UL_LEN_L      (PS_UL_HDR_LEN+PS_DATA_LEN_L+20) 	    // 140->160
#define PS_BUF_DL_LEN_B      (PS_DL_HDR_LEN_B+PS_DATA_LEN_B+18)     // 1582->1600 
#define PS_BUF_DL_LEN_L	     (PS_DL_HDR_LEN_L+PS_DATA_LEN_L+10)     // 310->320

#define IPC_CMD_SIZE         16
#define IPC_CMD_QUEUE_LEN    8
#define MD_VERSION_SIZE      40
#define MD_EXEC_CAUSE_SIZE   256

#define AT1_BUF_SIZE         1024
#define AT2_BUF_SIZE         1024
#define PS_CTRL_BUF_SIZE     2048
#define A2C_CTRL_BUF_SIZE    (IPC_CMD_QUEUE_LEN * IPC_CMD_SIZE)
#define AUD_CTRL_BUF_SIZE    512

/* Channel ctrl info */
#define AT1_UL_CTRL_OFFSET   0x0
#define AT1_DL_CTRL_OFFSET   0x2C
#define AT2_UL_CTRL_OFFSET   0x58
#define AT2_DL_CTRL_OFFSET   0x84
#define PS_UL_CTRL_OFFSET    0xB0
#define PS_DL_CTRL_OFFSET    0xDC
#define A2C_UL_CTRL_OFFSET   0x108
#define A2C_DL_CTRL_OFFSET   0x134
#define AUD_UL_CTRL_OFFSET   0x160
#define AUD_DL_CTRL_OFFSET   0x18C

/* Channel data buf */
#define AT1_UL_DATA_OFFSET   0x800
#define AT1_DL_DATA_OFFSET   (AT1_UL_DATA_OFFSET + AT1_BUF_SIZE)
#define AT2_UL_DATA_OFFSET   (AT1_DL_DATA_OFFSET + AT1_BUF_SIZE)
#define AT2_DL_DATA_OFFSET   (AT2_UL_DATA_OFFSET + AT2_BUF_SIZE)
#define A2C_UL_DATA_OFFSET   (AT2_DL_DATA_OFFSET + AT2_BUF_SIZE)
#define A2C_DL_DATA_OFFSET   (A2C_UL_DATA_OFFSET + A2C_CTRL_BUF_SIZE)
#define AUD_UL_DATA_OFFSET   (A2C_DL_DATA_OFFSET + A2C_CTRL_BUF_SIZE)
#define AUD_DL_DATA_OFFSET   (AUD_UL_DATA_OFFSET + AUD_CTRL_BUF_SIZE)
#define PS_UL_CTRL_DATA_OFFSET  (AUD_DL_DATA_OFFSET + AUD_CTRL_BUF_SIZE + 128)
#define PS_DL_CTRL_DATA_OFFSET  (PS_UL_CTRL_DATA_OFFSET + PS_CTRL_BUF_SIZE)

/*
 * IRQ Bit
 */
#define AT1_READY_BIT        BIT(0)
#define AT2_READY_BIT        BIT(1)
#define PS_READY_BIT         BIT(2)
#define A2C_CTRL_READY_BIT   BIT(3)
#define AUD_CTRL_READY_BIT   BIT(4)
#define MD_EXECPTION_BIT     BIT(5)
#define MD_IP_ADDR_ADD_BIT   BIT(6)
#define MD_IP_ADDR_DEL_BIT   BIT(7)
#define MD_TRACE_CTRL_BIT    BIT(8)
#define ZSP_TRACE_CTRL_BIT   BIT(9)
#define MD_SYSCORE_RESUME    BIT(10)

#define ZSP_MUSIC_NOTIFY   BIT(11)
#define ZSP_SBC_NOTIFY       BIT(12)

#define RISC_BT_LOG_NOTIFY   BIT(13)
#define RISC_BT_ASSERT_NOTIFY    BIT(14)
#define RISC_BT_NOTIFY_AP_WAKEUP BIT(15)
#define RISC_BT_NOTIFY_AP_SLEEP  BIT(16)

#define CP_NOTIFY_AP_STORE_RFPARA  BIT(17)

#define AP_NOTIFY_CP_EXEC_BIT    BIT(7)
#define AP_NOTIFY_CP_STOPLOG_BIT BIT(9)
#define AP_NOTIFY_ZSP_EXEC_BIT   BIT(2)

#define IRQ_MASK         (AT1_READY_BIT | AT2_READY_BIT | PS_READY_BIT | A2C_CTRL_READY_BIT | AUD_CTRL_READY_BIT)

/*
 * CTRL CMD ID
 */
#define C2A_DYNAMIC_NVM_UPDATE  0x1
#define C2A_STATIC_NVM_UPDATE   0x2
#define C2A_PSM_RTC_TIME        0x3
#define C2A_CSW_DATA_UPDATE     0x4
#define C2A_RF_CALIB_UPDATE     0x5
#define C2A_AUD_CALIB_UPDATE    0x6
#define C2A_PHY_DUMP_DATA_SAVE  0x7

#define AP_NOTIFY_CP_ASSERT     0x1
#define AP_NOTIFY_ZSP_ASSERT    0x2

#define MD_SYS_MAGIC    0xA8B1

#define BUF_IN_USE      (0xAAAAAAAA)
#define BUF_IN_IDLE     (0x55555555)

#define IP_ADDR_ONLY_IPV4	1
#define IP_ADDR_IPV4V6		2
#define IP_ADDR_ONLY_IPV6	3

#define  SYSMAIL_SAVE_REG_NUM     (1)

struct ps_header {
	uint32 next;
	uint8 cid;
	uint8 simid;
	uint16 len;
	uint32 flag;      /* BUF_IN_USR or BUF_IN_IDLE */
	uint32 buf_size;  /* 300B or 1600B */
	uint32 data_off;  /* data off in buffer */
	uint32 id;
};

struct ipc_cmd {
	uint32 id;
	uint32 para0;
	uint32 para1;
	uint32 para2;
};

struct md_sys_hdr {
	unsigned short magic;
	unsigned short mod_id;
	unsigned int req_id;
	unsigned int ret_val;
	unsigned short msg_id;
	unsigned short ext_len;
};

struct md_sys_hdr_ext {
	unsigned short magic;
	unsigned short mod_id;
	unsigned int req_id;
	unsigned int ret_val;
	unsigned short msg_id;
	unsigned short ext_len;
	unsigned char ext_data[];
};

struct ch_buf_ctrl {
	uint32 head;
	uint32 tail;
	uint32 status;
	uint32 alloc_net;
	uint32 free_net;
	uint32 alloc_vol;
	uint32 free_vol;
	uint32 alloc_net_l;
	uint32 free_net_l;
	uint32 alloc_vol_l;
	uint32 free_vol_l;
};

struct ch_buf {
	struct ch_buf_ctrl *pctrl;
	uint8 *pfifo;
	uint32 fifo_size;
	uint32 fifo_mask;
	uint32 queue_len;
	uint32 *net_free_buf_b;
	uint32 *net_free_buf_l;
	uint32 *vol_free_buf_b;
	uint32 *vol_free_buf_l;

	//spinlock_t lock;
	SCI_SEMAPHORE_PTR ch_lock;
	void *ch;
};

struct smd_ch {
	uint32 id;
	uint32 type;
	uint32 opened;

	struct ch_buf ul;	/* Send to Net */
	struct ch_buf dl;	/* Recv from Net */

	//spinlock_t notify_lock;
	void (*notify)(void *priv, uint32 flags);

	int32(*read)(struct ch_buf *buf, void *data, uint32 len);
	int32(*write)(struct ch_buf *buf, const void *data, uint32 len);
	int32(*read_avail)(struct ch_buf *buf);
	int32(*write_avail)(struct ch_buf *buf);
	int32(*alloc_ul_psbuf)(struct ch_buf *buf, uint32 size);
	int32(*free_dl_psbuf)(struct ch_buf *buf, uint32 offset);

	uint32 event_mask;
	void (*notify_other_cpu)(void);

	void *priv;
	void *pmd;
};

struct ip_info {
	uint32 ip_type;
	uint32 ipv4_addr;
	uint32 dnsv4_primary;
	uint32 dnsv4_auxiliary;
	uint8 ipv6_intf_id[8];
	uint8 dnsv6_primary[16];
	uint8 dnsv6_auxiliary[16];
};

struct track_info {
    uint32 widx;
    uint32 mailbox[256];
    uint32 time[256];
};

struct md_dev {
	struct smd_ch chs[SMD_CH_MAX];

	HWP_CP_MAILBOX_T *regs;
	uint8 *sm_base;
	uint32 sm_len;

	//struct work_struct notify_work;
	//spinlock_t irq_lock;
	int irq;
	uint32 ul_big_ps_buf_cnt;
	uint32 ul_lit_ps_buf_cnt;
	uint32 dl_big_ps_buf_cnt;
	uint32 dl_lit_ps_buf_cnt;
	uint32 md_version_off;
	uint32 md_exec_cause_off;

	uint32 ip_type;
	uint32 md_ip_addr_status;
	uint32 md_ip_addr;
	uint32 md_dns_addr;

	uint32 ul_big_start;
	uint32 ul_big_end;
	uint32 ul_lit_start;
	uint32 ul_lit_end;
	uint32 dl_big_start;
	uint32 dl_big_end;
	uint32 dl_lit_start;
	uint32 dl_lit_end;
	uint32 sbc_buf_off;
	uint32 delta_nv_off;
	uint32 ims_nv_info_off;
    uint32 bt_rf_sm_off;

	uint32 md_ipv6_addr_status;
	uint32 md_ipv6_addr_l[2];
	uint32 md_dnsv6_addr[4];

	void (*exec_handler)(void);	/* Modem execption handler */
	void (*trace_handler)(uint32 handler); /* CP Trace handler */
	void (*music_handler)(void);

	void *pdata;
};

enum ip_addr_status {
	IP_ADDR_NOCHANGE = 0,
	IP_ADDR_CHANGED = 1,
	IP_ADDR_READY = 2,
	IP_ADDR_DELETE = 3,
};

struct HAL_IP_SUSPEND_RESUME_T            
{
    char module_name[10];
    void (*reg_suspend)();                       //func addr of reg save
    void (*reg_resume)();                        //func addr of reg resume
    struct HAL_IP_SUSPEND_RESUME_T *next;        //next node addr
};

extern uint8 *rda_sm_base;
extern uint8 *rda_sm_phys;
extern struct smd_ch *g_ps_ch;

extern int32 ipc_ch_open(int32 ch_id, struct smd_ch **ch, void *priv,
			void (*notify)(void *, uint32));
extern int32 ipc_ch_read(struct smd_ch *ch, void *data, uint32 len);
extern int32 ipc_ch_write(struct smd_ch *ch, const void *data, uint32 len);
extern int32 ipc_ch_close(struct smd_ch *ch);
extern int32 ipc_ch_read_avail(struct smd_ch *ch);
extern int32 ipc_ch_write_avail(struct smd_ch *ch);

extern BOOLEAN ipc_ps_buf_full(struct smd_ch *ch);
extern BOOLEAN ipc_ps_ul_idle(struct smd_ch *ch);
extern void ipc_reset_ps_buf(struct smd_ch *ch);
extern void ipc_set_ps_ul_stat(struct smd_ch *ch, uint32 stat);
extern int32 ipc_alloc_ul_ps_buf(struct smd_ch *ch, uint32 size);
extern int32 ipc_free_dl_ps_buf(struct smd_ch *ch, uint32 offset);

extern void ipc_disable_ch_irq(struct smd_ch *ch);
extern void ipc_enable_ch_irq(struct smd_ch *ch);
extern void ipc_enable_ravail_event(struct smd_ch *ch);
extern void ipc_disable_ravail_event(struct smd_ch *ch);
extern void ipc_enable_wavail_event(struct smd_ch *ch);
extern void ipc_disable_wavail_event(struct smd_ch *ch);
extern void ipc_register_exec_handler(void (*handler)(void));
extern uint32 ipc_get_ch_status(struct smd_ch *ch);
extern uint32 ipc_get_cp_exec_type(void);
extern uint32 ipc_get_zsp_exec_type(void);
extern void ipc_ap_execption_notify(void);
extern void ipc_register_trace_handler(void (*handler) (uint32));

extern uint32 get_dl_free_buf_cnt(struct smd_ch *ch);
extern BOOLEAN ipc_ps_ul_opened(void);
extern BOOLEAN ipc_ps_ul_free(uint32 size);
extern void pmic_set_soft_reset(void);
extern void ipc_get_ip_dns_addr(struct ip_info *info);
extern void ipc_set_sbc_buf_addr(uint32 addr);
extern uint32 ipc_get_modem_version(void);
extern void ipc_notify_cp_stop_log(uint32 is_off);

typedef void (*pfunc_void)(void);

#endif
