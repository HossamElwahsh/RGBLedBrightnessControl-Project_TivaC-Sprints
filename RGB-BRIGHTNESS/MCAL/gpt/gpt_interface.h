/**
 * @file    :   gpt_interface.h
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Header File contains all GPT functions' prototypes, typedefs and pre-configurations
 * @version :   0.1
 * @date    :   2023-06-21
 *
 * @copyright Copyright (c) 2023
 */

#ifndef GPT_INTERFACE_H
#define GPT_INTERFACE_H

#include "std.h"

#define     MC_F_CPU_HZ      16000000UL

#define GPT_CONFIGURED_TIMERS_CHS_COUNT     3


/* Typedefs
 * -------------------- */

typedef enum{
    GPT_OK          = 0 ,
    GPT_INVALID_ARGS    ,
    GPT_INVALID_CFG     ,
    GPT_NOT_SUPPORTED   ,
    GPT_ERROR           ,
    GPT_STATUS_TOTAL
}en_gpt_status_t;

typedef enum{
    TIME_IN_SEC      = 1        ,
    TIME_IN_MS       = 1000     ,
    TIME_IN_US       = 1000000  ,
    TIME_UNITS_TOTAL
}en_gpt_time_unit_t;

typedef enum{
    CH_0    = 0 ,
    CH_1        ,
    CH_2        ,
    CH_3        ,
    CH_4        ,
    CH_5        ,
    CH_6_W      ,
    CH_7_W      ,
    CH_8_W      ,
    CH_9_W      ,
    CH_10_W     ,
    CH_11_W     ,
    CH_TOTAL
}en_gpt_channel_t;


/**
 * @brief                           :   Initializes GPT timers
 *
 * @return      GPT_OK              :   If Success
 *              GPT_ERROR           :   If Failed
 *              GPT_INVALID_CFG     :   Bad Config
 *
 */
en_gpt_status_t gpt_init(void);


/**
 * @brief                             :     Calculates timer values then starts a delay on the requested timer channel
 *
 * @param[in]   en_a_gpt_channel      :     Timer Channel
 * @param[in]   uint32_a_delay_value  :     Delay value in Time Units
 * @param[in]   en_a_gpt_time_unit    :     Time units (seconds, milli-seconds, micro-seconds)
 *
 * @return      GPT_OK                :     If Success
 *              GPT_INVALID_ARGS      :     Failed, Invalid Args Given
 *              GPT_NOT_SUPPORTED     :     Failed, Delay value requested isn't supported by selected timer channel
 */
en_gpt_status_t gpt_start(en_gpt_channel_t en_a_gpt_channel, uint32_t_ uint32_a_delay_value, en_gpt_time_unit_t en_a_gpt_time_unit);

/**
 * @brief                           :   Stops timer channel
 *
 * @param[in]   en_a_gpt_channel    :   Timer Channel
 *
 * @return      GPT_OK              :     If Success
 *              GPT_ERROR           :     Failed, Error occurred
 *              GPT_INVALID_ARGS    :     Failed, Invalid Args Given
 */
en_gpt_status_t gpt_stop(en_gpt_channel_t en_a_gpt_channel);


/**
 * @brief                                   :   Get Timer Current Elapsed time
 *
 * @param[in]   en_a_gpt_channel            :   Timer Channel
 * @param[out]  ptr_uint32_a_elapsed_time   :   Ptr to store elapsed time value
 *
 * @return      GPT_OK                      :     If Success
 *              GPT_ERROR                   :     Failed
 *              GPT_INVALID_ARGS            :     Failed, Invalid Args Given
 */
en_gpt_status_t gpt_get_elapsed_time(en_gpt_channel_t en_a_gpt_channel, uint32_t_ * ptr_uint32_a_elapsed_time);


/**
 * @brief                                   :   Get Timer Current Remaining time
 *
 * @param[in]   en_a_gpt_channel            :   Timer Channel
 * @param[out]  ptr_uint32_a_rem_time       :   Ptr to store remaining time value
 *
 * @return      GPT_OK                      :   If Success
 *              GPT_ERROR                   :   Failed
 *              GPT_INVALID_ARGS            :   Failed, Invalid Args Given
 */
en_gpt_status_t gpt_get_remaining_time(en_gpt_channel_t en_a_gpt_channel,  uint32_t_ * ptr_uint32_a_rem_time);


/**
 * @brief                                           :   Sets callback function to a timer channel
 *
 * @param[in]   en_a_gpt_channel                    :   Timer channel
 * @param[in]   ptr_vd_fun_vd_a_gpt_notification    :   Ptr to callback function to store
 *
 * @return      GPT_OK                              :   If Success
 *              GPT_ERROR                           :   Failed
 *              GPT_INVALID_ARGS                    :   Failed, Invalid Args Given
 */
en_gpt_status_t gpt_set_callback(en_gpt_channel_t en_a_gpt_channel, ptr_vd_fun_vd_t ptr_vd_fun_vd_a_gpt_notification);


/**
 * @brief                           :   enables timer channel notification (callback)
 *
 * @param       en_a_gpt_channel    :   Timer Channel
 *
 * @return      GPT_OK              :   If Success
 *              GPT_ERROR           :   Failed
 *              GPT_INVALID_ARGS    :   Failed, Invalid Args Given
 */
en_gpt_status_t gpt_enable_notification(en_gpt_channel_t en_a_gpt_channel);


/**
 * @brief                           :   disable timer channel notification (callback)
 *
 * @param       en_a_gpt_channel    :   Timer Channel
 *
 * @return      GPT_OK              :   If Success
 *              GPT_ERROR           :   Failed
 *              GPT_INVALID_ARGS    :   Failed, Invalid Args Given
 */
en_gpt_status_t gpt_disable_notification(en_gpt_channel_t en_a_gpt_channel);


#endif //GPT_INTERFACE_H
