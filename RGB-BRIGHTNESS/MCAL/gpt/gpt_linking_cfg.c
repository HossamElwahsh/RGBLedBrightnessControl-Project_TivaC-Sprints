//
// Created by hossam on 21/6/2023.
//

#include "gpt_linking_cfg.h"
#include "std.h"

st_gpt_config_t gl_st_gpt_lconfig_arr[GPT_CONFIGURED_TIMERS_CHS_COUNT] = {
        {
            CH_0,
            CH_MODE_ONE_SHOT,
         GPT_INT_ENABLED,
         NULL_PTR
         },
         {
            CH_1,
            CH_MODE_PERIODIC,
            GPT_INT_ENABLED,
         NULL_PTR
         },
         {
            CH_2,
            CH_MODE_PERIODIC,
            GPT_INT_ENABLED,
         NULL_PTR
         },
};