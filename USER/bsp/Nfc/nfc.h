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

/*
//����NFCָ��
PC->PN532:
    55 55 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF 03 FD D4 14 01 17 00
PN532->PC:
    00 00 FF 00 FF 00 00 00 FF 02 FE D5 15 16 00


һ�λ�ȡUIDָ��
PC->PN532:
    00 00 FF 04 FC D4 4A 01 00 E1 00
PN532->PC:
    00 00 FF 00 FF 00
PN532->PC:
    00 00 FF 0F F1 D5 4B 01 01 | 00 44 | 00 07 04 82 22 AA DC 39 80 AC 00
| 00 44 |  ˵����Ƭ������ MifareUltralight(MF0)
*/

#endif /* __NFC_H__ */
