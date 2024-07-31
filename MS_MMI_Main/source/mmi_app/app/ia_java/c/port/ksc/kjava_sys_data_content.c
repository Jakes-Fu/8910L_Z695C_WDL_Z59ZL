/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
#include <stdlib.h>

#include <kjava_sys_data.h>

#if defined(__BIG_ENDIAN) || defined(__BigEndian)
#include <kjava_sys_data_content.h>
#else
#include <kjava_sys_data_content32.h>
#endif

#define NUM_ROM_ENTRY 17

KJavaROMData g_rom_entries[NUM_ROM_ENTRY] = 
{
	// name, arrayName, arraySize, parent, child, sibling
	//  0
	{"", (char*)NULL, 0, -1, 1, -1, 1},
	//  1
	{"0.adi", (char*)_0_adi, __0_adi_size, 0, -1, 2, 0},
	//  2
	{"0.cer", (char*)_0_cer, __0_cer_size, 0, -1, 3, 0},
	//  3
	{"0.dat", (char*)_0_dat, __0_dat_size, 0, -1, 4, 0},
	//  4
	{"0.dms", (char*)_0_dms, __0_dms_size, 0, -1, 5, 0},
	//  5
	{"1.adi", (char*)_1_adi, __1_adi_size, 0, -1, 6, 0},
	//  6
	{"1.cer", (char*)_1_cer, __1_cer_size, 0, -1, 7, 0},
	//  7
	{"1.dat", (char*)_1_dat, __1_dat_size, 0, -1, 8, 0},
	//  8
	{"1.dms", (char*)_1_dms, __1_dms_size, 0, -1, 9, 0},
	//  9
	{"2.adi", (char*)_2_adi, __2_adi_size, 0, -1, 10, 0},
	// 10
	{"2.cer", (char*)_2_cer, __2_cer_size, 0, -1, 11, 0},
	// 11
	{"2.dms", (char*)_2_dms, __2_dms_size, 0, -1, 12, 0},
	// 12
	{"3.adi", (char*)_3_adi, __3_adi_size, 0, -1, 13, 0},
	// 13
	{"3.cer", (char*)_3_cer, __3_cer_size, 0, -1, 14, 0},
	// 14
	{"3.dms", (char*)_3_dms, __3_dms_size, 0, -1, 15, 0},
	// 15
	{"4.cer", (char*)_4_cer, __4_cer_size, 0, -1, 16, 0},
	{"suite.idx", (char*)_suite_idx, __suite_idx_size, 0, -1, -1, 0}
};
