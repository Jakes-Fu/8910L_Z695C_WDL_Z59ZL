/*
 *   
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */
  #ifdef __cplusplus
extern "C" {
#endif


#include "javacall_mobileidentity.h"
 /*imei imsi */
#include "mn_type.h"
#include "mn_api.h"
#include "mmicom_data.h"
#include "os_api.h"
 /**
* Get the serial number of IMEI .
* @param imei_val returned value: pointer to unsigned char buffer
*
*/
void javacall_mobile_identity_get_imei(char* imei_val){
    MN_IMEI_T imei = {0};
    char tmpStr[16];
    int i = 0;

    MNNV_GetIMEIEx(MN_DUAL_SYS_1, imei);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imei,MN_MAX_IMEI_LENGTH<<1,tmpStr);
    for(i=1;i<16;i++){
        imei_val[i-1] = *(tmpStr+i);
    }
    javacall_printf("...IMEI=%s",imei_val);
}

 /**
* Get the serial number of IMSI .
* @param imsi_val returned value: pointer to unsigned char buffer
*
*/
void javacall_mobile_identity_get_imsi(char* imsi_val){
    MN_IMSI_T   imsi = {0};

    imsi = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imsi.imsi_val,MN_MAX_IMSI_ARR_LEN<<1,imsi_val);
    javacall_printf("...IMSI=%s",imsi_val);
}
#ifdef __cplusplus
}
#endif