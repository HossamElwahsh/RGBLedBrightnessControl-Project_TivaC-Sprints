/**
 * @file     
 * @brief    
 * @author   Alaa Hisham
 * @date     June 22, 2023
 * @version  0.1
 */

#include "pwm_linking_cfg.h"

const st_pwm_signal_cfg_t arr_gl_st_signal_cfg[PWM_MAX_CHANNELS]
	= {
			{{{(en_pwm_port_t)GPIO_PORT_F, (en_pwm_pin_t)GPIO_PIN_1}, {NULL}, {NULL}}},
			{{{(en_pwm_port_t)GPIO_PORT_F, (en_pwm_pin_t)GPIO_PIN_2}, {NULL}, {NULL}}},
			{{{(en_pwm_port_t)GPIO_PORT_F, (en_pwm_pin_t)GPIO_PIN_3}, {NULL}, {NULL}}},
			{{{NULL}, {NULL}, {NULL}}}
		};