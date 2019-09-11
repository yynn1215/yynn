/*
 * hcsr04.h
 *
 *  Created on: 2019. 9. 2.
 *      Author: bitcamp
 */

#ifndef HCSR04_H_
#define HCSR04_H_

#include "stm32f4xx.h"
//#include "defines.h"
#include "tm_stm32_delay.h"
#include "tm_stm32f4_gpio.h"

/**
 * @defgroup TM_HCSR04_Macros
 * @brief    Library macros
 * @{
 */

/* Default timeout pulses */
#ifndef HCSR04_TIMEOUT
#define HCSR04_TIMEOUT			1000000
#endif

/**
 * @brief  Time in microseconds to centimeters conversion
 */
#define HCSR04_NUMBER			((float)0.0171821)

/**
 * @}
 */

/**
 * @defgroup TM_HCSR04_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  HC-SR04 working structure
 */
typedef struct {
	float Distance;              /*!< Distance measured from sensor in centimeters */
	GPIO_TypeDef* ECHO_GPIOx;    /*!< Pointer to GPIOx PORT for ECHO pin. Meant for private use only */
	uint16_t ECHO_GPIO_Pin;      /*!< GPIO Pin for ECHO pin. Meant for private use only */
	GPIO_TypeDef* TRIGGER_GPIOx; /*!< Pointer to GPIOx PORT for TRIGGER pin. Meant for private use only */
	uint16_t TRIGGER_GPIO_Pin;   /*!< GPIO Pin for ECHO pin. Meant for private use only */
} TM_HCSR04_t;

/**
 * @}
 */

/**
 * @defgroup TM_HCSR04_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes HC-SR04 sensor
 * @param  *HCSR04: Pointer to empty @ref TM_HCSR04_t structure to save initialization data
 * @param  *ECHO_GPIOx: Pointer to GPIOx PORT for ECHO pin
 * @param  ECHO_GPIO_Pin: GPIO Pin for ECHO pin
 * @param  *TRIGGER_GPIOx: Pointer to GPIOx PORT for TRIGGER pin
 * @param  TRIGGER_GPIO_Pin: GPIO Pin for ECHO pin
 * @retval HC-SR04 status:
 *            - 0: Device not detected
 *            - > 0: Device is ready to use
 */
uint8_t TM_HCSR04_Init(TM_HCSR04_t* HCSR04, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin);

/**
 * @brief  Starts sensor measurement and read it's data
 * @param  *HCSR04: Pointer to @ref TM_HCSR04_t structure to save initialization data
 * @retval Distance in float:
 *            - > 0: Valid distance in cm (centimeters)
 *            -  -1: Error
 */
float TM_HCSR04_Read(TM_HCSR04_t* HCSR04);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* HCSR04_H_ */
