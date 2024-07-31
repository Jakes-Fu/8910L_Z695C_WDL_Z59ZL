#ifndef _MTA_TRACESCAN_H
#define _MTA_TRACESCAN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "sci_types.h"

#ifdef WCDMA_AS_PRESENT  
#define TSC_NOT_USED
#endif


#define SDI_PARAMS(p_num, p) \
	SDI_PARAMS_##p_num p
	
#define SDI_PARAMS_0()

#define SDI_PARAMS_1(p1) \
	, p1
#define SDI_PARAMS_2(p1, p2) \
	, p1, p2
#define SDI_PARAMS_3(p1, p2, p3) \
	, p1, p2, p3
#define SDI_PARAMS_4(p1, p2, p3, p4) \
	, p1, p2, p3, p4
#define SDI_PARAMS_5(p1, p2, p3, p4, p5) \
	, p1, p2, p3, p4, p5
#define SDI_PARAMS_6(p1, p2, p3, p4, p5, p6) \
	, p1, p2, p3, p4, p5, p6
#define SDI_PARAMS_7(p1, p2, p3, p4, p5, p6, p7) \
	, p1, p2, p3, p4, p5, p6, p7
#define SDI_PARAMS_8(p1, p2, p3, p4, p5, p6, p7, p8) \
	, p1, p2, p3, p4, p5, p6, p7, p8
#define SDI_PARAMS_9(p1, p2, p3, p4, p5, p6, p7, p8, p9) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9
#define SDI_PARAMS_10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10	
#define SDI_PARAMS_11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11
#define SDI_PARAMS_12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12
#define SDI_PARAMS_13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13
#define SDI_PARAMS_14(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14
#define SDI_PARAMS_15(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15
#define SDI_PARAMS_16(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16
#define SDI_PARAMS_17(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17
#define SDI_PARAMS_18(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18
#define SDI_PARAMS_19(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19
#define SDI_PARAMS_20(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16	, p17, p18, p19, p20
#define SDI_PARAMS_21(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21
#define SDI_PARAMS_22(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22
#define SDI_PARAMS_23(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23
#define SDI_PARAMS_24(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24
#define SDI_PARAMS_25(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25
#define SDI_PARAMS_26(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26	
#define SDI_PARAMS_27(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27
#define SDI_PARAMS_28(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28) \
	, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28	


#define SDI_TRACE(_mod, x_format) \
	SCI_TRACE_LOW(x_format)

#define SDI_VTRACE(_mod, x_format, _param_num, _param) \
	SCI_TRACE_LOW(x_format SDI_PARAMS(_param_num, _param))


#if defined(TRACE_INFO_SUPPORT)

#if defined(TSC_NOT_USED) || defined(WCDMA_AS_PRESENT)
#define SDI_VTRACEID(_mod, _id, arg_fmt, arg_fmt2, x_format, _param_num, _param) \
	SCI_TRACE_LOW(x_format SDI_PARAMS(_param_num, _param))

#define SDI_TRACEID(_mod, _id, x_format) \
	SCI_TRACE_LOW(x_format)

#else
#define SDI_VTRACEID(_mod, _id, arg_fmt, arg_fmt2, x_format, _param_num, _param) \
	mta_traceid_vdata(TSC_MOD_##_mod##_##_id, arg_fmt2 SDI_PARAMS(_param_num, _param))

#define SDI_TRACEID(_mod, _id, x_format) \
	mta_traceid_data(TSC_MOD_##_mod##_##_id)
	
extern int mta_traceid_data( uint32 traceid);
extern int mta_traceid_vdata( uint32 traceid, const int8 *fmt, ...);
#endif

#else

#define SDI_VTRACEID(_mod, _id, arg_fmt, arg_fmt2, x_format, _param_num, _param) 

#define SDI_TRACEID(_mod, _id, x_format) 

#endif


#ifdef __cplusplus
}
#endif


#endif 
