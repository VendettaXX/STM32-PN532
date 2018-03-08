#include "sys.h"
#include "usart.h"
#include "stm32f10x_tim.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
  int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit(int x)
{
  x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
  while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
    USART1->DR = (u8) ch;
  return ch;
}
#endif

/*ʹ��microLib�ķ���*/
 /*
int fputc(int ch, FILE *f)
{
  USART_SendData(USART1, (uint8_t) ch);

  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

    return ch;
}
int GetKey (void)  {

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/


//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��  ������ɱ�־
//bit14��  ���յ�0x0d
//bit13~0��  ���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���
u8 RxBuffer1[200];
u8 Uart_send_counter; //Uart_send()�������͵��ֽ���
vu8 *Uart_send_pointer;//���͵�����ָ��
u32 RxCounter1=0;
u8 flag_nfc_status;
u8 flag_rev_finish;  //1Ϊ�������
u8 UID_backup[4];//UID����  ���ڴ��� ������дͬһ��
NVIC_InitTypeDef NVIC_InitStructure;
//��ʼ��IO ����1
//bound:������
void uart_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��
    USART_DeInit(USART1);  //��λ����1
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

    //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

    //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;    //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
}

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //  TIM2 Configuration:���ϼ����ж�:

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 720;
    TIM_TimeBaseStructure.TIM_Prescaler = 10000-1;//
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);//���������ж�
    /* TIM2 enable counter */
    //TIM_Cmd(TIM2, ENABLE);//����ʱ�� ���ﲻ���� �ڽ��ܵ�����ʱ���� ���ж��ǲ���֡����

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void TIM2_IRQHandler(void)
{
   TIM_Cmd(TIM2, DISABLE);
   TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

   RxCounter1=0;
   flag_rev_finish=1;
  /*****�������  ���ؽ��ܵ����  ˳������жϷ���
         �����Ժ������ݴ�����***********************/

}
void USART1_IRQHandler(void)                  //����1�жϷ������
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        TIM_Cmd(TIM2, ENABLE);
        TIM2->CNT &= 0x0000;//��ʱ����ʱ����  ÿ�ν������� �����ܼ��������ʱ������ֵʱ  ����tim�ж�  ��Ϊһ֡�������

        RxBuffer1[RxCounter1++] = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
        Uart_send_counter--;
        if(Uart_send_counter>0)
        {
            USART_SendData(USART1, *Uart_send_pointer++);
        }
        else
        {
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        }
    }
}


