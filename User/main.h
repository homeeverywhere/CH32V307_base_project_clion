/*
 * main.h
 *
 *  Created on: Apr 14, 2022
 *      Author: OWNER
 */

#ifndef USER_MAIN_H_
#define USER_MAIN_H_

#define USART_PRINT USART1
#define PRINT_RATE  115200


typedef struct
{
    uint32_t marchid;/* 架构编号寄存器 */
    uint32_t misa;   /* 硬件指令集寄存器 */
    uint32_t mtvec;  /* 异常基地址寄存器 */
}core_csr_t;

#define firm_vision 0x01

int main(void);

#endif /* USER_MAIN_H_ */
