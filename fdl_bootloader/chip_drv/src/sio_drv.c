//#include "fdl_stdio.h"
#include "os_api.h"
#include "sio_drv.h"
//#include "stdarg.h"
//#include "fdl_crc.h"

//#include "packet.h"
#include "common.h"
#include "fdl_channel.h"

#if defined(PLATFORM_SC6800H)
#define GR_CTRL_REG        0x8b000004
/* GEN0_UART0_EN    (0x1 << 1) */
/* GEN0_UART1_EN    (0x1 << 2) */
#define GR_UART_CTRL_EN    (0x3<<1)
#elif defined(PLATFORM_SC8800G)
#define GR_CTRL_REG        0x8b000004
/* GEN0_UART0_EN    (0x1 << 20) */
/* GEN0_UART1_EN    (0x1 << 21) */
#define GR_UART_CTRL_EN    (0x3 << 20 )
#else
#define GR_CTRL_REG        0x8b000018
#define GR_UART_CTRL_EN    0x00400000
#endif

#define ARM_APB_CLK    26000000UL

typedef struct UartPort
{
    unsigned int regBase;
    unsigned int baudRate;
} UartPort_T;

LOCAL UartPort_T gUart0PortInfo =
{
    0x83000000,
    115200
};
LOCAL UartPort_T gUart1PortInfo =
{
    0x84000000,
    115200
};

LOCAL unsigned int SIO_GetHwDivider (unsigned int baudrate)
{
    return (unsigned int) ( (ARM_APB_CLK + baudrate / 2) / baudrate);
}
LOCAL void SIO_HwOpen (struct FDL_ChannelHandler *channel, unsigned int divider)
{
    UartPort_T *port  = (UartPort_T *) channel->priv;

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UIX8910) 
	REG32(APB_CLK_CFG1) |= CLK_UART1_SEL_MCU;
#endif

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)
    * (volatile unsigned int *) (APB_EB0_CLR) = UART0_EB_CLR | UART1_EB_CLR;
#else
    /* Disable UART*/
    * ( (volatile unsigned int *) (GR_CTRL_REG)) &= ~ (GR_UART_CTRL_EN);
#endif

    /*Disable Interrupt */
    * (volatile unsigned int *) (port->regBase + ARM_UART_IEN) = 0;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)
    * (volatile unsigned int *) (APB_EB0_SET) = UART0_EB_SET | UART1_EB_SET;
#else
    /* Enable UART*/
    * (volatile unsigned int *) GR_CTRL_REG |= (GR_UART_CTRL_EN);
#endif

    /* Set baud rate  */
    * (volatile unsigned int *) (port->regBase + ARM_UART_CLKD0) = LWORD (divider);
    * (volatile unsigned int *) (port->regBase + ARM_UART_CLKD1) = HWORD (divider);


    /* Set port for 8 bit, one stop, no parity  */
    * (volatile unsigned int *) (port->regBase + ARM_UART_CTL0) = UARTCTL_BL8BITS | UARTCTL_SL1BITS;
    * (volatile unsigned int *) (port->regBase+ ARM_UART_CTL1) = 0;
    * (volatile unsigned int *) (port->regBase + ARM_UART_CTL2) = 0;
}


LOCAL int SIO_Open (struct FDL_ChannelHandler  *channel, unsigned int baudrate)
{
    unsigned int divider;
#if 0 // sc6530 cannot check this reg while UART is disable, it will TIMEOUT error    
    unsigned int i = 0;
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while (!SIO_TRANS_OVER (port->regBase)) /* Wait until all characters are sent out */
    {
        i++;

        if (i >= UART_SET_BAUDRATE_TIMEOUT)
        {
            return -1;
        }
    }
#endif
    divider = SIO_GetHwDivider (baudrate);

    SIO_HwOpen (channel, divider);
    return 0;

}
LOCAL int SIO_Read (struct FDL_ChannelHandler  *channel, const unsigned char *buf, unsigned int len)
{
    unsigned char *pstart = (unsigned char *) buf;
    const unsigned char *pend = pstart + len;
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while ( (pstart < pend)
            && SIO_RX_READY (SIO_GET_RX_STATUS (port->regBase)))
    {
        *pstart++ = SIO_GET_CHAR (port->regBase);
    }

    return pstart - (unsigned char *) buf;
}
LOCAL char SIO_GetChar (struct FDL_ChannelHandler  *channel)
{
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while (!SIO_RX_READY (SIO_GET_RX_STATUS (port->regBase)))
    {

    }

    return SIO_GET_CHAR (port->regBase);
}
LOCAL int SIO_GetSingleChar (struct FDL_ChannelHandler  *channel)
{
    UartPort_T *port  = (UartPort_T *) channel->priv;
    char ch;

    if (!SIO_RX_READY (SIO_GET_RX_STATUS (port->regBase)))
    {
        return -1;

    }
    else
    {
        ch  = SIO_GET_CHAR (port->regBase);
    }

    return ch;
}
LOCAL int SIO_Write (struct FDL_ChannelHandler  *channel, const unsigned char *buf, unsigned int len)
{
    const unsigned char *pstart = (const unsigned char *) buf;
    const unsigned char *pend = pstart + len;
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while (pstart < pend)
    {
        /* Check if tx port is ready.*/
        /*lint -save -e506 -e731*/
        while (!SIO_TX_READY (SIO_GET_TX_STATUS (port->regBase)))
        {

            /* Do nothing */
        }

        SIO_PUT_CHAR (port->regBase, *pstart);
        ++pstart;
    }

    /* Ensure the last byte is written successfully */
    while (!SIO_TX_READY (SIO_GET_TX_STATUS (port->regBase)))
    {
        /* Do nothing */
    }

    return pstart - (const unsigned char *) buf;
}

LOCAL int SIO_PutChar (struct FDL_ChannelHandler  *channel, const unsigned char ch)
{
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while (!SIO_TX_READY (SIO_GET_TX_STATUS (port->regBase)))
    {
        /* Do nothing */
    }

    SIO_PUT_CHAR (port->regBase, ch);

    /* Ensure the last byte is written successfully */
    while (!SIO_TX_READY (SIO_GET_TX_STATUS (port->regBase)))
    {
        /* Do nothing */
    }

    return 0;
}
LOCAL int SIO_SetBaudrate (struct FDL_ChannelHandler  *channel,  unsigned int baudrate)
{
    unsigned int i = 0;
    UartPort_T *port  = (UartPort_T *) channel->priv;

    while (!SIO_TRANS_OVER (port->regBase)) /* Wait until all characters are sent out */
    {
        i++;

        if (i >= UART_SET_BAUDRATE_TIMEOUT)
        {
            return -1;
        }
    }
    channel->Open (channel, baudrate);
    return 0;
}
LOCAL int SIO_Close (struct FDL_ChannelHandler  *channel)
{
    return 0;
}
struct FDL_ChannelHandler gUart0Channel =
{
    SIO_Open,
    SIO_Read,
    SIO_GetChar,
    SIO_GetSingleChar,
    SIO_Write,
    SIO_PutChar,
    SIO_SetBaudrate,
    SIO_Close,
    &gUart0PortInfo
};
struct FDL_ChannelHandler gUart1Channel =
{
    SIO_Open,
    SIO_Read,
    SIO_GetChar,
    SIO_GetSingleChar,
    SIO_Write,
    SIO_PutChar,
    SIO_SetBaudrate,
    SIO_Close,
    &gUart1PortInfo
};


#ifdef DEBUG_TRACE
void sio_trace( char* str, uint32 len )
{
    uint32    i = 0;
     while(i++ < len)
    {
#if defined(PLATFORM_UIX8910)
        serial_putc((const unsigned char)*str++);
#else
#if 1//UART1 as default
        SIO_PutChar(&gUart1Channel, (const unsigned char)*str++);
#else//UART0
        SIO_PutChar(&gUart0Channel, (const unsigned char)*str++);
#endif
#endif
    }
}

uint32 SIO_TraceLow(const char *x_format, ...)
{
    char       format_str[MAX_STR_SIZE + 1];
	va_list    args;
	int        nBuf;
    va_start(args, x_format);
    nBuf = _vsnprintf(format_str,MAX_STR_SIZE,x_format, args);
    va_end(args);
    
    if(nBuf > 0)
    {
      format_str[nBuf] = 0;
      sio_trace(format_str, nBuf);
    }
   
    return 0;
}

#else
void sio_trace (char *msg)
{
}

#endif
