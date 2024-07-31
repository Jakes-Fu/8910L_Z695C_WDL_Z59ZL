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
* ��������
*
* @param pBuf	ָ����Ҫ���ܵ�����buffer
* @param nBufLength	��Ҫ���ܵ����ݳ���(Ҫ��nBufLength��256����������)
* @param nOffsetInFile	ָ���ý����������ļ��е�λ�ã��Ա��Ż������磺ֻ�в��ֵ�buffer�ż��ܣ���ʡPC����ʱ�䣩
* @param nFormat			������WMA����MP3��ʽ 0:Mp3 1:Wma
*
* @return  ����0��ʾ�����ɹ�
*/
int KuroDecode(unsigned char* pBuf, int nBufLength, unsigned long nOffsetInFile, int nFormat);
/**
* ��������
*
* @param pBuf	ָ����Ҫ���ܵ�����buffer
* @param nBufLength	��Ҫ���ܵ����ݳ���(Ҫ��nBufLength��256����������)
* @param nOffsetInFile	ָ���ý����������ļ��е�λ�ã��Ա��Ż������磺ֻ�в��ֵ�buffer�ż��ܣ���ʡPC����ʱ�䣩
* @param nFormat			������WMA����MP3��ʽ 0:Mp3 1:Wma
*
* @return  ����0��ʾ�����ɹ�
*/
int KuroEncode(unsigned char* pBuf, int nBufLength, unsigned long nOffsetInFile, int nFormat);
#ifdef   __cplusplus
    }
#endif
#endif 
#endif
