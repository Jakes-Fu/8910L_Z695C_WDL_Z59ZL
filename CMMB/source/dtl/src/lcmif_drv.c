/*******************************************************************************
** File Name:      lcmif_drv.c                                                 *
** DATE:           15/10/2007                                                  *
** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:                                                                *
********************************************************************************

********************************************************************************
**                         Edit History                                        *
**-----------------------------------------------------------------------------*
** DATE            NAME           DESCRIPTION                                  *
** 15/10/2007     hongwei xu      Create.                                      *
** 02/11/2007     hongwei xu      eliminate all the strategy                   *
** 17/12/2007     hanbing zhang   add soft switch and support all cs interface *
*******************************************************************************/

/**----------------------------------------------------------------------------*
 **                         Dependencies                                       *
 **----------------------------------------------------------------------------*/

#include "os_api.h"
#include "tb_comm.h"
#include "lcmif_drv.h"
#include "sc_reg.h"
#include "arm_reg.h"
#include "lcm_user.h"

/**----------------------------------------------------------------------------*
 **                        Compiler Flag                                       *
 **----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/

//Index symbols
/*
*  Write index to IR with data bus set to this value means following access
*  aims at change or read the external memory write pointer
*/
#define WRITE_POINTER_HIGH_IDX        (2)
#define WRITE_POINTER_LOW_IDX         (3)
/*
*  Write index to IR with data bus set to this value means following access
*  aims at change or read the external memory read pointer
*/
#define READ_POINTER_HIGH_IDX         (4)
#define READ_POINTER_LOW_IDX          (5)
/*
*  If IR is set to this value,following access will modify or read
*  the internal read_burst_length register
*/
#define READ_BURST_LENGTH_IDX         (6)
/*
*  If IR is set to this value,following WR access will be write data
*/
#define DATA_ACCESS_IDX               (7)
/*
*  If LCM master WI with this value ,LCM slave will assert one write transaction
*  to AHB with start address set to "write_pointer"
*/
#define WRITE_TERMINATE_IDX           (8)
/*
*  If LCM master set IR to this value,LCM slave will set size to "burst_length" and
*  assert one burst read transaction to AHB
*/
#define READ_TRIGGER_IDX              (9)


/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/

#define LCMIF_MAX_BURST_LEN			(32)  //unit byte
#define LCMIF_TEST_RS_TIMES			(100) //no unit

//#define SOFT_HALFWORD_SWITCH

/**---------------------------------------------------------------------------*
** static value
*/

#define SDRAM_MAX_ADDR      0x91000000
/******************************************************************************/
// Description:    Test the RS status is '1' or not. Return KD_TRUE if success,
//               else KD_FALSE.
// Global resource dependence: 
// Author:  hongwei xu
// Note: 
/******************************************************************************/
LOCAL KDboolean Test_RS(void);


/******************************************************************************/
// Description:    set lcm parameter
// Global resource dependence: 
// Author:  hongwei xu
// Note: 
/******************************************************************************/
PUBLIC void LCMIF_SetParam(KDuint32 r_ahb_clk, //rocky ahb clock
                           KDuint32 s_ahb_clk  //seine ahb clock
                           )
{
    LCMUD_SetParam(r_ahb_clk,s_ahb_clk);
}

/******************************************************************************/
// Description:  Set the related registers to the default value, and initialize
//              the mutex used for the LCMIF.
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
PUBLIC KDboolean LCMIF_Init(
                          KDboolean is_reset
                          )
{
    KDboolean ret = KD_FALSE;
    ret = LCMUD_Init(is_reset);

    return ret;
}

/******************************************************************************/
// Description:   Close the LCMIF. Delete the mutex. 
// Global resource dependence: 
// Author:  hongwei xu
// Note:
/******************************************************************************/
PUBLIC KDboolean LCMIF_Close(void)
{
    return LCMUD_Close();
}

/*lint -save -e661 */ 
/******************************************************************************/
// Description:    Read 'read_len' bytes from 'read_from_addr' and save to 'read_ptr'.
//         Before read the data, the programme will get the MUTEX of the LCMIF first.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'read_from_addr' must be devided exactly by 4.
/******************************************************************************/
PUBLIC KDuint32 LCMIF_Read_Data(// return the number of data in byte have been read in
                       KDuint8* read_ptr,// the start addr used to save the read data
                       KDuint32 read_len, // the length of the data want to read, unit byte
                       KDuint32 read_from_addr// the start addr of the data to be read
                       )
{
    KDuint16 addr16 = 0;
    KDuint32 readtimes = 0; // counter for burst-read
    KDuint32 looptimes = 0; // times to do burst read with length of LCMIF_MAX_BURST_LEN
    KDuint32 remaindata = 0; // read_len mode LCMIF_MAX_BURST_LEN
    KDuint32 rdFifoTimes = 0;
    KDuint32 burstRDlen = 0;
    KDuint16 readdata =0;
    KDuint32 i = 0;
    KDuint32 read_max_len = read_len;
    KDuint32 rdminus = 0;
    
//    kdAssert(read_from_addr%4 == 0);
    kdAssert(read_from_addr%2 == 0);
    kdAssert(read_ptr != SCI_NULL);
    kdAssert(read_len != 0);
    kdAssert(read_from_addr<SDRAM_MAX_ADDR);
    
    //halfword地址
    if ( read_from_addr % 4 )
    {
        read_from_addr -= 2;
        addr16 = (uint16)(read_from_addr>>16);
        LCMUD_Write_Cmd(READ_POINTER_HIGH_IDX);
        LCMUD_Write_Data(addr16);        
        addr16 = (uint16)(read_from_addr);
        LCMUD_Write_Cmd(READ_POINTER_LOW_IDX);        
        LCMUD_Write_Data(addr16);

        LCMUD_Write_Cmd(READ_BURST_LENGTH_IDX);
        LCMUD_Write_Data(1);
        
        /* trigger the read action*/
        LCMUD_Write_Cmd(READ_TRIGGER_IDX);

        if(!Test_RS())//error occurs and no data read
        {
            return 0;
        }

        readdata = LCMUD_Read_Data();
        readdata = LCMUD_Read_Data();

        *read_ptr++ = (uint8)(readdata>>8);
        read_max_len--;
        if(read_max_len)
        {
            *read_ptr++ = (uint8)readdata;
            read_max_len--;
        }

        read_from_addr += 4;

        if(0 == read_max_len)
        {
            return read_len;
        }
    }

    kdAssert((read_from_addr%4) == 0);

    /*
    *  prepare for the read-loop
    */
    looptimes = read_max_len/LCMIF_MAX_BURST_LEN;
    remaindata = read_max_len%LCMIF_MAX_BURST_LEN;
    
    /* test the RS of the LCMIF */
    if(!Test_RS())
    {
        return 0;
    }
   
    /* set the read pointer */
    addr16 = (KDuint16)(read_from_addr>>16);
    LCMUD_Write_Cmd(READ_POINTER_HIGH_IDX);
    LCMUD_Write_Data(addr16);
    addr16 = (KDuint16)(read_from_addr);
    LCMUD_Write_Cmd(READ_POINTER_LOW_IDX);
    LCMUD_Write_Data(addr16);
    /*
    *  do "burst read" several times to read most of the data
    *  set the burst_read_length with LCMIF_MAX_BURST_LEN every time but 
    *  the read pointer is no need
    */
    burstRDlen = LCMIF_MAX_BURST_LEN/4; // unit 4-byte
    rdFifoTimes = LCMIF_MAX_BURST_LEN/2; // unit 2-byte
    /* set the burst read length */
    LCMUD_Write_Cmd(READ_BURST_LENGTH_IDX);
    LCMUD_Write_Data(burstRDlen);
    
    while(readtimes < looptimes)
    {        
        /* trigger the read action*/
        LCMUD_Write_Cmd(READ_TRIGGER_IDX);
        /* 
        *  test the RS of the LCMIF and 
        *  wait for the data be put in the FIFO
        */
        if(!Test_RS())//error occurs and no data read
        {
            /* Release the mutex.  */
            //SCI_PutMutex(s_lcmif_mutex);
            if ( !readtimes )
            {
                return 0;
            }
            else
            {
                return (readtimes * LCMIF_MAX_BURST_LEN-(read_max_len-read_len));
            }
        }

        /*
        *  now, read the data from the FIFO of the LCMIF
        */
        if(0 == readtimes)
        {
            readdata = LCMUD_Read_Data();
            rdminus = rdFifoTimes - 1;
        }
        else
        {
            rdminus = rdFifoTimes;
        }
        
        for( i = rdminus; i != 0; i--)//should be 16 times
        {            
            *read_ptr++ = (uint8)(readdata>>8);
            *read_ptr++ = (uint8)readdata;
            readdata = LCMUD_Read_Data();
        }
                      
        readtimes++;

    }
    if(looptimes)
    {
        *read_ptr++ = (uint8)(readdata>>8);
        *read_ptr++ = (uint8)readdata;
    }
    
    /*
    *  If 'remaindata' is not zero, there are more data need to read and
    *   remaindata<LCMIF_MAX_BURST_LEN
    */
    if(remaindata)
    {//more data need to read
        burstRDlen = remaindata/4;
        burstRDlen += (remaindata%4)?1:0;

        /* set the burst read length */        
        LCMUD_Write_Cmd(READ_BURST_LENGTH_IDX);
        LCMUD_Write_Data(burstRDlen);
        
        /* trigger the read action*/
        LCMUD_Write_Cmd(READ_TRIGGER_IDX);
        /* 
        *  test the RS of the LCMIF and 
        *  wait for the data be put in the FIFO
        */
        if(!Test_RS())//error occurs and no data read
        {
            /* Release the mutex.  */
            //SCI_PutMutex(s_lcmif_mutex);
            return (readtimes * LCMIF_MAX_BURST_LEN-(read_max_len-read_len));
        }
        /*
        *  now, read the data from the FIFO of the LCMIF
        */
        if(remaindata % 4)
        {//
            rdFifoTimes = (burstRDlen - 1) * 2;
            for(i=rdFifoTimes; i !=0; i--)
            {
                readdata = LCMUD_Read_Data();
                /* save the read data */
                *read_ptr++ = (KDuint8)(readdata>>8);
                *read_ptr++ = (KDuint8)readdata;
            }
            /*
            *  more than 1 byte but less than 4 bytes need to read
            */
            switch(remaindata%4)// 1 or 2 or 3
            {
            /*
            *  case 1: read twice and get 16bits once time but
            *             only 1 byte is needed
            */
            case 1:
                
                readdata = LCMUD_Read_Data();
                *read_ptr = (KDuint8)(readdata>>8);
                
                readdata = LCMUD_Read_Data();
                break;
            /*
            *  case 2: read twice and get 16bits once time but
            *             only 2 bytes is needed
            */
            case 2:
                readdata = LCMUD_Read_Data();
                *read_ptr++ = (KDuint8)(readdata>>8);
                *read_ptr = (KDuint8)readdata;

                readdata = LCMUD_Read_Data();
                break;
            /*
            *  case 3: read twice and get 16bits once time but
            *             only 3 byte is needed
            */
            case 3:
                readdata = LCMUD_Read_Data();
                *read_ptr++ = (KDuint8)(readdata>>8);
                *read_ptr++ = (KDuint8)readdata;

                readdata = LCMUD_Read_Data();
                *read_ptr = (KDuint8)(readdata>>8);
                break;
            default:
                break;
            }
        }
        else
        {//the remaindata can be devided exactly by 4
            rdFifoTimes = burstRDlen * 2;
            for(i=rdFifoTimes; i != 0; i-- )
            {
                readdata = LCMUD_Read_Data();
                *read_ptr++ = (uint8)(readdata>>8);
                *read_ptr++ = (uint8)readdata;
            }
        }//end if(remaindata%4)
    }//end if(remaindata)
        
    return read_len;
}

/******************************************************************************/
// Description:    Write 'write_len' bytes data from 'write_ptr' to 'write_to_addr'.
//           Before write, the programme will get the MUTEX of the LCMIF first.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'write_to_addr' must be devided exactly by 2.
/******************************************************************************/
PUBLIC KDuint32 LCMIF_Write_Data(// return the number of data have been written out
                        KDuint8* write_ptr,// the start addr of the data to be write.
                        KDuint32 write_len,// the length of the data to be write,unit byte
                        KDuint32 write_to_addr// the addr of data will be written to
                        )
{
    KDuint16 addr16 = 0;
    KDuint32 writetimes = 0; // counter for burst write
    KDuint32 looptimes = 0; // times of burst write need to do with length of LCMIF_MAX_BURST_LEN
    KDuint32 wrFifoTimes = 0;
    KDuint32 remaindata = 0;
    KDuint32 i = 0;
    KDuint16 writedata = 0;
    KDuint32 readaddr = 0;
    KDuint32 lastaddr = 0;
    KDuint32 left_len = write_len;
    KDuint8 readdata[4]={0};


    kdAssert(write_to_addr%2 == 0);
    kdAssert(left_len%2 == 0 && left_len != 0);
    kdAssert(write_ptr != SCI_NULL);
    kdAssert(write_to_addr<SDRAM_MAX_ADDR);
    
/*            

    if(SCI_GetMutex(s_lcmif_mutex, SCI_WAIT_FOREVER) != SCI_SUCCESS)
    {
        KD_TRACE_LOW("can not get the mutex of the LCMIF");
        return 0;
    }
*/
    //非4byte地址对齐
    if ( write_to_addr % 4 )
    {
        /* test the RS of the LCMIF */
        if(!Test_RS())
        {//error occurs and no data read
//            SCI_PutMutex(s_lcmif_mutex);
            return 0;
        }
        
        /* set the write pointer */
        addr16 = (KDuint16)(write_to_addr>>16);
        LCMUD_Write_Cmd(WRITE_POINTER_HIGH_IDX);
        LCMUD_Write_Data(addr16);
        addr16 = (KDuint16)(write_to_addr);
        LCMUD_Write_Cmd(WRITE_POINTER_LOW_IDX);
        LCMUD_Write_Data(addr16);
        
        /* set the DevRWMode to write*/
        /* write "data_access_idx" to LCMIF */
        LCMUD_Write_Cmd(DATA_ACCESS_IDX);
        
        writedata = ((KDuint16)(*write_ptr))<<8;
        write_ptr++;
        writedata |= (KDuint16)(*write_ptr);
        write_ptr++;
        LCMUD_Write_Data(writedata);
        /* write "write_terminate_idx" */
        LCMUD_Write_Cmd(WRITE_TERMINATE_IDX);
        
        write_to_addr +=  2;
        left_len -=2;

        if (!left_len)
        {
            if(!Test_RS())//error occurs and no data read //此时可以不测RS
            {
//                SCI_PutMutex(s_lcmif_mutex);
                return 0;
            }
//            SCI_PutMutex(s_lcmif_mutex);
            return write_len;            
        }
    }

    /* calculate the write times */
    looptimes = left_len/LCMIF_MAX_BURST_LEN;
    remaindata = left_len%LCMIF_MAX_BURST_LEN;

    /* test the RS of the LCMIF */
    if(!Test_RS())
    {//error occurs and no data read
//        SCI_PutMutex(s_lcmif_mutex);
        return 0;
    }
    /* set the write pointer */
    addr16 = (KDuint16)(write_to_addr>>16);
    LCMUD_Write_Cmd(WRITE_POINTER_HIGH_IDX);
    LCMUD_Write_Data(addr16);
    addr16 = (KDuint16)(write_to_addr);
    LCMUD_Write_Cmd(WRITE_POINTER_LOW_IDX);
    LCMUD_Write_Data(addr16);
    
    /*
    *  do "burst write" with length in LCMIF_MAX_BURST_LEN
    *  test the RS status but no need to set the write pointer
    */
    wrFifoTimes = LCMIF_MAX_BURST_LEN/2;
    while(writetimes < looptimes)
    {
        /* set the DevRWMode to write*/
        /* write "data_access_idx" to LCMIF */
        LCMUD_Write_Cmd(DATA_ACCESS_IDX);
        /* write data */
        for(i=0; i < wrFifoTimes; i++)
        {
            writedata = ((KDuint16)(*write_ptr))<<8;
            write_ptr++;
            writedata |= (KDuint16)(*write_ptr);
            write_ptr++;
            LCMUD_Write_Data(writedata);
        }
        /* write "write_terminate_idx" */
        LCMUD_Write_Cmd(WRITE_TERMINATE_IDX);
        
        ++writetimes;

        /* test the RS status */
        if(!Test_RS())//error occurs and no data read
        {
//            SCI_PutMutex(s_lcmif_mutex);
            return (writetimes * LCMIF_MAX_BURST_LEN);
        }
    }
    /*
    *  there are may be some bytes that less than LCMIF_MAX_BURST_LEN need to write
    */
    if(remaindata)//more data need to write
    {
        /* set the DevRWMode to write*/
        /* write "data_access_idx" to LCMIF */
        LCMUD_Write_Cmd(DATA_ACCESS_IDX);

        /* write data */
        wrFifoTimes = (remaindata/4) * 2;
        remaindata %= 4;
        for(i=0; i < wrFifoTimes; i++)
        {
            writedata = ((KDuint16)(*write_ptr))<<8;
            write_ptr++;
            writedata |= (KDuint16)(*write_ptr);
            write_ptr++;
            LCMUD_Write_Data(writedata);
        }
        if ( wrFifoTimes )
        {
            LCMUD_Write_Cmd(WRITE_TERMINATE_IDX);

        }

        if ( remaindata )
        {
            if ( wrFifoTimes )
            {
                if(!Test_RS())//error occurs and no data read //此时可以不测RS
                {
//                    SCI_PutMutex(s_lcmif_mutex);
                    return (left_len - remaindata);
                }

	            LCMUD_Write_Cmd(DATA_ACCESS_IDX);//data_access_idx                
            }

            writedata = ((KDuint16)(*write_ptr))<<8;
            write_ptr++;
            writedata |= (KDuint16)(*write_ptr);
            write_ptr++;
            LCMUD_Write_Data(writedata);

            LCMUD_Write_Cmd(WRITE_TERMINATE_IDX);

        }

        /*
        *  the length of the data need to write may be a odd number
        *  write the last byte specially as following
        */
        if(remaindata % 2)
        {
            //the lastaddr is even address
            lastaddr =  write_to_addr + left_len - 1;
            readaddr = lastaddr - (lastaddr%4);
            /* read the related 4-byte */
            if((LCMIF_Read_Data((KDuint8 *)&readdata, sizeof(KDuint32), readaddr)) != sizeof(KDuint32))
            {
                KD_TRACE_LOW("some error occurs during reading the related 4 bytes\n");
//                SCI_PutMutex(s_lcmif_mutex);
                return (left_len - 1);// only one byte is not written
            }
            /* construct the 16-bit value with the last byte and the next byte of the last byte */
            writedata = (KDuint16)((*write_ptr<<8)|(readdata[lastaddr%4+1]));
            /*
            *  write the last byte to the LCMIF 
            */
            /* test the RS status */
            //执行了上面的LCMIF_Read_Data,此处可以不再看RS状态，此时多做了一次状态测试@Hanbing.Zhang
            if(!Test_RS())//error occurs and no data read  
            {
//                SCI_PutMutex(s_lcmif_mutex);
                return (left_len -1);
            }
            /* set the DevRWMode to write*/
            LCMUD_Write_Cmd(DATA_ACCESS_IDX);//data_access_idx
            LCMUD_Write_Data(writedata);//write the data
            LCMUD_Write_Cmd(WRITE_TERMINATE_IDX);// write_terminal_idx
        }
        /* test the RS status */
        if(!Test_RS())//error occurs and no data read
        {
//            SCI_PutMutex(s_lcmif_mutex);
            return (left_len -1);
        }
    }
    
    /* release the mutex */
//    SCI_PutMutex(s_lcmif_mutex);
    return write_len;
}
/*lint -restore */

/******************************************************************************/
// Description:    Read a KDuint32 value from the 'read_addr'.
// Global resource dependence: 
// Author: hongwei xu
// Note:  The 'read_addr' must be devided exactly by 4.
/******************************************************************************/
PUBLIC KDboolean LCMIF_Read(// 
                   KDuint32 *read_data,//save the data read from 'read_addr'
                   KDuint32 read_addr// the addr of the value want to read
                   )
{
    /* read the data from the LCMIF */
    if((LCMIF_Read_Data((KDuint8*)read_data, sizeof(KDuint32), read_addr)) != sizeof(KDuint32))
    {
        KD_TRACE_LOW("LCMIF_Read() faile\n");
        return KD_FALSE;
    }
    return KD_TRUE;
}

/******************************************************************************/
// Description:    Write a KDuint32 value 'write_data' to 'write_addr'.
// Global resource dependence: 
// Author:  hongwei xu
// Note: The 'write_addr' must be devided exactly by 2.
/******************************************************************************/
PUBLIC KDboolean LCMIF_Write(// 
                    KDuint32 write_data,// the data want to write out
                    KDuint32 write_addr// the addr used to save the 'write_data'
                    )
{
    if(LCMIF_Write_Data ((KDuint8 *)&write_data, sizeof(KDuint32), write_addr) != sizeof(KDuint32))
    {
        KD_TRACE_LOW("LCMIF_Write() faile\n");
        return KD_FALSE;
    }
    return KD_TRUE;
}

/******************************************************************************/
// Description:    Test the RS status is '1' or not. Return KD_TRUE if success,
//               else KD_FALSE.
// Global resource dependence: 
// Author:  hongwei xu
// Note: 
/******************************************************************************/
LOCAL KDboolean Test_RS(void)
{
    KDuint32 i = 0;
    KDuint32 ret = 0;

    for(i=0; i<LCMIF_TEST_RS_TIMES; i++)
    {
        ret = LCMUD_Read_CMD();

        //if( 1 == (KDuint16)LCMIF_READ_VALUE(LCMIF_RDDATA))//RS==1
        if ( 0xFFFF == ret )
        {
            return KD_TRUE;
        }
        if(i == (LCMIF_TEST_RS_TIMES - 1) )
        {//error occurs and no data read
            KD_TRACE_LOW("some error occurs during checking the RS status\n");
            return KD_FALSE;
        }
    }
    return KD_TRUE;
}

/******************************************************************************/
// Description:  Set addr cs_index=0~5
//                            cd_index=0~5
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
PUBLIC void LCMIF_SetAddress(KDuint16 cs_index,KDuint16 cd_index)
{
    //s_lcmif_addr.cmd_addr = LCM_DATA_ADDR(cs_index,cd_index);
    //s_lcmif_addr.data_addr = LCM_CMD_ADDR(cs_index,cd_index);
}

/******************************************************************************/
// Description:  Test lcm interface
// Global resource dependence: 
// Author: hongwei xu
// Note:
/******************************************************************************/
void LCMIF_Test(void)
{
    KDuint32 i = 0, j = 0;
    KDuint32 test_len = 100;
    KDuint16 data_buf[100] = {0};
    KDuint16 read_buf[100] = {0};
    KDuint32 start_address = 0x10000400;
    KDuint8   d8_buf[200] = {0};
    KDuint8   r8_buf[200] = {0};
    
    for ( i = 0;i<test_len; i ++ )
    {
        data_buf[i]= 0xabcd;
    }

    for ( i = 1; i < test_len;i+=2 )
    {
        LCMIF_Write_Data((KDuint8*)data_buf, i*sizeof(KDuint16), start_address);
        LCMIF_Read_Data((KDuint8*)read_buf, i*sizeof(KDuint16), start_address);
        for(j = 0 ; j < i ; j ++ )
        {
            if ( data_buf[j] != read_buf[j] )
            {
                KD_TRACE_LOW("Test error j=%d,i=%d",j,i);
                kdAssert(0);  /*lint !e506*/
            }
        }
        kdMemset((KDuint8*)&read_buf,0,test_len*sizeof(KDuint16));
    }

     KD_TRACE_LOW("Test even ok");    

    kdMemcpy((KDuint8*)d8_buf,(KDuint8*)data_buf,200);
    for ( i = 1; i < test_len;i+=1 )
    {
        LCMIF_Read_Data(r8_buf, i*sizeof(KDuint8), start_address);
        for(j = 0 ; j < i ; j ++ )
        {
            if ( (KDuint8*)d8_buf[j] != (KDuint8*)r8_buf[j] )
            {
                KD_TRACE_LOW("Test odd error j=%d,i=%d",j,i);
                kdAssert(0); /*lint !e506*/
            }
        }
        kdMemset((KDuint8*)&r8_buf,0,test_len*sizeof(KDuint8));
    }     
     KD_TRACE_LOW("Test odd ok");        
    
}

#ifdef   __cplusplus
}
#endif

