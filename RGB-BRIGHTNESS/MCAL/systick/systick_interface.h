/**
 * @file    :   systick_interface.h
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Header File contains all SYSTICK functions' prototypes, typedefs and pre-configurations
 * @version :   0.2
 * @date    :   2023-06-17
 *
 * @copyright Copyright (c) 2023
 */

#ifndef SYSTICK_INTERFACE_H
#define SYSTICK_INTERFACE_H

#include "std.h"

#define SYS_CLOCK_MHZ   8

#if SYS_CLOCK_MHZ < 8
    #warning System clock below 8 MHZ is not supported by systick
#endif
#define PIOSC_MHZ       16

/*----------------------------------------------------------/
/- ENUMS
/----------------------------------------------------------*/
typedef enum{
    /* precision internal oscillator / 4 (divided by 4) */
    CLK_SRC_PIOSC    =   0   ,

    /* system clock */
    CLK_SRC_SYS_CLK          ,

    CLK_SRC_TOTAL
}en_systick_clk_src_t;

typedef enum{
    ST_OK               =   0   ,
    ST_INVALID_CONFIG           ,
    ST_INVALID_ARGS             ,
}en_systick_error_t;

typedef void (*fun_systick_callback_t)(void);


/*----------------------------------------------------------/
/- STRUCTURES
/----------------------------------------------------------*/
typedef struct{

    /**
     * False: Interrupt generation is disabled. Software can use the
     *      COUNT bit to determine if the counter has ever reached 0
     *
     * True: An interrupt is generated to the NVIC when SysTick counts to 0.
     *
     * */
//    boolean bool_systick_int_enabled;

    en_systick_clk_src_t en_systick_clk_src;

    fun_systick_callback_t fun_ptr_systick_cb;

}st_systick_cfg_t;

/**
 * @brief                       : Initializes SYSTICK driver
 *
 * @param ptr_a_st_systick_cfg        : Pointer to Systick Configuration
 *
 * @return  ST_OK              :   In case of Successful Operation
 *          ST_INVALID_ARGS    :   In case of Failed Operation (Invalid Arguments Given)
 *          ST_INVALID_CONFIG  :   In case of Failed Operation (Invalid Systick Config Given)
 */
en_systick_error_t systick_init(st_systick_cfg_t * ptr_a_st_systick_cfg);


/**
 * @brief                      : Initiates a sync blocking delay
 *
 * @param uint32_a_ms_delay      : Desired delay in ms
 *
 * @return  ST_OK              :   In case of Successful Operation
 *          ST_INVALID_ARGS    :   In case of Failed Operation (Invalid Arguments Given)
 *          ST_INVALID_CONFIG  :   In case of Failed Operation (Invalid Systick Config Given)
 */
en_systick_error_t systick_ms_delay(uint32_t_ uint32_a_ms_delay);


/**
 * @brief                      :    Initiates a sync blocking delay
 *
 * @param uint32_a_ms_delay      :    Desired delay in ms
 * @note                       :    Will be cancelled if any sync/blocking delay was requested
 *
 * @return  ST_OK              :    In case of Successful Operation
 *          ST_INVALID_ARGS    :    In case of Failed Operation (Invalid Arguments Given)
 *          ST_INVALID_CONFIG  :    In case of Failed Operation (Invalid Systick Config Given)
 */
en_systick_error_t systick_async_ms_delay(uint32_t_ uint32_a_ms_delay);


/**
 * @brief                      :    Sets callback function to be called when an async delay is finished
 *
 * @param fun_ptr_a_systick_cb   :    Pointer to callback fn
 *
 * @return  ST_OK              :    In case of Successful Operation
 *          ST_INVALID_ARGS    :    In case of Failed Operation (Invalid Arguments Given)
 *          ST_INVALID_CONFIG  :    In case of Failed Operation (Invalid Systick Config Given)
 */
en_systick_error_t systick_set_callback(fun_systick_callback_t fun_ptr_a_systick_cb);


#endif //SYSTICK_INTERFACE_H
