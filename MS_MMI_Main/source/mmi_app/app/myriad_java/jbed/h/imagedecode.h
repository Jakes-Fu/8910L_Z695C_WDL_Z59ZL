/*
 * Copyright 2001-2008 Esmertec AG. All Rights Reserved.
 * $Id$
 */

/*
 * @(#)imageDecode.h    1.7 01/07/24 @(#)
 * Copyright (c) 1999-2001 Sun Microsystems, Inc. All Rights Reserved.
 *
 * This software is the confidential and proprietary information of Sun
 * Microsystems, Inc. ("Confidential Information").  You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered into
 * with Sun.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 */

#ifndef _IMAGEDECODE_H_
#define _IMAGEDECODE_H_

/* Wrapper "classes" to provide device-independent access to
 * source data streams and device-dependent image formats.
 */

typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;

typedef unsigned char uchar;
typedef void* (*imageAllocator)(void *closure, int nbytes, bool_t alpha);

/* A destination image descriptor.
 *
 * Typical usage is :
 *
 *   LCDUIcreateImageDst returns one of these (or an extension).
 *   But it need not create the underlying device-specific data.
 *
 *   dst->getStorageSize() is used to query memory requirements,
 *   dst->alloc() used to create the storage in the java heap, and
 *   attach it to the associated java image object.
 *
 *   dst->setSize() is used to set the dimensions
 *
 *   Pixels and attributes are written.
 *
 *   dst->done() is called when all writes have been finished. This
 *   flushes any outstanding work to the image, and frees dst itself.
 *   IT IS INVALID TO REFER TO dst AFTER CALLING THIS FUNCTION.
 */

typedef struct _idd {
    int    depth;
    void   *ptr;                /* pointer to device specific image struct */
    void   *imagePtr;           /* pointer to device specific image data */
    void   *alPtr;              /* pointer to alpha data for image (or NULL) */
    void   **closure;           /* Image object this struct is attached to */

#ifdef M3G_JPEG_SUPPORT
    int    jpegColorSpace;      /* # of color components in JPEG image, see jpeglib.h */
    int    jpegNumComponents;   /* colorspace of JPEG image, see jpeglib.h */
#endif

    void   (*setColormap)(struct _idd *self, long *map, int length);
    int    (*getStorageSize)(struct _idd *self, int width, int height);
    IBOOL  (*setSize)(struct _idd *self, int width, int height);
    void   (*setPixel)(struct _idd *self, int x, int y,
                                    uchar a, uchar b, uchar c);
    void   (*sendPixels)(struct _idd *self, int y,
                                    uchar *scanline, uchar *alphas, IBOOL rgb);
    void   (*sendPackedPixels)(struct _idd *self, int y, uchar *scanline);
    void   (*copyPixels)(struct _idd *self, void *sourceData);
    void   (*discardAlphaData)(struct _idd *self);

#ifdef MIDP2
    void   (*copyPixelsTransformed)(struct _idd *self, void *sourceData,
                                    IU8 *srcAlphas,
                                    int x, int y, int width, int height,
                                    int transform);
    void   (*setARGBPixels)(struct _idd *self, ARRAY pRGBData,
                                    int width, int height, IBOOL processAlpha);
#endif

    /* may be called with invalid image data */
    void   (*done)(struct _idd *self);
    void   (*getSize)(struct _idd *self, int *width, int *height);
    imageAllocator alloc;
    void   (*setTransparentColor) (struct _idd *self, long color,
                                    int paletteIndex);
    void   (*postfixTransparency) (struct _idd *self);
} imageDstData, *imageDstPtr;

typedef void (*alphaDiscarder)(imageDstPtr self);

extern bool_t decodeImageFromData(imageDstPtr dst,
                        uint8_t **imgData, int len);

bool_t PNGdecodeImage(uint8_t **img, int len, imageDstPtr dst);

#ifdef JPEG_SUPPORT
bool_t JPEGdecodeImage(uint8_t **img, int len, imageDstPtr dst);
#endif

#ifdef GIF_SUPPORT
extern bool_t GIFdecodeImage(uint8_t **img, int len, imageDstPtr dst);
#endif

#ifdef BMP_SUPPORT
extern bool_t BMPdecodeImage(uint8_t **img, int len, imageDstPtr dst);
#endif

#ifdef WBMP_SUPPORT
/**
 * Porting interface to the Wireless Bitmap Graphics (WBMP) decoder.
 * See the Wireless Application Environment Defined Media Type Specification
 * Version 15-May-2001 (WAP-237-WAEMT-20010515-a) for details.
 *
 * @param src The WBMP data stream.
 * @param dst The lcdui image to draw to.
 * @return TRUE if the decoding was sucessful, FALSE otherwise. Will raise an
 * IllegalArgumentException if the WBMP stream is badly formatted.
 */
extern bool_t WBMPdecodeImage(uint8_t **img, int len, imageDstPtr dst);
#endif

extern imageDstPtr
LCDUIcreateImageDst(bool_t mutable, imageAllocator alloc,
                                alphaDiscarder discardAplha, void **closure);


#ifdef NATIVE_IMAGE_DECODER_SUPPORT

typedef enum
{
    IMAGE_TYPE_PNG = 1,
    IMAGE_TYPE_GIF,
    IMAGE_TYPE_BMP,
    IMAGE_TYPE_WBMP,
    IMAGE_TYPE_JPEG,
}JBED_IMAGE_TYPE;

extern int JBED_GUIIMG_GetImgInfo(
                                      uint8_t             *data_ptr,      //in:
                                      uint32_t            data_size,      //in:
                                      uint32_t            *src_width,   //out:
                                      uint32_t            *src_height    //out:
                                      );

extern int JBED_GUIIMG_Decode(  uint8_t*  data_ptr,               //in
                                    uint32_t  data_size,              //in
                                    uint8_t*  alpha_buf_ptr,          //in/out
                                    uint32_t  alpha_buf_size,         //in

                                    bool_t    is_handle_transparent,  //in
                                    bool_t    is_exist_bg,            //in
                                    uint32_t  display_w,              //in
                                    uint32_t  display_h,              //in

                                    uint8_t*  decode_data_ptr,        //in/out
                                    uint32_t  decode_data_size,       //in
                                    uint32_t* is_handle_alpha,        //out
                                    uint32_t* image_type              //out
                                  );

bool_t NativeDecodeImage(uint8_t **img, int len, imageDstPtr dst,JBED_IMAGE_TYPE imgType);

#endif  /*NATIVE_IMAGE_DECODER_SUPPORT*/


#endif /* _IMAGEDECODE_H_ */
