/**
 * @file     
 * @brief    
 * @author   Alaa Hisham
 * @date     June 22, 2023
 * @version  0.1
 */


#ifndef PWM_LINKING_CFG_H_
#define PWM_LINKING_CFG_H_

/*----------------------------------------------------------/
/- Includes
/----------------------------------------------------------*/
#include "gpio_interface.h"

/*----------------------------------------------------------/
/- Macros
/----------------------------------------------------------*/
#define PWM_MAX_CHANNELS				4
#define PWM_MAX_PINS_PER_CHANNEL		3

/*----------------------------------------------------------/
/- ENUMS
/----------------------------------------------------------*/
typedef enum
{
    PWM_PORT_A	=	0	,
    PWM_PORT_B			,
    PWM_PORT_C			,
    PWM_PORT_D			,
	PWM_PORT_E			,
	PWM_PORT_F			,
    PWM_PORT_TOTAL
}en_pwm_port_t;


typedef enum{
    PWM_PIN_0	=	0	,
    PWM_PIN_1			,
    PWM_PIN_2			,
    PWM_PIN_3			,
    PWM_PIN_4			,
    PWM_PIN_5			,
    PWM_PIN_6			,
    PWM_PIN_7			,
    PWM_PIN_TOTAL
}en_pwm_pin_t;

typedef struct
{
	en_pwm_port_t port;
	en_pwm_pin_t  pin ;
}st_pwm_pin_t;
	

typedef struct
{
	st_pwm_pin_t  pins_per_channel[PWM_MAX_PINS_PER_CHANNEL];	
}st_pwm_signal_cfg_t;

extern const st_pwm_signal_cfg_t arr_gl_st_signal_cfg[PWM_MAX_CHANNELS];


#endif
