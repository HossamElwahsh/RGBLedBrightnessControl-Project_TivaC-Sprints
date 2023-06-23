/**
 ** @file      pwm_program.c
 ** @brief     
 ** @author    Alaa
 ** @date      June 22, 2023
 ** @version   0.1
 */

/*----------------------------------------------
- INCLUDES
----------------------------------------------*/
#include "gpio_interface.h"
#include "gpt_interface.h"
#include "pwm_linking_cfg.h"
#include "pwm_interface.h"

/*----------------------------------------------
- Private Macros
----------------------------------------------*/
#define PWM_GPT_CHANNEL				CH_2
#define PWM_MAX_DUTY_CYCLE			100
#define PWM_MAX_ACTIVE_CHANNEL		1

/*----------------------------------------------
- Enums
----------------------------------------------*/
typedef enum
{
	CHANNEL_DISABLED  = 0 ,
	CHANNEL_ENABLED 			
}en_pwm_channel_state_t;

typedef enum
{
	SIGNAL_OFF	= 0	,
	SIGNAL_ON
}en_pwm_signal_state_t;

/*----------------------------------------------
- Structs
----------------------------------------------*/
typedef struct
{
	en_pwm_channel_state_t channel_state;
	en_pwm_signal_state_t  signal_state	;
	uint16_t_			   on_time		;
	uint16_t_  			   off_time		;
}st_signal_state_t;

/*----------------------------------------------
- global variables
----------------------------------------------*/
static st_signal_state_t arr_gl_st_signal_state[PWM_MAX_CHANNELS];
static uint8_t_  u8_gl_active_channels = ZERO;
static en_pwm_channel_id_t en_gl_active_channel_index;

static void pwm_timer_cbf(void);
/*----------------------------------------------
- Function Implementation
----------------------------------------------*/
/**
 * @brief Initializes pwm module with defined configurations
 * 
 * @return PWM_OK    : Operation is done successfully
 *		   PWM_ERROR : Operation Failed
 */
en_pwm_error_t pwm_init(void)
{
	en_pwm_error_t en_lo_error_state = PWM_OK;
	uint8_t_ u8_lo_channel_iterator;
	uint8_t_ u8_lo_pin_iterator;
	
	en_lo_error_state = (en_pwm_error_t)gpt_init();
	
	if(PWM_OK != en_lo_error_state)
	{
		en_lo_error_state = PWM_ERROR;
	}
	else
	{
		for(u8_lo_channel_iterator=ZERO; u8_lo_channel_iterator<PWM_MAX_CHANNELS; u8_lo_channel_iterator++)
		{
			for(u8_lo_pin_iterator=ZERO; u8_lo_pin_iterator<PWM_MAX_PINS_PER_CHANNEL; u8_lo_pin_iterator++)
			{
				st_gpio_cfg_t st_lo_pwm_pin;
				
				st_lo_pwm_pin.port = (en_gpio_port_t)(arr_gl_st_signal_cfg[u8_lo_channel_iterator].pins_per_channel[u8_lo_pin_iterator].port);
				st_lo_pwm_pin.pin  = (en_gpio_pin_t)(arr_gl_st_signal_cfg[u8_lo_channel_iterator].pins_per_channel[u8_lo_pin_iterator].pin) ;
				st_lo_pwm_pin.pin_cfg = OUTPUT;
				st_lo_pwm_pin.current = PIN_CURRENT_2MA;
				
				en_lo_error_state = (en_pwm_error_t)gpio_pin_init(&st_lo_pwm_pin);

				if (PWM_OK != en_lo_error_state)
				{
					en_lo_error_state = PWM_ERROR;
					break;
				}
			}
			
			if (PWM_OK != en_lo_error_state) break;
		}
	}
	
	return en_lo_error_state;
}

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
en_pwm_error_t pwm_adjust_signal(en_pwm_channel_id_t en_a_channel_id, uint8_t_ u8_a_dutyCycle, uint16_t_ u16_a_msPeriod)
{
	//uint16_t_ u16_lo_onTime, u16_lo_offTime;
	en_pwm_error_t en_lo_error_state = PWM_OK;
	
	if(u8_a_dutyCycle <= PWM_MAX_DUTY_CYCLE)
	{
		arr_gl_st_signal_state[en_a_channel_id].on_time = (uint16_t_)((u8_a_dutyCycle / 100.0) * u16_a_msPeriod);
		arr_gl_st_signal_state[en_a_channel_id].off_time = (uint16_t_)(((100-u8_a_dutyCycle) / 100.0) * u16_a_msPeriod);
	}
	else
	{
		en_lo_error_state = PWM_ERROR;
	}
	
	return en_lo_error_state;
}

/**
 * @brief Start generating pwm signal on the given channel
 *
 * @param[in] en_a_channel_id  : PWM channel to generate the signal on
 *
 * @return PWM_OK			   : Operation is done successfully
 *		   PWM_INVALID_CHANNEL : When the passed channel is not supported
 * 		   PWM_ERROR		   : Operation failed
 */
en_pwm_error_t pwm_start(en_pwm_channel_id_t en_a_channel_id)
{
	en_pwm_error_t en_lo_error_state = PWM_OK;
	uint8_t_ u8_lo_pin_iterator;

	if ((u8_gl_active_channels < PWM_MAX_ACTIVE_CHANNEL)
		&&(en_a_channel_id < PWM_CHANNEL_TOTAL))
	{
		u8_gl_active_channels++;
		en_gl_active_channel_index = en_a_channel_id;
		arr_gl_st_signal_state[en_a_channel_id].channel_state = CHANNEL_ENABLED;
		
		if (ZERO == arr_gl_st_signal_state[en_a_channel_id].on_time)
		{
			/* Set all pins on the channel to low */
		}
		else
		{
			/* Set all pins on the channel to high */
			for (u8_lo_pin_iterator = ZERO; u8_lo_pin_iterator < PWM_MAX_PINS_PER_CHANNEL; u8_lo_pin_iterator++)
			{
				st_gpio_cfg_t st_lo_pwm_pin;

				//if (NULL == arr_gl_st_signal_cfg[en_a_channel_id].pins_per_channel[u8_lo_pin_iterator].port) continue;

				st_lo_pwm_pin.port = (en_gpio_port_t)(arr_gl_st_signal_cfg[en_a_channel_id].pins_per_channel[u8_lo_pin_iterator].port);
				st_lo_pwm_pin.pin = (en_gpio_pin_t)(arr_gl_st_signal_cfg[en_a_channel_id].pins_per_channel[u8_lo_pin_iterator].pin);

				en_lo_error_state = (en_pwm_error_t)gpio_setPinVal(st_lo_pwm_pin.port, st_lo_pwm_pin.pin, HIGH);

				if (PWM_OK != en_lo_error_state)
				{
					en_lo_error_state = PWM_ERROR;
					break;
				}
			}
			arr_gl_st_signal_state[en_gl_active_channel_index].signal_state = SIGNAL_ON;
			
			if ((ZERO == arr_gl_st_signal_state[en_a_channel_id].off_time) && (ZERO != arr_gl_st_signal_state[en_a_channel_id].on_time))
			{
				/* Do Nothing */
			}
			else
			{
				gpt_set_callback(PWM_GPT_CHANNEL, pwm_timer_cbf);
				en_lo_error_state = (en_pwm_error_t)gpt_start(PWM_GPT_CHANNEL, arr_gl_st_signal_state[en_a_channel_id].on_time, TIME_IN_MS);
			}
		}
	}
	else
	{
		en_lo_error_state = PWM_ERROR;
	}
	
	return en_lo_error_state;
}

/**
 * @brief Stop generating a pwm signal on the given channel
 *
 * @param[in] en_a_channel_id  : PWM channel to stop
 *
 * @return PWM_OK			   : Operation is done successfully
 *		   PWM_INVALID_CHANNEL : When the passed channel is not supported
 * 		   PWM_ERROR		   : Operation failed
 */
en_pwm_error_t pwm_stop(en_pwm_channel_id_t en_a_channel_id)
{
	en_pwm_error_t en_lo_error_state = PWM_OK;
	
	if(en_a_channel_id < PWM_CHANNEL_TOTAL)
	{
		if (CHANNEL_ENABLED == arr_gl_st_signal_state[en_a_channel_id].channel_state)
		{
			en_lo_error_state = (en_pwm_error_t)gpt_stop(PWM_GPT_CHANNEL);
	
			if(PWM_OK == en_lo_error_state)
			{
				/* Update the channel state */
				arr_gl_st_signal_state[en_a_channel_id].channel_state = CHANNEL_DISABLED;
				u8_gl_active_channels--;
			}
			else
			{
				/* Do Nothing */
			}
		}
		else
		{
			/* Do Nothing */
		}
	}
	else
	{
		en_lo_error_state = PWM_INVALID_CHANNEL;
	}
	
	return en_lo_error_state;
}

/**
 * @brief Function to call whenever the pwm timer channel fires
 * 				an interrupt
 */
static void pwm_timer_cbf(void)
{
	en_gpio_pin_level_t en_lo_pwm_pin_level;
	uint8_t_ u8_lo_pin_index;
	uint32_t_ u32_lo_delay;
	
	/* set the pin level and interval duration according to signal state */
	if (arr_gl_st_signal_state[en_gl_active_channel_index].signal_state == SIGNAL_ON)
	{
		en_lo_pwm_pin_level = LOW;
		u32_lo_delay = arr_gl_st_signal_state[en_gl_active_channel_index].off_time;
		arr_gl_st_signal_state[en_gl_active_channel_index].signal_state = SIGNAL_OFF;
	}
	else
	{
		en_lo_pwm_pin_level = HIGH;
		u32_lo_delay = arr_gl_st_signal_state[en_gl_active_channel_index].on_time;
		arr_gl_st_signal_state[en_gl_active_channel_index].signal_state = SIGNAL_ON;
	}

	/* Stop the timer */
	gpt_stop(PWM_GPT_CHANNEL);

	/* Set all the channel pins to the right level */
	for (u8_lo_pin_index = 0; u8_lo_pin_index < PWM_MAX_PINS_PER_CHANNEL; u8_lo_pin_index++)
	{
		gpio_setPinVal((en_gpio_port_t)(arr_gl_st_signal_cfg[en_gl_active_channel_index].pins_per_channel[u8_lo_pin_index].port),
					   (en_gpio_pin_t)(arr_gl_st_signal_cfg[en_gl_active_channel_index].pins_per_channel[u8_lo_pin_index].pin),
					   en_lo_pwm_pin_level);
	}

	/* Start timer again */
	gpt_start(PWM_GPT_CHANNEL, u32_lo_delay, TIME_IN_MS);
}
