//
// Created by hossam on 21/6/2023.
//

#ifndef GPT_LINKING_CFG_H
#define GPT_LINKING_CFG_H

#include "gpt_interface.h"

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


typedef struct gpt_config {
    en_gpt_channel_t        en_gpt_channel_id;
    en_gpt_channel_mode_t   en_gpt_channel_mode;
    en_gpt_int_enabled_t    en_gpt_int_enabled;
}st_gpt_config_t;

/* Extern config */
extern st_gpt_config_t gl_st_gpt_lconfig_arr[GPT_CONFIGURED_TIMERS_CHS_COUNT];

#endif //GPT_LINKING_CFG_H
