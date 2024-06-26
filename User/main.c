/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/*
 *@Note


*/
#include "debug.h"
#include "string.h"
#include "self_check.h"
#include "main.h"

#define bsd_sys_clk 144000000

void Delay_Ms(uint16_t n);
void Delay_Us(uint16_t n);
void check_rst_src(void);

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    check_rst_src();
    self_check();

    printf("double 类型的字节数是 %u 字节.\n",sizeof(double));
    printf("Enter main loop.\n");
    while(1)
    {
        Delay_Ms(1000);
    }
}

void Delay_Us(uint16_t n)
{
    TIM3->CTLR1 &=~ TIM_CEN;
    TIM3->PSC = bsd_sys_clk/1000000;
    TIM3->ATRLR = n;
    TIM3->DMAINTENR |= 1;
    TIM3->CTLR1 |= TIM_CEN;
    while((TIM3->INTFR&0x0001) == 0);
    TIM3->INTFR = (uint16_t)~TIM_FLAG_Update;
    TIM3->CTLR1 &=~ TIM_CEN;
}

void Delay_Ms(uint16_t n)
{
    TIM3->CTLR1 &=~ TIM_CEN;
    TIM3->PSC = bsd_sys_clk/4000;
    TIM3->ATRLR = n*4;
    TIM3->DMAINTENR |= 1;
    TIM3->CTLR1 |= TIM_CEN;
    while((TIM3->INTFR&TIM_FLAG_Update) == 0);
    TIM3->INTFR = (uint16_t)~TIM_FLAG_Update;
    TIM3->CTLR1 &=~ TIM_CEN;
}