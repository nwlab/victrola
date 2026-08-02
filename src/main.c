/**
  ******************************************************************************
  * @file    main.c
  * @author  Iakovenko Andrii
  * @version V1.0.0
  * @date    1-August-2026
  * @brief   This file contains the main function for GPIO Toggle example.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"    /* SDCC patch: required by SDCC for interrupts */
#include "stm8s_tim4.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */

#define GPIO_OUT_PORT  (GPIOD)
#define GPIO_IN_PORT   (GPIOD)
#define LED_BLUE_GPIO_PIN  (GPIO_PIN_6)
#define LED_RED_GPIO_PIN  (GPIO_PIN_5)
#define AMP_ON_GPIO_PIN  (GPIO_PIN_4)
#define POWER_ON_GPIO_PIN  (GPIO_PIN_3)
#define POWER_GOOD_GPIO_PIN  (GPIO_PIN_2)
#define GPIO_OUT_PINS  (LED_BLUE_GPIO_PIN | LED_RED_GPIO_PIN | AMP_ON_GPIO_PIN | POWER_ON_GPIO_PIN)
#define GPIO_IN_PINS  (POWER_GOOD_GPIO_PIN)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t g_ms_ticks = 0;

/* Private function prototypes -----------------------------------------------*/
void delay_ms(uint16_t ms);

/* Private functions ---------------------------------------------------------*/
static void init_delay_timer(void)
{
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  enableInterrupts();
  TIM4_Cmd(ENABLE);
}

static void turn_table_on(bool on)
{
  if (on)
  {
    GPIO_WriteHigh(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)POWER_ON_GPIO_PIN);
  }
  else
  {
    GPIO_WriteLow(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)POWER_ON_GPIO_PIN);
  }
}

static void amp_table_on(bool on)
{
  if (on)
  {
    GPIO_WriteHigh(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)AMP_ON_GPIO_PIN);
  }
  else
  {
    GPIO_WriteLow(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)AMP_ON_GPIO_PIN);
  }
}

static bool is_power_good(void)
{
  return !(GPIO_ReadInputPin(GPIO_IN_PORT, (GPIO_Pin_TypeDef)POWER_GOOD_GPIO_PIN) == RESET);
}

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  /* Initialize internal clock */
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

  init_delay_timer();

  /* Initialize I/Os in Output Mode */
  GPIO_Init(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)GPIO_OUT_PINS, GPIO_MODE_OUT_PP_LOW_FAST);

  /* Initialize I/Os in Input Mode */
  GPIO_Init(GPIO_IN_PORT, (GPIO_Pin_TypeDef)GPIO_IN_PINS, GPIO_MODE_IN_FL_NO_IT);

  GPIO_WriteHigh(GPIO_OUT_PORT, (GPIO_Pin_TypeDef)LED_RED_GPIO_PIN);

  turn_table_on(TRUE);

  while (1)
  {
    if (is_power_good())
    {
        amp_table_on(TRUE);
    }
    else
    {
        amp_table_on(FALSE);
    }
    // delay_ms(500);
  }

}

/**
  * @brief Delay for the requested number of milliseconds using TIM4.
  * @param ms Milliseconds to wait.
  * @retval None
  */
void delay_ms(uint16_t ms)
{
  uint32_t start_tick = g_ms_ticks;

  while ((g_ms_ticks - start_tick) < ms)
  {
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
