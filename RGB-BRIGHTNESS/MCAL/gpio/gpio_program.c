/**
 ** @file      gpio_program.c
 ** @brief     
 ** @author    Alaa
 ** @date      June 16, 2023
 ** @version   0.1
 */

/*- INCLUDES
----------------------------------------------*/
#include "bit_math.h"

#include "TM4C123.h"

#include "gpio_interface.h"
#include "gpio_private.h"


gpio_cb arr_gpio_cbf[GPIO_PORT_TOTAL][GPIO_PIN_TOTAL] = {{NULL}};

/*---------------------------------------------------------/
/ FUNCTION IMPLEMENTATION 
/---------------------------------------------------------*/

/** 
 ** @breif Function to validate the values of the given port and pin
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port value to validate
 *				[in]  en_a_pin   	 : The pin value to validate
 *
 ** @return	GPIO_OK          : If the port and pin have valid values
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 */
static en_gpio_error_t port_pin_check(en_gpio_port_t port, en_gpio_pin_t pin)
{
	if		 (port >= GPIO_PORT_TOTAL) return GPIO_INVALID_PORT;
	else if((pin  >= GPIO_PIN_TOTAL) 
			|| (port == GPIO_PORT_F && pin > GPIO_PIN_4))
					return GPIO_INVALID_PIN;
	else return GPIO_OK;
}

/** 
 **@breif Function initialize a gpio pin 
 *
 * This function configures any gpio pin with the
 * configurations set in the referenced structure
 *
 ** @Parameters
*				[in] ptr_st_pin_cfg : pointer to the pin configuration structure
 *
 ** @return	GPIO_OK          : If the operation is done successfully
 *					GPIO_INALID_PORT : If the passed port is not a valid port
 *					GPIO_INALID_PIN  : If the passed pin is not a valid pin
 *					GPIO_ERROR	     : If the passed pointer is a null pointer
 */
en_gpio_error_t gpio_pin_init  (st_gpio_cfg_t* ptr_st_pin_cfg)
{
	en_gpio_error_t gpio_error_state = GPIO_OK;
	
	if(NULL_PTR != ptr_st_pin_cfg)
	{
		/* Validate the port and pin values */
		gpio_error_state = port_pin_check(ptr_st_pin_cfg->port, ptr_st_pin_cfg->pin);
		
		if(GPIO_OK == gpio_error_state)
		{
			en_gpio_port_t port = ptr_st_pin_cfg->port;
			en_gpio_pin_t  pin  = ptr_st_pin_cfg->pin;
			
			/* Enable the port clock */
			SET_BIT(RCGCGPIO, port);
						
			/* Set the pin direction */
			switch(ptr_st_pin_cfg->pin_cfg)
			{
				case INPUT: //floating
				{
					SET_BIT(GPIODEN(port), pin);
					CLR_BIT(GPIOAMSEL(port), pin);
					CLR_BIT(GPIOAFSEL(port), pin);
					
					CLR_BIT(GPIODIR(port), pin);
					break;
				}
				case OUTPUT:
				{
					SET_BIT(GPIODEN(port), pin);
					CLR_BIT(GPIOAMSEL(port), pin);
					CLR_BIT(GPIOAFSEL(port), pin);
					
					SET_BIT(GPIODIR(port), pin);
					break;
				}
				case INPUT_ANALOG:
				{
					CLR_BIT(GPIODIR(port), pin);
					CLR_BIT(GPIODEN(port), pin);
					SET_BIT(GPIOAMSEL(port), pin);
				}
				case INPUT_PULL_UP:
				{
					SET_BIT(GPIODEN(port), pin);
					CLR_BIT(GPIOAMSEL(port), pin);
					CLR_BIT(GPIOAFSEL(port), pin);
					
					CLR_BIT(GPIODIR(port), pin);
					SET_BIT(GPIOPUR(port), pin);
					break;
				}
				case INPUT_PULL_DOWN:
				{
					SET_BIT(GPIODEN(port), pin);
					CLR_BIT(GPIOAMSEL(port), pin);
					CLR_BIT(GPIOAFSEL(port), pin);
					
					CLR_BIT(GPIODIR(port), pin);
					SET_BIT(GPIOPDR(port), pin);
					break;
				}
				case OUTPUT_OPEN_DRAIN:
				{
					SET_BIT(GPIODEN(port), pin);
					CLR_BIT(GPIOAMSEL(port), pin);
					CLR_BIT(GPIOAFSEL(port), pin);
					
					SET_BIT(GPIODIR(port), pin);
					SET_BIT(GPIOODR(port), pin);
					break;
				}
				default: gpio_error_state = GPIO_INVALID_PIN_CFG;
			}			
			/* Set the pin drive strength */
			if	((GPIO_OK == gpio_error_state) 
				&& (GET_BIT(GPIODIR(port), pin)))
			{
				switch(ptr_st_pin_cfg->current)
				{
					case PIN_CURRENT_2MA: SET_BIT(GPIODR2R(port), pin); break;
					case PIN_CURRENT_4MA: SET_BIT(GPIODR4R(port), pin); break;
					case PIN_CURRENT_8MA: SET_BIT(GPIODR8R(port), pin); break;
					default : gpio_error_state = GPIO_ERROR;
				}
			}
			else
			{
				/* Do Nothing */
			}			
		}
		else
		{
			gpio_error_state = GPIO_INVALID_PORT;
		}
	}
	else
	{
		/* Do Nothing */
	}

	return gpio_error_state;
}

/** 
 ** @breif Function to set the value of an entire port
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in] 	en_a_pinVal	 : The value to set the bit to
 *
 ** @return	GPIO_OK           : If the operation is done successfully
 *					GPIO_INVALID_PORT : If the passed port is not a valid port
 *					GPIO_ERROR				: If the pin value is invalid (not HIGH/LOW)
 *															or if the port is not configured as an output port
 */
en_gpio_error_t gpio_setPortVal		 (en_gpio_port_t en_a_port,  uint8_t_ u8_a_portVal)
{
	en_gpio_error_t gpio_error_state;
	
	/* Check whether the port is valid */
	if(en_a_port < GPIO_PORT_TOTAL)
	{
		/* Check that the port is an output port */
		if(PORT_SET == GPIODIR(en_a_port))
		{
				GPIODATA(en_a_port) = u8_a_portVal;
		}
		else
		{
			gpio_error_state = GPIO_ERROR;
		}
	}
	else
	{
		gpio_error_state = GPIO_INVALID_PORT;
	}

	return gpio_error_state;
}

/** 
 ** @breif Function to set the value of a given pin 
 *
 * This function sets the value of the given pin to 
 * the given pin value
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin to set the value of 
 *				[in] 	en_a_pinVal	 : The value to set the bit to
 *
 ** @return	GPIO_OK           : If the operation is done successfully
 *					GPIO_INVALID_PORT : If the passed port is not a valid port
 *					GPIO_INVALID_PIN  : If the passed pin is not a valid pin
 *					GPIO_ERROR				: If the pin value is invalid (not HIGH/LOW)
 *															or if the pin is not configured as an output pin
 */
en_gpio_error_t gpio_setPinVal (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_pin_level_t en_a_pinVal)
{
	/* Validate the port and pin numbers */
	en_gpio_error_t gpio_error_state = port_pin_check(en_a_port, en_a_pin);
		
	if(GPIO_OK == gpio_error_state)
	{
		/* Check that the pin is an outpun pin */
		if(GET_BIT(GPIODIR(en_a_port), en_a_pin))
		{
			switch(en_a_pinVal)
			{
				case LOW : CLR_BIT(GPIODATA(en_a_port), en_a_pin); break;
				case HIGH: SET_BIT(GPIODATA(en_a_port), en_a_pin); break;
				default	 : gpio_error_state = GPIO_ERROR;
			}
		}
		else
		{
			gpio_error_state = GPIO_ERROR;
		}
	}
	else
	{
		/* Do Nothing */
	}

	return gpio_error_state;
}

/** 
 ** @breif Function to toggle the value of a given pin 
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin to set the value of 
 *
 ** @return	GPIO_OK           : If the operation is done successfully
 *					GPIO_INVALID_PORT : If the passed port is not a valid port
 *					GPIO_INVALID_PIN  : If the passed pin is not a valid pin
 *					GPIO_ERROR				: If the pin is not configured as an output pin
 */
en_gpio_error_t gpio_togPinVal 		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin)
{
	/* Validate the port and pin numbers */
	en_gpio_error_t gpio_error_state = port_pin_check(en_a_port, en_a_pin);
		
	if(GPIO_OK == gpio_error_state)
	{
		/* Check that the pin is an outpun pin */
		if(GET_BIT(GPIODIR(en_a_port), en_a_pin))
		{
			TOG_BIT(GPIODATA(en_a_port), en_a_pin);
		}
		else
		{
			gpio_error_state = GPIO_ERROR;
		}
	}
	else
	{
		/* Do Nothing */
	}

	return gpio_error_state;
}

/** 
 **@breif Function to get the value of a given pin 
 *
 * This function reads the value of the given pin and 
 * returns the value in the given address
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin to read value of 
 *				[out] pu8_a_val  	 : pointer to variable to store the pin value
 *
 ** @return	GPIO_OK          : If the operation is done successfully
 *					GPIO_INVALID_PORT : If the passed port is not a valid port
 *					GPIO_INVALID_PIN  : If the passed pin is not a valid pin
 *					GPIO_ERROR	     : If the passed pointer is a null pointer
 *
 */
en_gpio_error_t gpio_getPinVal (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_pin_level_t* pu8_a_val)
{
	en_gpio_error_t gpio_error_state = GPIO_OK;
	
	if(NULL_PTR != pu8_a_val)
	{
		gpio_error_state = port_pin_check(en_a_port, en_a_pin);
		
		if(GPIO_OK == gpio_error_state)
		{
			*pu8_a_val = (en_gpio_pin_level_t) GET_BIT(GPIODATA(en_a_port), en_a_pin);
		}
		else
		{
			/* Do Nothing */
		}
	}
	else
	{
		gpio_error_state = GPIO_ERROR;
	}

	return gpio_error_state;
}

/** 
 ** @breif Function enable the GPIO interrupt for a given pin 
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin 
 *
 ** @return	GPIO_OK          : If the operation is done successfully
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 */
en_gpio_error_t gpio_enableInt (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin)
{
	en_gpio_error_t gpio_error_state = port_pin_check(en_a_port, en_a_pin);
	
	if(GPIO_OK == gpio_error_state)
	{
		SET_BIT(GPIOIM(en_a_port), en_a_pin);
		
		if(GPIO_PORT_F == en_a_port)
		{
			/* Todo: */
			NVIC_EnableIRQ(GPIOF_IRQn);
			__enable_irq();
		}
		else
		{
			NVIC_EnableIRQ((IRQn_Type) en_a_port);
			__enable_irq();
		}
	}
	else { /* Do Nothing */}
	
	return gpio_error_state;
}

/** 
 * @breif Function enable the GPIO interrupt for a given pin 
 *
 * @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin 
 *
 * @return GPIO_OK           : If the operation is done successfully
 *				 GPIO_INVALID_PORT : If the passed port is not a valid port
 *				 GPIO_INVALID_PIN  : If the passed pin is not a valid pin
 */
en_gpio_error_t gpio_disableInt(en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin)
{
	/* Validate the port and pin values */
	en_gpio_error_t gpio_error_state = port_pin_check(en_a_port, en_a_pin);
	
	if(GPIO_OK == gpio_error_state)
	{
		CLR_BIT(GPIOIM(en_a_port), en_a_pin);
	}
	else { /* Do Nothing */}
	
	return gpio_error_state;
}

/**
 * @brief  Function to choose the trigger event for a given pin 
 *
 * This function sets the given GPIO pin to be triggered whenever 
 * the given event occurs
 *
 * @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin 
 *				[in]  en_a_event 	 : The event to trigger an interrupt
 * @return 
 *			GPIO_OK           		: If the operation is done successfully
 *			GPIO_INVALID_PORT 		: If the passed port is not a valid port
 *			GPIO_INVALID_PIN  		: If the passed pin is not a valid pin
 *			GPIO_INVALID_INT_EVENT: If the passed event is not valid/supported
 */
en_gpio_error_t gpio_setIntSense(en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_int_event_t en_a_event)
{
	/* Validate the port and pin values */
	en_gpio_error_t gpio_error_state = port_pin_check(en_a_port, en_a_pin);

	if(GPIO_OK == gpio_error_state)
	{
		if(en_a_event < INT_EVENT_TOTAL)
		{
			uint32_t_ pin_mask = NULL;
			SET_BIT(pin_mask, en_a_pin);
			
			/* Disable the interrupt */
			CLR_BIT(GPIOIM(en_a_port), en_a_pin);
			
			/* Configure the interrupt sense */
			if(GET_BIT(en_a_event, GPIO_INT_SENSE_MASK)) SET_BIT(GPIOIS(en_a_port), en_a_pin);
			else CLR_BIT(GPIOIS(en_a_port), en_a_pin);
			
			if(BOTH_EDGES == en_a_event)
			{
				SET_BIT(GPIOIBE(en_a_port), en_a_pin);
			}
			else
			{
				/* Configure the interrupt level */
				if(GET_BIT(en_a_event, GPIO_INT_LEVEL_MASK)) SET_BIT(GPIOIEV(en_a_port), en_a_pin);
				else CLR_BIT(GPIOIEV(en_a_port), en_a_pin);
			}
			
			/* Clear the raw interrupt flag */
			CLR_BIT(GPIORIS(en_a_port), en_a_pin);
			
			/* Enable the interrupt */
			SET_BIT(GPIOIM(en_a_port), en_a_pin);
		}
		else
		{
			gpio_error_state = GPIO_INVALID_INT_EVENT;
		}
	}
	else
	{
		/* Do Nothing */
	}

	return gpio_error_state;
}

/** 
 ** @breif Function to set the callback function for a GPIO pin interrupt
 *
 * This function sets a given function to be called whenever an
 * interrupt on the given pin is triggered (if the pin interrupt was enabled)
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin to read value of 
 *				[in]  pv_a_cbf     : pointer to the desired function to call
 *
 ** @return	GPIO_OK          : If the operation is done successfully
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 *					GPIO_ERROR	     : If the passed function pointer is a null pointer
 */
en_gpio_error_t gpio_setIntCallback(en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, gpio_cb pv_a_cbf)
{
	en_gpio_error_t gpio_error_state;
		
	if(NULL_PTR != pv_a_cbf)
	{
		/* Validate the port and pin numbers */
		gpio_error_state = port_pin_check(en_a_port, en_a_pin);
		
		if(GPIO_OK == gpio_error_state)
		{	
			arr_gpio_cbf[en_a_port][en_a_pin] = pv_a_cbf;
		}
	}
	else
	{
		gpio_error_state = GPIO_ERROR;
	}
	
	return gpio_error_state;
}
/*---------------------------------------------------------/
/ INTERRUPT HANDLERS
/---------------------------------------------------------*/
void GPIOA_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_5)) pin = GPIO_PIN_5;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_6)) pin = GPIO_PIN_6;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_A), GPIO_PIN_7)) pin = GPIO_PIN_7;
	else{/* Do Nothing */}
	
	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_A), pin);

	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_A][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_A][pin]();
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

void GPIOB_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_5)) pin = GPIO_PIN_5;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_6)) pin = GPIO_PIN_6;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_B), GPIO_PIN_7)) pin = GPIO_PIN_7;
	else{/* Do Nothing */}

	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_B), pin);
	
	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_B][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_B][pin]();
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

void GPIOC_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_5)) pin = GPIO_PIN_5;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_6)) pin = GPIO_PIN_6;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_C), GPIO_PIN_7)) pin = GPIO_PIN_7;
	else{/* Do Nothing */}

	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_C), pin);
	
	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_C][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_C][pin]();
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

void GPIOD_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_5)) pin = GPIO_PIN_5;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_6)) pin = GPIO_PIN_6;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_D), GPIO_PIN_7)) pin = GPIO_PIN_7;
	else{/* Do Nothing */}

	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_D), pin);
	
	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_D][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_D][pin]();
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


void GPIOE_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_5)) pin = GPIO_PIN_5;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_6)) pin = GPIO_PIN_6;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_E), GPIO_PIN_7)) pin = GPIO_PIN_7;
	else{/* Do Nothing */}

	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_E), pin);
	
	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_E][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_E][pin]();
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

void GPIOF_Handler(void)
{
	en_gpio_pin_t pin = GPIO_PIN_TOTAL;
	
	if		 (GET_BIT(GPIOMIS(GPIO_PORT_F), GPIO_PIN_0)) pin = GPIO_PIN_0;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_F), GPIO_PIN_1)) pin = GPIO_PIN_1;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_F), GPIO_PIN_2)) pin = GPIO_PIN_2;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_F), GPIO_PIN_3)) pin = GPIO_PIN_3;
	else if(GET_BIT(GPIOMIS(GPIO_PORT_F), GPIO_PIN_4)) pin = GPIO_PIN_4;
	else{/* Do Nothing */}

	/* Clear the interrupt flag */
	SET_BIT(GPIOICR(GPIO_PORT_F), pin);
	
	if(pin != GPIO_PIN_TOTAL)
	{
		if(arr_gpio_cbf[GPIO_PORT_F][pin] != NULL)
		{
			arr_gpio_cbf[GPIO_PORT_F][pin]();
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

