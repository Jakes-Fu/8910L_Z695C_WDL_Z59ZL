#ifndef _madptype_h
#define _madptype_h

#ifdef __cplusplus   
extern "C"{   
#endif // __cplusplus


#include "mcfcommon.h"


/*=================内存类型=================*/
//内存类型，即平台可以提供出来的内存获取方式，不同平台
//有所不同，如MTK可有med/asm/static方式，其他平台可能就没有
#define MCR_MEMTYPE_DYNAMIC		(1)
#define MCR_MEMTYPE_STATIC		(2)


//内存类型//确认该标识在后台没有做任何过滤条件，仅做保留。/*================内存标识================*/
#define MEMTYPE_STATIC					(1<<0)
#define MEMTYPE_ASM						(1<<1)
#define MEMTYPE_MEDIA					(1<<2)
#define MEMTYPE_NORMAL					(1<<3)
#define MEMTYPE_UNKNOWN					(0)


/*================平台标识================*/
#define PLATFORMID_MCARE_MTK6223		(0)
#define PLATFORMID_MCARE_MTK6225		(1)
#define PLATFORMID_MCARE_MTK6226		(2)
#define PLATFORMID_MCARE_MTK6228		(3)
#define PLATFORMID_MCARE_MTK6235		(4)
#define PLATFORMID_MCARE_MTK6253		(5)
#define PLATFORMID_MCARE_MTK6236		(6)
#define PLATFORMID_MCARE_MTK6268		(7)
#define PLATFORMID_MCARE_MTK6238		(8)
#define PLATFORMID_MCARE_MTK6252		(9)
#define PLATFORMID_MCARE_MTK6276		(10)


#define PLATFORMID_SPREADTRUM  			(50)
#define PLATFORMID_MCARE_SPREAD_SC6600L 			(51)
#define PLATFORMID_MCARE_SPREAD_SC6610				(56)
#define PLATFORMID_MCARE_SPREAD_SC6620				(60)
#define PLATFORMID_MCARE_SPREAD_SC6800H				(66)
#define PLATFORMID_MCARE_SPREAD_SC8800G				(70)
#define PLATFORMID_MCARE_SPREAD_SC6530				(76)



#define PLATFORMID_MSTAR         		(80)
#define PLATFORMID_BREW         		(110)


/*================选择对应的展讯平台================*/
#define SPREAD_SC6600L_1025			(6600L_1025)
#define SPREAD_SC6600L_0937			(6600L_0937)
#define SPREAD_SC6620				(6620)
#define SPREAD_SC6610				(6610)
#define SPREAD_SC6800H_1109_P5	(6800H_1109_P5)
#define SPREAD_SC6800H_1134			(6800H_1134)



/*================存储位置================*/
#define MCR_STORAGE_CARD				(0)//removealbe card
#define MCR_STORAGE_PUBLIC				(1)//public phone storage,accessable and visible
#define MCR_STORAGE_PRIVATE				(2)//private phone storage,maybe accessable but not visible
#define MCR_STORAGE_RESERVE				(3)//other


#define __CFG_NO__      (0)
#define __CFG_YES__     (1)

#define __CFG_NONE__    (0)
#define __CFG_TMC__     (1)
#define __CFG_TMZ__     (2)



#ifdef __cplusplus   
}   
#endif // __cplusplus

#endif // _madptype_h

