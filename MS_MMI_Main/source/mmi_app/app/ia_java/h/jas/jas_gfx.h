/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
//#include "std_header.h"
//#include "os_api.h"
//#include "sci_types.h"


#ifndef JAS_GFX_H
#define JAS_GFX_H

#include "caf.h"

#include "guilcd.h"
#include "chip.h"
#include "mmi_default.h"
#include "guiimg.h"
#include "jblend_platform.h"
#include "java_mmi_image.h"
#include "guistring.h"


#define JVM_PIXEL_FORMAT 565

#if (JVM_PIXEL_FORMAT == 565)
typedef unsigned short JAS_GFX_PIXEL;
#elif (JVM_PIXEL_FORMAT == 8565 || JVM_PIXEL_FORMAT == 8666 || JVM_PIXEL_FORMAT == 8888)
#error "spreadtrum support only 565."
#else
#error "spreadtrum support only 565."
#endif



typedef struct
{
    JAS_GFX_PIXEL *data; 
    int width;        
    int height;       
} JAS_GFX_SCREEN_BUFF_S;


typedef struct
{
    int x; 
    int y; 
    int w; 
    int h; 
} JAS_GFX_CLIP_S;


#endif /* JAS_GFX_H */
