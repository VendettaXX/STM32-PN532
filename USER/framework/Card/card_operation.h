#ifndef __CARD_OPERATION_H__
#define __CARD_OPERATION_H__

#include "bsp_config.h"
#include "pn532_mifare.h"
#include "pn532.h"

//���⺯��
//�쿨
bool check_card(void);
//дˢ��ʱ��
int write_touch_card_time(int *time);
//������ˢ��ʱ��
int read_all_touch_card_time(void);
//���ÿ�
void reset_card(void);
#endif