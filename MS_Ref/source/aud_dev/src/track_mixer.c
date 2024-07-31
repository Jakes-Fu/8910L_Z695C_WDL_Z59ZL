/******************************************************************************
 ** File Name:      track_mixer.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Voiceband(ARM Controlled) audioservice adapter layer.*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "adm.h"
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define VB_MAX_DA_VALUE (32767)
#define VB_MIN_DA_VALUE (-32768) 
//#define DECODE_IN_ZSP
#define MULTITRACK_BUFFER_SIZE         (8*1024)    //half-word (8*1024/4)//
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

LOCAL void ARMVB_AddrAdjust(
    uint32 uiTrackNum,
    int16 **const ppsLeftData,
    int16 **const ppsRightData,
    uint32 uiDataOffset
)
{
    switch (g_adm_dec_output_buf[uiTrackNum].channel_choise_e)
    {
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr[uiDataOffset]));
            *ppsRightData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r[uiDataOffset]));
        }
        break;
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr[uiDataOffset]));
            *ppsRightData = *ppsLeftData;
        }
        break;
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r[uiDataOffset]));
            *ppsRightData = *ppsLeftData;
        }
        break;
    default:
        {
            SCI_PASSERT(SCI_FALSE, ("channel_choise_e:%d.", g_adm_dec_output_buf[uiTrackNum].channel_choise_e));/*assert verified*/
        }
        break;
    }
}
static uint32 _Track_Mixer_Inner(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  int downmix,
                  int is_mute);

uint32 _Track_Mixer(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  unsigned num_of_blocks,
                  int downmix,
                  int is_mute)
{
    uint32 cpy_total_size = 0;
    uint32 cpy_once_size = 0;

    if(SCI_NULL != mix_buf_left)
    {
        SCI_MEMSET(mix_buf_left, 0, MULTITRACK_BUFFER_SIZE *sizeof(int32) );
    }
    if(SCI_NULL != mix_buf_right)
    {
        SCI_MEMSET(mix_buf_right, 0, MULTITRACK_BUFFER_SIZE *sizeof(int32) );
    }
    while (num_of_blocks)
    {
#ifdef DECODE_IN_ZSP
        cpy_once_size = _Track_Mixer_Inner(track_ptr,
                  mix_buf_left,
                  mix_buf_right,
                  dest_buf_left,
                  dest_buf_right,
                  samples_per_block,
                    0,//downmix=0,mono for zsp decode test
                  is_mute);
#else
        cpy_once_size = _Track_Mixer_Inner(track_ptr,
                  mix_buf_left,
                  mix_buf_right,
                  dest_buf_left,
                  dest_buf_right,
                  samples_per_block,
                  downmix,
                  is_mute);
#endif
        dest_buf_left += samples_per_block;
        dest_buf_right += samples_per_block; //lint !e613
        num_of_blocks--;
    cpy_total_size += cpy_once_size;
    }
    return cpy_total_size;
}

#define MULTI_TRACK_JUDGEMENT
BOOLEAN _Track_Mixer_End(ADM_TRACK_T *track_ptr)
{
    BOOLEAN bBool = SCI_TRUE;
#ifdef MULTI_TRACK_JUDGEMENT
    ADM_TRACK_T *track_p = NULL;
    uint32 all_track_num = 0;
#endif
    
    if (track_ptr == SCI_NULL)
    {
        return SCI_FALSE;
    }
#ifdef MULTI_TRACK_JUDGEMENT
    track_p = track_ptr;
    while(track_p)
    {
        SCI_ASSERT (track_p->uiTrackNum < ADM_MAX_TRACK_NUM); /*assert verified*/
        all_track_num++;
        track_p = track_p->ptNextTrack;
    }

    SCI_TRACE_LOW("[ljf] _Track_Mixer_End all_track_num= %d ", all_track_num);
    track_p = track_ptr;
    if (all_track_num == 1)
    {
        while(track_p)
        {
            SCI_ASSERT (track_p->uiTrackNum < ADM_MAX_TRACK_NUM); /*assert verified*/
            SCI_TRACE_LOW("[ljf] _Track_Mixer_End track_ptr->uiTrackNum= %d ", track_p->uiTrackNum);
            if(!g_adm_dec_output_buf[track_p->uiTrackNum].bDecodeEnd && g_adm_track_controller[track_p->uiTrackNum].track_on)
            {
                return SCI_FALSE;
            }
            track_p = track_p->ptNextTrack;
        }
    }
    else
    {
        while(track_p)
        {
            SCI_ASSERT (track_p->uiTrackNum < ADM_MAX_TRACK_NUM); /*assert verified*/
            SCI_TRACE_LOW("[ljf] _Track_Mixer_End track_ptr->uiTrackNum= %d ", track_p->uiTrackNum);
            if(g_adm_track_controller[track_p->uiTrackNum].track_on)
            {
                return SCI_FALSE;
            }
            track_p = track_p->ptNextTrack;
        }
    }
#else
    while(track_ptr)
    {
        SCI_ASSERT (track_ptr->uiTrackNum < ADM_MAX_TRACK_NUM); /*assert verified*/
        if(!g_adm_dec_output_buf[track_ptr->uiTrackNum].bDecodeEnd && g_adm_track_controller[track_ptr->uiTrackNum].track_on)
        {
            return SCI_FALSE;
        }
        track_ptr = track_ptr->ptNextTrack;
    }
#endif
    return SCI_TRUE;
}
void _Track_Mixer_MemAdd(int32 *dst, int16 *src, uint32 cnt)
{
    uint32 i = 0;
    
    SCI_ASSERT(dst != SCI_NULL);
    SCI_ASSERT(src != SCI_NULL);

    for(i = 0; i < cnt; i++)
    {
        dst[i] += (int32)src[i];
    }
}

void _BAK_Track_Mixer_Mem_int32toint16(int32 *src, int16 *dst, uint32 cnt)
{
    uint32 i = 0;
    int32 sample;
    
    SCI_ASSERT(dst != SCI_NULL);
    SCI_ASSERT(src != SCI_NULL);

    for (i = 0; i < cnt; i++)
    {
        sample = src[i];
        if (sample > VB_MAX_DA_VALUE)
            sample = VB_MAX_DA_VALUE;
        else if (sample < VB_MIN_DA_VALUE)
            sample = VB_MIN_DA_VALUE;

        dst[i] = (int16)sample;
    }
}

void _Track_Mixer_Mem_int32toint16(int32 *pibuffer, uint32 cnt)
{
    uint32 i = 0;
    int32 sample;
    
    SCI_ASSERT(pibuffer != SCI_NULL);

    for (i = 0; i < cnt; i++)
    {
        sample = pibuffer[i];
        if (sample > VB_MAX_DA_VALUE)
            sample = VB_MAX_DA_VALUE;
        else if (sample < VB_MIN_DA_VALUE)
            sample = VB_MIN_DA_VALUE;

        pibuffer[i] = sample;
    }
}

void _Track_Mixer_GetReadBuffer(uint32 uiTrackNum, int16 **const ppsLeftData, int16 **const ppsRightData)
{
    SCI_ASSERT(ppsLeftData != SCI_NULL);
    SCI_ASSERT(ppsRightData != SCI_NULL);
    SCI_ASSERT (uiTrackNum < ADM_MAX_TRACK_NUM); 

    switch (g_adm_dec_output_buf[uiTrackNum].channel_choise_e)
    {
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL:
        {
            *ppsLeftData = g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr;
            *ppsRightData = g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r;
        }
        break;
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT:
        {
            *ppsLeftData =g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr;
            *ppsRightData = *ppsLeftData;
        }
        break;
        case DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT:
        {
            *ppsLeftData = g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r;
            *ppsRightData = *ppsLeftData;
        }
        break;
        default:
        {
            SCI_PASSERT(SCI_FALSE, ("channel_choise_e:%d.", g_adm_dec_output_buf[uiTrackNum].channel_choise_e));/*assert verified*/
        }
        break;
    }
}

uint32 _Track_Mixer_CopyToTmpBuffer( int32 *mix_buf_left, uint32 uiTrackNum,uint32 uiBufferlen)
{
    int16 *left_data_ptr = SCI_NULL;
    int16 *right_data_ptr = SCI_NULL;
    ADM_DecOutputBuf_T *pDecOutputBuf = SCI_NULL;
    uint32 i = 0;

    SCI_ASSERT(mix_buf_left != SCI_NULL);
    SCI_ASSERT(uiTrackNum< ADM_MAX_TRACK_NUM);

    pDecOutputBuf = &g_adm_dec_output_buf[uiTrackNum];
    SCI_TRACE_LOW("[xx] func[%s] uiBufferlen= %d ;data_count = %d", __FUNCTION__, uiBufferlen,pDecOutputBuf->data_count);
    if (uiBufferlen > pDecOutputBuf->data_count)
    {
        uiBufferlen = pDecOutputBuf->data_count;
    }
    if (0 == pDecOutputBuf->data_count)
    {
        SCI_TRACE_LOW("[xx] func[%s] data_count = %d, so return 0", __FUNCTION__, pDecOutputBuf->data_count);
        return 0;
    }

    _Track_Mixer_GetReadBuffer(uiTrackNum, &left_data_ptr, &right_data_ptr);

    for(i = 0; i < uiBufferlen; i++)
    {
        mix_buf_left[i] += left_data_ptr[(pDecOutputBuf->data_out_pos +i)%pDecOutputBuf->buffer_size];
    }

    pDecOutputBuf->data_out_pos = (pDecOutputBuf->data_out_pos + uiBufferlen) % pDecOutputBuf->buffer_size;
    pDecOutputBuf->data_count -= uiBufferlen;

    //SCI_ASSERT(pDecOutputBuf->data_count > 0 && pDecOutputBuf->data_count <= pDecOutputBuf->buffer_size);

    SCI_TRACE_LOW("[xx] func[%s] copy lenth[0x%x] ", __FUNCTION__, uiBufferlen);
    return uiBufferlen;
}

uint32 _Track_Mixer_DownmixCopyToTmpBuffer( int32 *mix_buf_left, uint32 uiTrackNum,uint32 uiBufferlen)
{
    int16 *left_data_ptr = SCI_NULL;
    int16 *right_data_ptr = SCI_NULL;
    ADM_DecOutputBuf_T *pDecOutputBuf = SCI_NULL;
    uint32 uitmp = 0;
    uint32 i = 0;

    SCI_ASSERT(mix_buf_left != SCI_NULL);
    SCI_ASSERT(uiTrackNum< ADM_MAX_TRACK_NUM);

    pDecOutputBuf = &g_adm_dec_output_buf[uiTrackNum];
    SCI_TRACE_LOW("[xx] func[%s] uiBufferlen= %d ;data_count = %d", __FUNCTION__, uiBufferlen,pDecOutputBuf->data_count);
    if (uiBufferlen > (2 * pDecOutputBuf->data_count))
    {
        uiBufferlen = (2 * pDecOutputBuf->data_count);
    }
    if (0 == pDecOutputBuf->data_count)
    {
        SCI_TRACE_LOW("[xx] func[%s] data_count = %d, so return 0", __FUNCTION__, pDecOutputBuf->data_count);
        return 0;
    }

    _Track_Mixer_GetReadBuffer(uiTrackNum, &left_data_ptr, &right_data_ptr);

    for(i = 0; i < uiBufferlen; i+=2)
    {
        uitmp = (pDecOutputBuf->data_out_pos +i/2)%pDecOutputBuf->buffer_size;
        mix_buf_left[i] += left_data_ptr[uitmp];
        mix_buf_left[i+1] += right_data_ptr[uitmp];
    }

    pDecOutputBuf->data_out_pos = (pDecOutputBuf->data_out_pos + uiBufferlen/2) % pDecOutputBuf->buffer_size;
    pDecOutputBuf->data_count -= uiBufferlen/2;

    SCI_TRACE_LOW("[xx] func[%s] copy lenth[0x%x] ", __FUNCTION__, uiBufferlen);
    return uiBufferlen;
}


uint32 _Track_Mixer_Inner(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block, // HALF-WORLD
                  int downmix,
                  int is_mute)
{
    int32 n = ADM_MAX_TRACK_NUM;
    int track_count = 0;
    uint32 copy_size = 0;
    uint32 mid_copy_size = 0;
    int32 *vb_da0_multitrack;
    /*int32 *vb_da1_multitrack;*/

    // mix every tracks that is on
    while (track_ptr != SCI_NULL)
    {
        n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/
        SCI_TRACE_LOW("[xx] func[%s] uiTrackNum[0x%x] track_on[0x%x]", __FUNCTION__, n, g_adm_track_controller[n].track_on);

        track_ptr = track_ptr->ptNextTrack;

       // if decode in zsp, the g_adm_dec_output_buf not enough to pass watermark to turn on track_on when zsp callback first come on 
       //it result in put zero data to sharmemory and zsp assert. zgt 2018.11.22
#ifdef DECODE_IN_ZSP
        if(1)
#else
        if (g_adm_track_controller[n].track_on)
#endif
       {
            vb_da0_multitrack = mix_buf_left;
            /*vb_da1_multitrack = mix_buf_right;*/

            // COPY DATA
            SCI_DisableIRQ();
            if (downmix == 0)
            {
                mid_copy_size = _Track_Mixer_CopyToTmpBuffer(vb_da0_multitrack, n, samples_per_block);
            }
            else
            {
                mid_copy_size = _Track_Mixer_DownmixCopyToTmpBuffer(vb_da0_multitrack, n, samples_per_block);
            }
            SCI_RestoreIRQ();
            if (copy_size < mid_copy_size)
            {
                SCI_TRACE_LOW("func[%s] copy_size:%d mid_copy_size:%d", __FUNCTION__, copy_size, mid_copy_size);
                copy_size = mid_copy_size;
            }

            if (!is_mute)
            {
                track_count++;
            }
        }
        else // (track_on == 0)
        {
            SCI_TRACE_LOW("func[%s] trackon is 0, do nothing", __FUNCTION__);
            //copy_size = samples_per_block;
        }
    }
    if (0 == copy_size)
    {
        SCI_TRACE_LOW("_Track_Mixer_Inner no data, so return [%d] data", samples_per_block);
        return samples_per_block;
    }
//for video play seek noise.zgt 
    if (is_mute || track_count==0 /*no track outputs data*/)
    {
        SCI_TRACE_LOW("_Track_Mixer send zero, mute time:%d, count: %d, %d.", SCI_GetTickCount(), g_adm_dec_output_buf[0].data_count, samples_per_block);     
        copy_size = samples_per_block;
    }
    else
    {
        /*begin xin.xu1 20190409 for bugid 1032174*/
        if(SwitchDeviceTimerGetFlag())
        {
            SCI_MEMSET(vb_da0_multitrack, 0, MULTITRACK_BUFFER_SIZE *sizeof(int32) );
        }
        else
        {
            _Track_Mixer_Mem_int32toint16(vb_da0_multitrack, copy_size);
        }
        }
        /*end xin.xu1 20190409 for bugid 1032174*/
    
    return copy_size;
}

void _Read_SingleTrack(ADM_TRACK_T *track_ptr,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned sample_count,
                  BOOLEAN is_mute)
{
    int track_found = 0;
    uint32 uiCount = sample_count;
    int16 *destLeft = dest_buf_left;
    int16 *destRight = dest_buf_right;

    // mix every tracks that is on
//        SCI_MEMSET(dest_buf_left, 0, sample_count*2);
//        SCI_MEMSET(dest_buf_right, 0, sample_count*2);
    while (track_ptr != SCI_NULL)
    {
        int32 n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/
        
        track_ptr = track_ptr->ptNextTrack;
        
        if (g_adm_track_controller[n].track_on)
        {
            track_found = 1;
            while(sample_count)
            {
                uint32 copy_count;
                SCI_ASSERT(g_adm_dec_output_buf[n].data_count <= g_adm_dec_output_buf[n].buffer_size); /*assert verified*/
                SCI_ASSERT(g_adm_dec_output_buf[n].data_out_pos < g_adm_dec_output_buf[n].buffer_size); /*assert verified*/
                if (g_adm_dec_output_buf[n].data_out_pos + sample_count <= g_adm_dec_output_buf[n].buffer_size)
                {
                    copy_count = sample_count;
                }
                else
                {
                    copy_count = g_adm_dec_output_buf[n].buffer_size - g_adm_dec_output_buf[n].data_out_pos;
                }
                if (copy_count > g_adm_dec_output_buf[n].data_count)
                {
                    copy_count = g_adm_dec_output_buf[n].data_count;
                }

                // get input buffer pointer
                if (copy_count > 0)
                {
                    int16 *left_data_ptr;
                    int16 *right_data_ptr;
                    ARMVB_AddrAdjust(n, &left_data_ptr, &right_data_ptr, g_adm_dec_output_buf[n].data_out_pos);
                    SCI_MEMCPY(dest_buf_left, left_data_ptr, copy_count*2);
                    SCI_MEMCPY(dest_buf_right, right_data_ptr, copy_count*2);
                }
                else
                {
                    SCI_MEMSET(dest_buf_left, 0, sample_count*2);
                    SCI_MEMSET(dest_buf_right, 0, sample_count*2);
                    break;
                }
                dest_buf_left += copy_count;
                dest_buf_right += copy_count;
                sample_count -= copy_count;
                SCI_DisableIRQ();
                g_adm_dec_output_buf[n].data_count -= copy_count;
                g_adm_dec_output_buf[n].data_out_pos += copy_count;
                if (g_adm_dec_output_buf[n].data_out_pos ==  g_adm_dec_output_buf[n].buffer_size)
                {
                    g_adm_dec_output_buf[n].data_out_pos = 0;
                }
                SCI_RestoreIRQ();
            }
            break;
        }
    }

    //SCI_TRACE_LOW("_Read_SingleTrack %d, %d, %d.", is_mute, sample_count, uiCount);

    /*xin.xu1 20190409 for bugid 1032174*/
    if (!track_found || is_mute || SwitchDeviceTimerGetFlag())
    {
        if(dest_buf_left)
        {
            SCI_MEMSET(destLeft, 0, (uiCount<<1));
        }
        if(dest_buf_right)
        {
            SCI_MEMSET(destRight, 0, (uiCount<<1));
        }
        
        return;
    }
}

uint32 _GetDataCount_SingleTrack(ADM_TRACK_T *track_ptr)
{
    while (track_ptr != SCI_NULL)
    {
        int32 n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/
        
        track_ptr = track_ptr->ptNextTrack;
        
        if (g_adm_track_controller[n].track_on)
        {
            return g_adm_dec_output_buf[n].data_count;
        }
    }
    return 0;
}

