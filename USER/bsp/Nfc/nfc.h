#ifndef __NFC_H__
#define __NFC_H__
#include "stm32f10x.h"

void nfc_InListPassiveTarget(void);      //���� 106 kbps type A (ISO/IEC14443 Type A),
void nfc_GetUID(void);                   //��ȡ��ǩ UID
void nfc_PsdVerifyKeyA(void);            // ������Ȩ����֤KeyA
void nfc_read(void);                     //Ĭ�� �� 02����16���ֽ�
uint8_t *nfc_GetReadData(void);          //��ȡ��ǩ ����ֵ  Ĭ�ϵڶ��飨16�ֽ����ݣ�
void  nfc_write(uint8_t data);           //Ĭ���� 02��д 16���ֽڵĵ�һ���ֽ�
void nfc_WakeUp(void);                   //����
void CleanBuffer(u16 num);               //���� ǰ ���ٸ��ֽڵ�����


void TIM2_Configuration(void);
#endif /* __NFC_H__ */
