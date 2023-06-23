/**
 * @file     btn_interface.h
 * @brief    header file to interface with a push button on TM4C123GH6PM
 * @author   Alaa Hisham
 * @date     June 17, 2023
 * @version  0.1
 */


#ifndef BTN_INTERFACE_H_
#define BTN_INTERFACE_H_

/*----------------------------------------------------------/
/- ENUMS
/----------------------------------------------------------*/
/* button Pins */
typedef enum{
	BTN_PIN_0	=	0	,
	BTN_PIN_1			,
	BTN_PIN_2			,
	BTN_PIN_3			,
	BTN_PIN_4			,
	BTN_PIN_5			,
	BTN_PIN_6			,
	BTN_PIN_7			,
	BTN_PIN_TOTAL
}en_btn_pin_t_;

/* button Ports */
typedef enum
{
	BTN_PORT_A	=	0	,
	BTN_PORT_B			,
	BTN_PORT_C			,
	BTN_PORT_D			,
	BTN_PORT_E			,
	BTN_PORT_F			,
	BTN_PORT_TOTAL
}en_btn_port_t_;

typedef enum
{
	BTN_STATE_NOT_PRESSED = 0	,
	BTN_STATE_PRESSED			,
	BTN_STATE_TOTAL
}en_btn_state_t_;

typedef enum
{
	BTN_INTERNAL_PULL_UP = 0	,
	BTN_INTERNAL_PULL_DOWN		,
	BTN_EXTERNAL_PULL_UP			,
	BTN_EXTERNAL_PULL_DOWN		,
	BTN_PULL_TOTAL
}en_btn_pull_t_;

typedef enum
{
	BTN_ACTIVATED	= 0 ,
	BTN_DEACTIVATED  
}en_btn_active_state_t_;

typedef enum
{
	BTN_STATUS_OK = 0			 ,
	BTN_STATUS_INVALID_PULL_TYPE ,
	BTN_STATUS_INVALID_STATE	 ,
	BTN_STATUS_DEACTIVATED			,
	BTN_STATUS_BUSY
}en_btn_status_code_t_;

/*----------------------------------------------------------/
/- STRUCTS
/----------------------------------------------------------*/
typedef struct
{
	en_btn_port_t_				en_btn_port 	  ;
	en_btn_pin_t_ 				en_btn_pin 	  ;
	en_btn_pull_t_				en_btn_pull_type ;
	/** Read only */
	en_btn_active_state_t_		en_btn_activation;
}st_btn_config_t_;

/*---------------------------------------------------------/
/- FUNCTION PROTOTYPES
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
en_btn_status_code_t_ btn_init(st_btn_config_t_* ptr_st_btn_config);

en_btn_status_code_t_ btn_deinit(st_btn_config_t_* ptr_st_btn_config);

/**
 * @brief Function to read the current button state 
 * 
 * @param ptr_st_btn_config            : pointer to the desired button structure
 * @param ptr_en_btn_state             : pointer to variable to store the button state
 * 
 * @return BTN_STATUS_OK                : When the operation is successful
 *         BTN_STATUS_INVALID_STATE     : Button structure and/or button state pointers are NULL_PTRs
 *         BTN_STATUS_INVALID_PULL_TYPE : If the pull type field in button structure is set to invalid value
 *		   BTN_STATUS_DEACTIVATED		: If we are trying to read from a deactivated button
 */
en_btn_status_code_t_ btn_read(st_btn_config_t_* ptr_st_btn_config, en_btn_state_t_* ptr_en_btn_state);

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
en_btn_status_code_t_ btn_set_notification(st_btn_config_t_* ptr_str_btn_config, void (*pv_btn_cbf)(void));

#endif /* BTN_INTERFACE_H_ */
