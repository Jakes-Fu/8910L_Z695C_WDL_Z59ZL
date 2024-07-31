/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
 
/*****************************************************************************
** File Name:      recordclient.c                                        *
** Description:    This file is used to implement record function             *
******************************************************************************/

/*************************************************************************
 * Include Header Files
 *************************************************************************/
#include "jblend_config.h"

#ifdef JAVA_SUPPORT_MEDIA_RECORD  

#include "jkmediacontrol.h"
#include "sci_types.h"
#include "jkmediacapture.h"
#include "jkmediaoption.h"
#include "ajms_arc.h"
#include "audioclient.h"


/*************************************************************************
 * Global Definition and Declaration
 *************************************************************************/

BOOLEAN jas_audio_recorder_open( int32 java_recorder){return SCI_TRUE;}
BOOLEAN jas_audio_recorder_close(int32 java_recorder){return SCI_TRUE;}
BOOLEAN jas_audio_recorder_start_record(int32 java_recorder){return SCI_FALSE;}
BOOLEAN jas_audio_recorder_pause_record(int32 java_recorder){return SCI_FALSE;}
BOOLEAN jas_audio_recorder_commit(int32 java_recorder){return SCI_FALSE;}
BOOLEAN jas_audio_recorder_reset(int32 java_recorder){return SCI_FALSE;}
BOOLEAN jas_audio_recorder_set_record_location(int32 java_recorder, wchar *path){return SCI_TRUE;}
BOOLEAN jas_audio_recorder_set_record_size_limit(int32 java_recorder, int32 limit_size){return 0;}
long jas_audio_recorder_get_record_time(int32 java_recorder){return 0;}
#endif
