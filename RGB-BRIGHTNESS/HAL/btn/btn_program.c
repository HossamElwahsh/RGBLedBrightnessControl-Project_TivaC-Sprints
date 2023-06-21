/*
* CFile1.c
*
* Created: 5/30/2023 2:24:25 PM
*  Author: Alaa
*/

/*----------------------------------------------------------/
/- INCLUDES 
/----------------------------------------------------------*/
#include "systick_interface.h"
#include "systick_linking_config.h"
#include "gpio_interface.h"

#include "btn_interface.h"

/*---------------------------------------------------------/
/- LOCAL MACROS
/---------------------------------------------------------*/
#define BTN_DEBOUNCE_DELAY			200 //in ms

/*---------------------------------------------------------/
/- FUNCTION IMPLEMENTATION
/---------------------------------------------------------*/
/**
* @brief Function to initialize a given button instance
*
* @param ptr_st_btn_config            : pointer to the desired button structure
*
* @return BTN_STATUS_OK 	            : When the operation is successful
*         BTN_STATUS_INVALID_STATE		: Button structure pointer is a NULL_PTR
*         BTN_STATUS_INVALID_PULL_TYPE : If the pull type field in button structure is set to invalid value
*/
en_btn_status_code_t_ btn_init(st_btn_config_t_* ptr_st_btn_config)
{
	en_btn_status_code_t_ lo_en_btn_status = BTN_STATUS_OK;
	st_gpio_cfg_t btn_pin_cfg;

	if (NULL_PTR != ptr_st_btn_config)
	{
		btn_pin_cfg.port = (en_gpio_port_t) ptr_st_btn_config->en_btn_port;
		btn_pin_cfg.pin  = (en_gpio_pin_t)  ptr_st_btn_config->en_btn_pin;
		
		switch (ptr_st_btn_config->en_btn_pull_type)
		{
			case BTN_INTERNAL_PULL_UP	 :
			case BTN_INTERNAL_PULL_DOWN: btn_pin_cfg.pin_cfg = (en_gpio_pin_cfg_t)
																	(INPUT_PULL_UP 
																	+ ptr_st_btn_config->en_btn_pull_type);
																	break;
			case BTN_EXTERNAL_PULL_UP  : 
			case BTN_EXTERNAL_PULL_DOWN: btn_pin_cfg.pin_cfg = INPUT; break;
			default : lo_en_btn_status = BTN_STATUS_INVALID_PULL_TYPE;
		}
		
		/* Initialize the button pin */
		gpio_pin_init(&btn_pin_cfg);
		
		systick_init(&gl_st_systick_cfg_0);
		
		/* Set the button state */
		ptr_st_btn_config->en_btn_activation = BTN_ACTIVATED;
	}
	else
	{
		lo_en_btn_status = BTN_STATUS_INVALID_STATE;
	}
	
	return lo_en_btn_status;
}

en_btn_status_code_t_ btn_disable(st_btn_config_t_* ptr_st_btn_config)
{
	en_btn_status_code_t_ lo_en_btn_status = BTN_STATUS_OK;
	
	if (NULL_PTR != ptr_st_btn_config)
	{
		if(ptr_st_btn_config->en_btn_activation == BTN_ACTIVATED)
		{
				ptr_st_btn_config->en_btn_activation = BTN_DEACTIVATED;
		}
	}
	else
	{
		lo_en_btn_status = BTN_STATUS_INVALID_STATE;
	}
	
	return lo_en_btn_status;
}

/**
* @brief Function to read the current button state
*
* @param ptr_st_btn_config            : pointer to the desired button structure
* @param ptr_en_btn_state             : pointer to variable to store the button state
*
* @return BTN_STATUS_OK                : When the operation is successful
*         BTN_STATUS_INVALID_STATE     : Button structure and/or button state pointers are NULL_PTRs
*         BTN_STATUS_INVALID_PULL_TYPE : If the pull type field in button structure is set to invalid value
*		  BTN_STATUS_DEACTIVATED	   : If we are trying to read from a deactivated button
*/
en_btn_status_code_t_ btn_read(st_btn_config_t_* ptr_st_btn_config, en_btn_state_t_* ptr_en_btn_state)
{
	en_btn_status_code_t_ lo_en_btn_status = BTN_STATUS_OK;
	en_gpio_pin_level_t lo_en_btn_val;

	if((ptr_st_btn_config != NULL_PTR) && (ptr_en_btn_state != NULL_PTR))
	{
		if(BTN_ACTIVATED == ptr_st_btn_config->en_btn_activation)
		{
			gpio_getPinVal((en_gpio_port_t) ptr_st_btn_config->en_btn_port,
										(en_gpio_pin_t)  ptr_st_btn_config->en_btn_pin ,
											&lo_en_btn_val);
	
			switch (ptr_st_btn_config->en_btn_pull_type)
			{
				case BTN_INTERNAL_PULL_UP:
				case BTN_EXTERNAL_PULL_UP:
				{
					*ptr_en_btn_state = (en_btn_state_t_)(!lo_en_btn_val);
					if(BTN_STATE_PRESSED == *ptr_en_btn_state)
					{
						systick_ms_delay(BTN_DEBOUNCE_DELAY);
					}
					break;
				}
				case BTN_INTERNAL_PULL_DOWN:
				case BTN_EXTERNAL_PULL_DOWN:
				{
					*ptr_en_btn_state = (en_btn_state_t_)lo_en_btn_val;
					if(BTN_STATE_PRESSED == *ptr_en_btn_state)
					{
						systick_ms_delay(BTN_DEBOUNCE_DELAY);
					}
					break;
				}				
				default : lo_en_btn_status = BTN_STATUS_INVALID_PULL_TYPE;
			}
		}
		else
		{
			lo_en_btn_status = BTN_STATUS_DEACTIVATED;
		}
	}
	else
	{
		lo_en_btn_status = BTN_STATUS_INVALID_STATE;
	}

	return lo_en_btn_status;
}

/**
* @brief Function to set a notification function when the given button is pressed
*
* @param ptr_str_btn_config            : pointer to the desired button structure
* @param pv_btn_cbf                    : pointer to the notification function
*
* @return BTN_STATUS_OK                : When the operation is successful
*         BTN_STATUS_INVALID_STATE     : Button structure and/or function pointers are NULL_PTRs
*                                        or if the button is not connected to and EXI pin
*         BTN_STATUS_INVALID_PULL_TYPE : If the pull type field in button structure is set to invalid value
*/
en_btn_status_code_t_ btn_set_notification(st_btn_config_t_* ptr_str_btn_config, void (*pv_btn_cbf)(void))
{
	en_btn_status_code_t_ lo_en_btn_status = BTN_STATUS_OK;
	
	if ((NULL_PTR != ptr_str_btn_config) && (NULL_PTR != pv_btn_cbf))
	{
		switch (ptr_str_btn_config->en_btn_pull_type)
		{
			case BTN_INTERNAL_PULL_UP:
			case BTN_EXTERNAL_PULL_UP:
			{
				gpio_setIntSense((en_gpio_port_t) ptr_str_btn_config->en_btn_port,
												 (en_gpio_pin_t)  ptr_str_btn_config->en_btn_pin ,
												 FALLING_EDGE);
				break;
			}
			
			case BTN_EXTERNAL_PULL_DOWN:
			{
				gpio_setIntSense((en_gpio_port_t) ptr_str_btn_config->en_btn_port,
												 (en_gpio_pin_t)  ptr_str_btn_config->en_btn_pin ,
												 RISING_EDGE);
				break;
			}
			default : lo_en_btn_status = BTN_STATUS_INVALID_PULL_TYPE;
		}
		
		if(BTN_STATUS_OK == lo_en_btn_status)
		{
			gpio_setIntCallback((en_gpio_port_t) ptr_str_btn_config->en_btn_port,
												  (en_gpio_pin_t)  ptr_str_btn_config->en_btn_pin ,
													pv_btn_cbf);
			gpio_enableInt((en_gpio_port_t) ptr_str_btn_config->en_btn_port,
													(en_gpio_pin_t)  ptr_str_btn_config->en_btn_pin );
		}
	}
	else
	{
		lo_en_btn_status = BTN_STATUS_INVALID_STATE;
	}
	
	return lo_en_btn_status;
}
