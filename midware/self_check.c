/*
 * self_check.c
 *
 *  Created on: Apr 15, 2022
 *      Author: OWNER
 */

#include "ch32v30x.h"
#include "self_check.h"
#include "main.h"
#include "system_interface_mamage.h"

#define core_mxl     (core_csr.misa&0xc0000000>>30)
#define Vender0   ((core_csr.marchid& 0x7c000000)>>26)
#define Vender1   ((core_csr.marchid& 0x03e00000)>>21)
#define Vender2   ((core_csr.marchid& 0x001f0000)>>16)

core_csr_t core_csr;/* 存放内部CSR */

void self_check(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    uint8_t i;

    NVIC_EnableIRQ(Software_IRQn);
    NVIC_SetPendingIRQ(Software_IRQn);
    NVIC_DisableIRQ(Software_IRQn);

    printf("\n\n末端采集和分析设备\n");
    printf("设备初始化……\n");
    printf("固件版本：V%04x，编译日期：%s,%s……\n",firm_vision,__DATE__,__TIME__);
    printf("主控型号：CH32V307VCT6.\n");
    printf("内核型号：%c%c%c-%c%d%c.\n",Vender0+64,Vender1+64,Vender2+64,((core_csr.marchid&(0x1f<<10))>>10)+64,((core_csr.marchid&(0x1f<<5))>>5),((core_csr.marchid&0x0000001f)>>0)+64 );
    printf("内核特征  位宽：%d 位,支持指令集：",(core_mxl == 1)?32:((core_mxl == 2)?64:(core_mxl == 3)?128:0));
    for(i=0;i<26;i++)
    {
        if(core_csr.misa & (1<<i))
            printf("%c ",65+i);
    }
    printf("\n");
    printf("存储配置：RAM:%d KB,FLASH:%d KB,其中零等待部分大小：%d KB.\n",(4-((FLASH->OBR&(3<<8))>>2))*32,*(uint16_t*)(0x1FFFF7E0),320-(4-((FLASH->OBR&(3<<8))>>2))*32 );
    RCC_GetClocksFreq(&RCC_Clocks);
    printf("当前主频:%d MHz.\n",  RCC_Clocks.SYSCLK_Frequency/1000000);
    printf("内核频率:%d MHz.\n",  RCC_Clocks.HCLK_Frequency  /1000000);
    printf("外设总线1:%d MHz.\n", RCC_Clocks.PCLK1_Frequency /1000000);
    printf("外设总线2:%d MHz.\n", RCC_Clocks.PCLK2_Frequency /1000000);
    printf("ADC频率:%d MHz.\n",   RCC_Clocks.ADCCLK_Frequency/1000000);
    printf("中断向量表在：0x%08x.\n",core_csr.mtvec);
    printf("main函数在：0x%08x.\n",main);

}

void check_rst_src(void)
{
    uint32_t flag = RCC->RSTSCKR;

    printf("固件开始运行，刚发生");
    if(flag & (1<<31) )printf("低功耗复位 ");
    if(flag & (1<<30) )printf("窗口开门狗复位 ");
    if(flag & (1<<29) )printf("独立开门狗复位 ");
    if(flag & (1<<28) )printf("软件复位 ");
    if(flag & (1<<27) )printf("掉电复位 ");
    if(flag & (1<<26) )printf("手动复位 ");
    printf("\n");
    RCC->RSTSCKR|=(1<<24);
}
