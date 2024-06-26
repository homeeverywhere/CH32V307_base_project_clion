/********************************** (C) COPYRIGHT *******************************
* File Name          : system_ch32v30x.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : CH32V30x Device Peripheral Access Layer System Source File.
*                      For HSE = 8Mhz
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*********************************************************************************/
#include "ch32v30x.h" 
#include "main.h"
/* 
* Uncomment the line corresponding to the desired System clock (SYSCLK) frequency (after 
* reset the HSI is used as SYSCLK source).
* If none of the define below is enabled, the HSI is used as System clock source. 
*/
 #define SYSCLK_FREQ_HSE    HSE_VALUE

  uint32_t SystemCoreClock         = HSI_VALUE;                /* System Clock Frequency (Core Clock) */

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

static void SetSysClock(void);
//static void SetSysClockToHSE(void);
static void SetSysClockTo144(void);
void printf_init(void);


/*********************************************************************
 * @fn      SystemInit
 *
 * @brief   Setup the microcontroller system Initialize the Embedded Flash Interface,
 *        the PLL and update the SystemCoreClock variable.
 *
 * @return  none
 */
void SystemInit (void)
{
  RCC->CTLR |= (uint32_t)0x00000001;

  RCC->CFGR0 &= (uint32_t)0xF8FF0000;

  RCC->CTLR &= (uint32_t)0xFEF6FFFF;
  RCC->CTLR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR0 &= (uint32_t)0xFF80FFFF;

  RCC->CTLR &= (uint32_t)0xEBFFFFFF;
  RCC->INTR = 0x00FF0000;
  RCC->CFGR2 = 0x00000000;

  SetSysClock();
}

/*********************************************************************
 * @fn      SystemCoreClockUpdate
 *
 * @brief   Update SystemCoreClock variable according to Clock Register Values.
 *
 * @return  none
 */
void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, Pll_6_5 = 0;

  tmp = RCC->CFGR0 & RCC_SWS;
  
  switch (tmp)
  {
    case 0x00:
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08: 
      pllmull = RCC->CFGR0 & RCC_PLLMULL;
      pllsource = RCC->CFGR0 & RCC_PLLSRC; 
      pllmull = ( pllmull >> 18) + 2;

#ifdef CH32V30x_D8
          if(pllmull == 17) pllmull = 18;
#else
          if(pllmull == 2) pllmull = 18;
          if(pllmull == 15){
              pllmull = 13;  /* *6.5 */
              Pll_6_5 = 1;
          }
          if(pllmull == 16) pllmull = 15;
          if(pllmull == 17) pllmull = 16;
#endif

      if (pllsource == 0x00)
      {
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {    
        if ((RCC->CFGR0 & RCC_PLLXTPRE) != (uint32_t)RESET)
        {
          SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          SystemCoreClock = HSE_VALUE * pllmull;
        }
      }

      if(Pll_6_5 == 1) SystemCoreClock = (SystemCoreClock / 2);

      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
 
  tmp = AHBPrescTable[((RCC->CFGR0 & RCC_HPRE) >> 4)];
  SystemCoreClock >>= tmp;  
}

/*********************************************************************
 * @fn      SetSysClock
 *
 * @brief   Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClock(void)
{
//  SetSysClockToHSE();
  SetSysClockTo144();
  printf_init();
}

/*********************************************************************
 * @fn      SetSysClockToHSE
 *
 * @brief   Sets HSE as System clock source and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
#if 0
static void SetSysClockToHSE(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
   
  RCC->CTLR |= ((uint32_t)RCC_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CTLR & RCC_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CTLR & RCC_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  

  if (HSEStatus == (uint32_t)0x01)
  {
    /* HCLK = SYSCLK */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;      
    /* PCLK2 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
    /* PCLK1 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV1;
    
    /* Select HSE as system clock source */
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
    RCC->CFGR0 |= (uint32_t)RCC_SW_HSE;    

    /* Wait till HSE is used as system clock source */
    while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x04)
    {
    }
  }
  else
  { 
        /* If HSE fails to start-up, the application will have wrong clock
     * configuration. User can add here some code to deal with this error 
         */
  }  
}
#endif

void printf_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = PRINT_RATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART_PRINT, &USART_InitStructure);
    USART_Cmd(USART_PRINT, ENABLE);
}

static void SetSysClockTo144(void)
	{
	__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

	RCC->CTLR |= ((uint32_t) RCC_HSEON);

	/* Wait till HSE is ready and if Time out is reached exit */
	do
		{
		HSEStatus = RCC->CTLR & RCC_HSERDY;
		StartUpCounter++;
		}
	while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

	if ((RCC->CTLR & RCC_HSERDY) != RESET)
		{
		HSEStatus = (uint32_t) 0x01;
		}
	else
		{
		HSEStatus = (uint32_t) 0x00;
		}

	if (HSEStatus == (uint32_t) 0x01)
		{
		/* HCLK = SYSCLK */
		RCC->CFGR0 |= (uint32_t) RCC_HPRE_DIV1;
		/* PCLK2 = HCLK */
		RCC->CFGR0 |= (uint32_t) RCC_PPRE2_DIV1;
		/* PCLK1 = HCLK */
		RCC->CFGR0 |= (uint32_t) RCC_PPRE1_DIV2;

		/*  PLL configuration: PLLCLK = HSE * 18 = 144 MHz */
		RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_PLLSRC | RCC_PLLXTPRE |
		RCC_PLLMULL));

#ifdef CH32V30x_D8
		RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE | RCC_PLLMULL18);
#else
		RCC->CFGR0 |= (uint32_t) (RCC_PLLSRC_HSE | RCC_PLLXTPRE_HSE
		        | RCC_PLLMULL18_EXTEN);
#endif

		/* Enable PLL */
		RCC->CTLR |= RCC_PLLON;
		/* Wait till PLL is ready */
		while((RCC->CTLR & RCC_PLLRDY) == 0)
			{
			}
		/* Select PLL as system clock source */
		RCC->CFGR0 &= (uint32_t) ((uint32_t) ~(RCC_SW));
		RCC->CFGR0 |= (uint32_t) RCC_SW_PLL;
		/* Wait till PLL is used as system clock source */
		while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
			{
			}
		}
	else
		{
		/*
		 * If HSE fails to start-up, the application will have wrong clock
		 * configuration. User can add here some code to deal with this error
		 */
		}
	}

