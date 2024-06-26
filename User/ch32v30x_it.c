/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main Interrupt Service Routines.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include "ch32v30x_it.h"

/* 硬件压栈只支持3个  */
void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void SW_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
    printf("suffered NMI.\n");
}

#include "main.h"

extern core_csr_t core_csr;/* 存放内部CSR */

void SW_Handler(void)
{
    NVIC_ClearPendingIRQ(Software_IRQn);
    core_csr.mtvec  = __get_MTVEC();
    core_csr.marchid  = __get_MARCHID();
    core_csr.misa  = __get_MISA();
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
uint32_t d_mepc=0,d_mcause=0,d_mtval=0;
void HardFault_Handler(void)
{
    asm("la a0, d_mepc");
    asm("csrr a1, mepc");
    asm("sw a1, 0(a0)");

    asm("la a0, d_mcause");
    asm("csrr a1, mcause");
    asm("sw a1, 0(a0)");

    asm("la a0, d_mtval");
    asm("csrr a1, mtval");
    asm("sw a1, 0(a0)");

    printf("mepc=%08x\r\n",d_mepc);
    printf("mcause=%08x\r\n",d_mcause);
    printf("mtval=%08x\r\n",d_mtval);
    printf("Error:A hardFault has occurred.Program has been blocked.\n");
    while(1);
}
