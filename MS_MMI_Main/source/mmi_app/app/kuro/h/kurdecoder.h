#ifdef KURO_SUPPORT
/******************************************************************************
 ** File Name:    kurdecoder.h                                                    *
 ** Author:       cmzs                                                 *
 ** DATE:         10/25/2007                                                    *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 10/25/2007     cmzs     Create.                                     *
 ******************************************************************************/

#ifndef KURDECODE_H_
#define KURDECODE_H_
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**
* 解密数据
*
* @param pBuf	指向需要解密的数据buffer
* @param nBufLength	需要解密的数据长度(要求nBufLength是256的整数倍数)
* @param nOffsetInFile	指出该解密数据在文件中的位置（以便优化，比如：只有部分的buffer才加密，节省PC计算时间）
* @param nFormat			给出是WMA还是MP3格式 0:Mp3 1:Wma
*
* @return  大于0表示操作成功
*/
int KuroDecode(unsigned char* pBuf, int nBufLength, unsigned long nOffsetInFile, int nFormat);
/**
* 加密数据
*
* @param pBuf	指向需要解密的数据buffer
* @param nBufLength	需要解密的数据长度(要求nBufLength是256的整数倍数)
* @param nOffsetInFile	指出该解密数据在文件中的位置（以便优化，比如：只有部分的buffer才加密，节省PC计算时间）
* @param nFormat			给出是WMA还是MP3格式 0:Mp3 1:Wma
*
* @return  大于0表示操作成功
*/
int KuroEncode(unsigned char* pBuf, int nBufLength, unsigned long nOffsetInFile, int nFormat);
#ifdef   __cplusplus
    }
#endif
#endif 
#endif
