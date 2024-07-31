/*
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
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
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT
 * BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN
 * IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES
 */

#include "javacall_image.h"
#include "javacall_logging.h"
#include "img_dec_interface.h"
#include "guiimg.h"


/**
 * Get image information
 * 
 * @param source        Pointer to image data source
 * @param sourceSize    Size of source in bytes
 * @param imageInfo     Pointer to javacall_image_info type buffer
 * 
 * @retval JAVACALL_OK      
 *          imageData is acceptable format and image information returned by imageInfo argument     
 * @retval JAVACALL_FAIL    
 *          imageData is not acceptable
 */
javacall_result javacall_image_get_info(const void* source, long sourceSize,
                                        /*OUT*/ javacall_image_info* imageInfo)
{
    javacall_result     res = JAVACALL_FAIL ;
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; 
    
    dec_src.src_ptr       = source;     /*lint !e605 */
    dec_src.src_file_size = sourceSize;
    
    get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
    if (IMG_DEC_RET_SUCCESS == get_result) {
        switch (dec_info.img_type) {
        case IMG_DEC_TYPE_BMP:
            imageInfo->width = dec_info.img_detail_info.bmp_info.img_width;
            imageInfo->height= dec_info.img_detail_info.bmp_info.img_height;
            res = JAVACALL_OK ;
            break;
        case IMG_DEC_TYPE_WBMP:
            imageInfo->width = dec_info.img_detail_info.wbmp_info.img_width;
            imageInfo->height = dec_info.img_detail_info.wbmp_info.img_height;
            res = JAVACALL_OK ;
            break;
        case IMG_DEC_TYPE_JPEG:
            imageInfo->width = dec_info.img_detail_info.jpeg_info.img_width;
            imageInfo->height = dec_info.img_detail_info.jpeg_info.img_heigth;
            res = JAVACALL_OK ;
            break;
        case IMG_DEC_TYPE_PNG:
            imageInfo->width = dec_info.img_detail_info.png_info.img_width;
            imageInfo->height = dec_info.img_detail_info.png_info.img_height;
            res = JAVACALL_OK ;
            break;
        case IMG_DEC_TYPE_GIF:
            imageInfo->width = dec_info.img_detail_info.gif_info.img_width;
            imageInfo->height = dec_info.img_detail_info.gif_info.img_height;
            res = JAVACALL_OK ;
            break;
        default:
            javacall_printf(LOG_STR_LEAVE("fail"));
            res =  JAVACALL_FAIL;
            break;
        }
        imageInfo->hasAlpha = JAVACALL_FALSE;      
    }
    javacall_printf(LOG_STR_LEAVE("fail"));
    return res;
}

/**
 * Start a decoding transaction of the source image to array of javacall_pixel type
 *
 * For Synchronous decoding:
 *   1.Copy the address of the source buffer to a static variable (also the handle can be used for that).
 *   2.Return JAVACALL_OK
 *
 * For Asynchronous decoding:
 *   1.Allocate a memory block of size sourceSize
 *   2.Copy the source buffer to the new allocated buffer.
 *   3.Send the buffer to the decoder which is in another task.
 *   4.Mark the decoding operation with a unique handle.
 *   5.Return JAVACALL_WOULD_BLOCK
 *
 * @param source        Pointer to image data source
 * @param souceSize     Byte size of source
 * @param width         Width of source data
 * @param height        Height of source data
 * @param handle        Handle of this image decoding. This could be used from async decoding mode
 *                      It could return a NULL value, if this API return JAVACALL_OK
 *
 * @retval JAVACALL_OK          Decoding OK! - Synchronous decoding
 * @retval JAVACALL_FAIL        Fail
 * @retval JAVACALL_WOULD_BLOCK Image decoding performed asynchronously
 */
javacall_result javacall_image_decode_start(const void* source, 
                                            long sourceSize,
                                            long width, 
                                            long height,
                                            /*OUT*/ javacall_handle* handle){
    javacall_image_info imageInfo;
    javacall_image_data* pImageData;

    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};

    if(NULL == handle) {
        javacall_printf(LOG_STR_LEAVE("fail, NULL == handle"));
        return JAVACALL_FAIL;
    }

    if (JAVACALL_OK != javacall_image_get_info(source, sourceSize, &imageInfo)) {
        javacall_printf(LOG_STR_LEAVE("fail, parse image info failed"));
        return JAVACALL_FAIL;
    }

    if(width != imageInfo.width || height != imageInfo.height) {
        javacall_printf(LOG_STR_LEAVE("Image info is not match."));
        return JAVACALL_FAIL;
    }
    
    pImageData = (javacall_image_data*)handle;
    img_src.src_buf_ptr = source;   /*lint !e605*/
    img_src.src_data_size = sourceSize;
    img_display.dest_width = width;
    img_display.dest_height = height;
    img_display.crop_rect_ptr = PNULL;
    img_output.actual_width = width;
    img_output.actual_height = height;
    img_output.decode_data_ptr = (uint8*)pImageData->decodeBuf;
    img_output.decode_data_size = pImageData->decodeBufSize;

    if (GUIIMG_Decode(&img_src, &img_display, &img_output) == FALSE) {
        javacall_printf(LOG_STR_LEAVE("fail, decode failed"));
        return JAVACALL_FAIL;
    }

    javacall_printf(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/**
 * Finalize the image decoding transaction
 *
 * For Synchronous decoding:
 *   1.This function will be called right after javacall_image_decode_start() returns with JAVACALL_OK. 
 *   2.Decode the source buffer that was stored previously into decodeBuf , alphaBuf.
 * For Asynchronous decoding:
 *   1.This function will be called after javanotify_on_image_decode_end is invoked.
 *   2.The platform should only copy the decoded image to decodeBuf , alphaBuf.
 *
 * @param handle        The handle get from javacall_image_decode_start
 * @param decodeBuf     Pointer to decoding target buffer
 * @param decodeBufSize Size of decodeBuf in bytes
 * @param alphaBuf      Pointer to alpha data buffer
 *                      It could be a NULL if there is no need to get alpha data
 * @param alphaBufSize  Size of alphaBuf in bytes
 *
 * @retval JAVACALL_OK      Success
 * @retval JAVACALL_FAIL    Fail
 */
javacall_result javacall_image_decode_finish(javacall_handle handle,
                                             /*OUT*/ javacall_pixel* decodeBuf, long decodeBufSize,
                                             /*OUT*/ char* alphaBuf, long alphaBufSize) {
    return JAVACALL_FAIL;
}


