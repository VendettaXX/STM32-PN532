#include "log.h"
#include "hal_uart.h"
/******************************************************************************
 * @brief printf��ӡ�ض���
 *****************************************************************************/
PUTCHAR_PROTOTYPE
{
    //Place your implementation of fputc here , e.g. write a character to the USART
    USART_SendData(USART1,(u8)ch);
    //Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}
