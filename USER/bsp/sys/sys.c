/*
 * Copyright (c) 2018 zhangyuhude@163.com
 * All Rights Reserved.
 */

#include "sys.h"

/******************************************************************************
 * @brief LED_Init
 *****************************************************************************/

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

}
