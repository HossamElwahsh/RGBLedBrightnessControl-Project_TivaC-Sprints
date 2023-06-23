/**
 * @file     pwm_interface.h
 * @brief    
 * @author   Alaa Hisham
 * @date     June 22, 2023
 * @version  0.1
 */


#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

/*----------------------------------------------------------/
/- Includes
/----------------------------------------------------------*/
#include "std.h"

/*----------------------------------------------------------/
/- ENUMS
/----------------------------------------------------------*/

typedef enum
{
	PWM_OK 				= 0	,
	PWM_INVALID_CHANNEL		,
	PWM_ERROR
}en_pwm_error_t;

typedef enum
{
	PWM_CHANNEL_1			= 0 ,
	PWM_CHANNEL_2					,
	PWM_CHANNEL_3         ,
	PWM_CHANNEL_4         ,
	PWM_CHANNEL_TOTAL
}en_pwm_channel_id_t;

/*----------------------------------------------------------/
/- Function Prototypes
/----------------------------------------------------------*/
/**
 * @brief Initializes pwm module with defined configurations
 * 
 * @return PWM_OK    : Operation is done successfully
 *		   PWM_ERROR : Operation Failed
 */
en_pwm_error_t pwm_init(void);

/**
 * @brief Adjust the signal parameters (period and duty cycle)
 * 
 * @param[in] en_a_channel_id  : PWM channel to generate the signal on
 * @param[in] u8_a_dutyCycle   : duty cycle of the PWM signal (0 - 100)
 * @param[in] u16_a_msPeriod   : the period of the PWM signal
 * 
 * @return PWM_OK			   : Operation is done successfully   
 *		   PWM_INVALID_CHANNEL : When the passed channel is not supported
 * 		   PWM_ERROR		   : When the duty cycle is invalid (>100)
 */
en_pwm_error_t pwm_adjust_signal(en_pwm_channel_id_t en_a_channel_id, uint8_t_ u8_a_dutyCycle, uint16_t_ u16_a_msPeriod);

/**
 * @brief Start generating pwm signal on the given channel
 *
 * @param[in] en_a_channel_id  : PWM channel to generate the signal on
 *
 * @return PWM_OK			   : Operation is done successfully
 *		   PWM_INVALID_CHANNEL : When the passed channel is not supported
 * 		   PWM_ERROR		   : Operation failed
 */
en_pwm_error_t pwm_start(en_pwm_channel_id_t en_a_channel_id);

/**
 * @brief Stop generating a pwm signal on the given channel
 *
 * @param[in] en_a_channel_id  : PWM channel to stop
 *
 * @return PWM_OK			   : Operation is done successfully
 *		   PWM_INVALID_CHANNEL : When the passed channel is not supported
 * 		   PWM_ERROR		   : Operation failed
 */
en_pwm_error_t pwm_stop (en_pwm_channel_id_t en_a_channel_id);

#endif
