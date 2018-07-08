#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "sys.h"

//24L01������
#define NRF24L01_CE                 PAout(4) //24L01Ƭѡ�ź�
#define NRF24L01_CSN                PCout(4) //SPIƬѡ�ź�
#define NRF24L01_IRQ                PCin(5)  //IRQ������������
//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH                5   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH                5   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH              32  //20�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH              32  //20�ֽڵ��û����ݿ��


void NRF24L01_Init(void);//��ʼ��
void RX_Mode(void);//����Ϊ����ģʽ
void TX_Mode(void);//����Ϊ����ģʽ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);//д������
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);//��������
uint8_t NRF24L01_Read_Reg(uint8_t reg);           //���Ĵ���
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);//д�Ĵ���
uint8_t NRF24L01_Check(void);//���24L01�Ƿ����

uint8_t NRF24L01_TxPacket(uint8_t *txbuf);//����һ����������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);//����һ����������
#endif











