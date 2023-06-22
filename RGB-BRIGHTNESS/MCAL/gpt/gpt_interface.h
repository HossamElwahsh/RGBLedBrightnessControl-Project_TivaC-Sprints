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

#define     MAX_PRESCALER_REG_TIMER         255
#define     MAX_PRESCALER_WIDE_TIMER        65535

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

// todo support more modes
typedef enum{
    CH_MODE_ONE_SHOT = 0    ,
    CH_MODE_PERIODIC        ,
    CH_MODE_TOTAL
}en_gpt_channel_mode_t;


typedef enum{
    GPT_INT_ENABLED = 0     ,
    GPT_INT_DISABLED        ,
    GPT_INT_TOTAl
}en_gpt_int_enabled_t;

typedef void (* ptr_vd_fun_vd_t)(void);

typedef struct gpt_config {
    en_gpt_channel_t        en_gpt_channel_id;
    en_gpt_channel_mode_t   en_gpt_channel_mode;
    en_gpt_int_enabled_t    en_gpt_int_enabled;
}st_gpt_config_t;

en_gpt_status_t gpt_init(st_gpt_config_t * st_a_gpt_config_arr);

en_gpt_status_t gpt_enable_notification(en_gpt_channel_t en_a_gpt_channel);
en_gpt_status_t gpt_disable_notification(en_gpt_channel_t en_a_gpt_channel);

en_gpt_status_t gpt_start(en_gpt_channel_t en_a_gpt_channel, uint32_t_ uint32_a_delay_value, en_gpt_time_unit_t en_a_gpt_time_unit);
en_gpt_status_t gpt_stop(en_gpt_channel_t en_a_gpt_channel);

en_gpt_status_t gpt_get_elapsed_time(en_gpt_channel_t en_a_gpt_channel, uint32_t_ * uint32_a_elapsed_time);
en_gpt_status_t gpt_get_remaining_time(en_gpt_channel_t en_a_gpt_channel,  uint32_t_ * uint32_a_rem_time);

en_gpt_status_t gpt_set_callback(en_gpt_channel_t en_a_gpt_channel, ptr_vd_fun_vd_t ptr_vd_fun_vd_a_gpt_notification);


#endif //GPT_INTERFACE_H
