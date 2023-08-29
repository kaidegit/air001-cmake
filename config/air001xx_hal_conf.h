// https://wiki.luatos.com/chips/air001/Air001-MDK.html
// 可根据st相关文件改写

#ifndef AIR001_CMAKE_AIR001XX_HAL_CONF_H
#define AIR001_CMAKE_AIR001XX_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#define HAL_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLEDZ
#define HAL_CORTEX_MODULE_ENABLED

#define HSE_VALUE            24000000U
#define HSE_STARTUP_TIMEOUT  100U
#define HSI_VALUE            8000000U
#define HSI_STARTUP_TIMEOUT  5000U
#define LSI_VALUE            32000U
#define LSE_VALUE            32768U    /*!< Value of the External Low Speed oscillator in Hz */
#define LSE_STARTUP_TIMEOUT  5000U     /*!< Time out for LSE start up, in ms */
#define  TICK_INT_PRIORITY            0x00U /*!< tick interrupt priority */


#include "air001xx_hal_rcc.h"
#include "air001xx_hal_gpio.h"
#include "air001xx_hal_dma.h"
#include "air001xx_hal_flash.h"
#include "air001xx_hal_pwr.h"
#include "air001xx_hal_rtc.h"
#include "air001xx_hal_cortex.h"

#define assert_param(expr) ((void)0U)

#endif
