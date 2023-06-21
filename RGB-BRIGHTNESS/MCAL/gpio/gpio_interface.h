#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

/*----------------------------------------------------------/
/- INCLUDES
/----------------------------------------------------------*/
#include "std.h"

/*----------------------------------------------------------/
/- MACROS
/----------------------------------------------------------*/
#define PORT_CLR  	0x00
#define PORT_SET		0xff

/*----------------------------------------------------------/
/- PRIMITIVE TYPES 
/----------------------------------------------------------*/
typedef void (*gpio_cb)(void);

/*----------------------------------------------------------/
/- ENUMS
/----------------------------------------------------------*/
typedef enum
{
	GPIO_PORT_A =0	,
	GPIO_PORT_B			,
	GPIO_PORT_C			,
	GPIO_PORT_D			,
	GPIO_PORT_E			,
	GPIO_PORT_F			,
	GPIO_PORT_TOTAL
}en_gpio_port_t;

typedef enum
{
	GPIO_PIN_0 = 0	,
	GPIO_PIN_1			,
	GPIO_PIN_2			,
	GPIO_PIN_3			,
	GPIO_PIN_4			,
	GPIO_PIN_5			,
	GPIO_PIN_6			,
	GPIO_PIN_7			,
	GPIO_PIN_TOTAL
}en_gpio_pin_t;

typedef enum
{
	DIGITAL	= 0			,
	ANALOG					,
	ALT_FUNC
}en_gpio_pin_mode_t;


typedef enum
{
	LOW = 0,
	HIGH
}en_gpio_pin_level_t;


typedef enum
{
	INPUT								= 0 ,
	OUTPUT									,
	INPUT_ANALOG  				  ,
	INPUT_PULL_UP						,
	INPUT_PULL_DOWN					,
	OUTPUT_OPEN_DRAIN				,
	ALT_FUNCTION						,
	PIN_CFG_TOTAL
}en_gpio_pin_cfg_t;


typedef enum
{
	PIN_CURRENT_2MA = 0  ,
	PIN_CURRENT_4MA      ,
	PIN_CURRENT_8MA          
}en_gpio_pin_current_t;

typedef enum
{
	FALLING_EDGE = 0	,
	LOW_LEVEL					,
	RISING_EDGE 			,
	HIGH_LEVEL				,
	BOTH_EDGES				,
	INT_EVENT_TOTAL
}en_gpio_int_event_t;

typedef enum
{
	GPIO_OK							= 0 ,
	GPIO_INVALID_PORT				,
	GPIO_INVALID_PIN				,
	GPIO_INVALID_PIN_CFG		,
	GPIO_INVALID_INT_EVENT	,
	GPIO_ERROR
}en_gpio_error_t;

/*----------------------------------------------------------/
/- STRUCTURES
/----------------------------------------------------------*/
typedef struct
{
	en_gpio_port_t 				port			 ; /* The port of the pin to configure */
	en_gpio_pin_t  				pin				 ; /* The pin number to configure */
	en_gpio_pin_cfg_t			pin_cfg		 ;
	//en_gpio_pin_level_t 	init_val	 ; /* The initial pin value (LOW/HIGH) */
	en_gpio_pin_current_t current		 ; /* The output current on the pin(s)(ignored if input) */
}st_gpio_cfg_t;

/*---------------------------------------------------------/
/ FUNCTIONS PROTOTYPES 
/---------------------------------------------------------*/
en_gpio_error_t gpio_port_init 		 (st_gpio_cfg_t* port_cfg);

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
en_gpio_error_t gpio_pin_init  		 (st_gpio_cfg_t* pin_cfg);

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
en_gpio_error_t gpio_setPortVal		 (en_gpio_port_t en_a_port,  uint8_t_ u8_a_portVal);

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
en_gpio_error_t gpio_setPinVal 		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_pin_level_t en_a_pinVal);

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
en_gpio_error_t gpio_togPinVal 		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin);

/** 
 ** @breif Function to get the value of a given pin 
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
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 *					GPIO_ERROR	     : If the passed pointer is a null pointer
 */
en_gpio_error_t gpio_getPinVal 		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_pin_level_t* pu8_a_Val);

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
en_gpio_error_t gpio_enableInt 		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin);

/** 
 ** @breif Function disable the GPIO interrupt for a given pin 
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port of the desired pin
 *				[in]  en_a_pin   	 : The desired pin 
 *
 ** @return	GPIO_OK          : If the operation is done successfully
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 */
en_gpio_error_t gpio_disableInt		 (en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin);

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
en_gpio_error_t gpio_setIntSense(en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, en_gpio_int_event_t en_a_event);

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
en_gpio_error_t gpio_setIntCallback(en_gpio_port_t en_a_port, en_gpio_pin_t en_a_pin, gpio_cb pv_a_cbf);

#endif
