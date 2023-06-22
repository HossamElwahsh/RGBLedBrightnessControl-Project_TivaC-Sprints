/**
 * @file    :   gpt_program.c
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Program File contains all GPT functions' implementation
 * @version :   0.1
 * @date    :   2023-06-21
 *
 * @copyright Copyright (c) 2023
 */

#include "gpt_interface.h"
#include "gpt_linking_cfg.h"
#include "bit_math.h"
#include "gpt_private.h"
#include "TM4C123.h"
//#include "TM4C123GH6PM.h"

static boolean gl_gpt_initialized = FALSE;

static uint32_t_ gpt_get_timer_base_address(en_gpt_channel_t en_a_gpt_channel);
static ptr_vd_fun_vd_t gl_callbacks_array[CH_TOTAL] = {NULL_PTR};

/**
 * @brief                           :   Initializes GPT timers
 * @param[in]   en_a_gpt_channel    :   GPT Timers' configurations arrays
 *
 * @return      GPT_OK              :   If Success
 *              GPT_ERROR           :   If Failed
 *              GPT_INVALID_ARGS    :   Null pointer passed
 *              GPT_INVALID_CFG     :   Bad Config
 *
 */
en_gpt_status_t gpt_init(st_gpt_config_t * st_a_gpt_config_arr)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    if(FALSE == gl_gpt_initialized)
    {
        // args check
        if(NULL_PTR == st_a_gpt_config_arr)
        {
            en_gpt_status_retval = GPT_INVALID_ARGS;
        }
        else
        {
            // init timers
            for (int i = 0; i < GPT_CONFIGURED_TIMERS_CHS_COUNT; i++)
            {
                // config check
                if(
                        (st_a_gpt_config_arr[i].en_gpt_channel_id >= CH_TOTAL) ||
                        (st_a_gpt_config_arr[i].en_gpt_channel_mode >= CH_MODE_TOTAL)
//                        ((st_a_gpt_config_arr[i].uint32_prescale_value > 0xFFFF) && (st_a_gpt_config_arr[i].en_gpt_channel_id < CH_6_W)) // non wide only supports 8 bit prescaler
                        )
                {
                    en_gpt_status_retval = GPT_INVALID_CFG;
                    break; // break inits
                }

                /* Enable Timer Gating Control */
                if(st_a_gpt_config_arr[i].en_gpt_channel_id <= 5)
                {
                    SET_BIT(RCGCTIMER_REG_ADDRESS, st_a_gpt_config_arr[i].en_gpt_channel_id);
                }
                else
                {
                    SET_BIT(RCGCWTIMER_REG_ADDRESS, st_a_gpt_config_arr[i].en_gpt_channel_id % 6);
                }

                uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(st_a_gpt_config_arr[i].en_gpt_channel_id);
                if(0 != uint32_l_base_address)
                {
                    // initialize timer
                    // todo support concat mode
                    SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMCFG_REG_OFFSET), GPTMCFG_NOT_CONCAT_BIT);

                    if(CH_MODE_ONE_SHOT == st_a_gpt_config_arr[i].en_gpt_channel_mode)
                    {
                        SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAMR_REG_OFFSET), GPTMTXMR_ONE_SHOT_BIT);
                    }
                    else if(CH_MODE_PERIODIC == st_a_gpt_config_arr[i].en_gpt_channel_mode)
                    {
                        SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAMR_REG_OFFSET), GPTMTXMR_PERIODIC_BIT);
                    }
                    else
                    {
                        en_gpt_status_retval = GPT_INVALID_CFG;
                        break; // break inits
                    }

                    // set count direction to down
                    // todo support up count - CLEAR : Count Down, SET : Count Up
                    CLR_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAMR_REG_OFFSET), GPTMTXMR_DIR_BIT);

                    // enable interrupts if requested
                    if(GPT_INT_ENABLED == st_a_gpt_config_arr[i].en_gpt_int_enabled)
                    {
                        gpt_enable_notification(st_a_gpt_config_arr[i].en_gpt_channel_id);
                    }

                }
                else
                {
                    en_gpt_status_retval = GPT_ERROR; // failed to get base address
                    break; // break inits
                }


            }

            gl_gpt_initialized = TRUE;
        }
    }
    else
    {
        /* Do Nothing */
    }

    return en_gpt_status_retval;
}

/**
 * @brief                             :     Calculates timer values then starts a delay on the requested timer channel
 *
 * @param[in]   en_a_gpt_channel      :     Timer Channel
 * @param[in]   uint32_a_delay_value  :     Delay value in Time Units
 * @param[in]   en_a_gpt_time_unit    :     Time units (seconds, milli-seconds, micro-seconds)
 *
 * @return      GPT_OK                :     If Success
 * @return      GPT_INVALID_ARGS      :     Failed, Invalid Args Given
 * @return      GPT_NOT_SUPPORTED     :     Failed, Delay value requested isn't supported by selected timer channel
 */
en_gpt_status_t gpt_start(en_gpt_channel_t en_a_gpt_channel, uint32_t_ uint32_a_delay_value, en_gpt_time_unit_t en_a_gpt_time_unit)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    if(
            (en_a_gpt_channel >= GPT_CONFIGURED_TIMERS_CHS_COUNT) ||
            ((en_a_gpt_channel <= CH_5) && (0xFFFF < uint32_a_delay_value)) || // non-wide timer only supports 16-bits for now
            ( (en_a_gpt_time_unit != TIME_IN_SEC) && (en_a_gpt_time_unit != TIME_IN_US) && (en_a_gpt_time_unit != TIME_IN_MS) )
    )
    {
        en_gpt_status_retval = GPT_INVALID_ARGS;
    }
    else
    {
        boolean bool_l_wide_timer = en_a_gpt_channel > CH_5;
        // get base address
        uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(gl_st_gpt_lconfig_arr[en_a_gpt_channel].en_gpt_channel_id);

        if(0 != uint32_l_base_address)
        {
            // calculate prescaler & value to load

            f64_t_ f64_l_min_tick_time_sec = 0; // minimum supported value by timer to be calculated
            f64_t_ f64_l_max_total_time_sec = 0; // maximum supported value by timer to be calculated
            uint16_t_ uint16_l_prescaler = 0;
            uint32_t_ uint32_l_number_of_ticks = 0;
            f64_t_ f64_l_desired_delay_in_sec = 0;

            switch (en_a_gpt_time_unit) {

                case TIME_IN_SEC:
                {
                    f64_l_desired_delay_in_sec = uint32_a_delay_value;
                    break;
                }
                case TIME_IN_MS:
                {
                    f64_l_desired_delay_in_sec = (f64_t_)((f64_t_)uint32_a_delay_value / TIME_IN_MS);
                    break;
                }
                case TIME_IN_US:
                    f64_l_desired_delay_in_sec = (f64_t_)((f64_t_)uint32_a_delay_value / TIME_IN_US);
                    break;
                case TIME_UNITS_TOTAL:
                    break;
            }


            f64_l_min_tick_time_sec = 1.0 / MC_F_CPU_HZ; // one tick time in seconds
            f64_l_max_total_time_sec =
                    ((f64_t_) (bool_l_wide_timer ? MAX_PRESCALER_WIDE_TIMER : MAX_PRESCALER_REG_TIMER) / MC_F_CPU_HZ) *
                    (bool_l_wide_timer ? MAX_32_BIT_VAL : MAX_16_BIT_VAL);

            // check support for requested delay amount
            if(
                    (f64_l_desired_delay_in_sec < f64_l_min_tick_time_sec) || // requested delay less than minimum supported
                    (f64_l_desired_delay_in_sec > f64_l_max_total_time_sec)      // OR requested delay is more than max supported
            )
            {
                en_gpt_status_retval = GPT_NOT_SUPPORTED;
            }
            else // delay supported
            {
                // calculate appropriate prescaler
                uint16_l_prescaler = CEIL_NUMBER(PRESCALER_FINDER_HELPER((bool_l_wide_timer ? MAX_32_BIT_VAL : MAX_16_BIT_VAL) , f64_l_desired_delay_in_sec));

                // set prescaler
                if (FALSE == bool_l_wide_timer) // non wide timer
                {
                    // only 16 bits pre-scaler
                    // clear and update prescaler
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAPR_REG_OFFSET) = 0;
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAPR_REG_OFFSET) |= uint16_l_prescaler;
                } else // wide timer
                {
                    // only 8 bits pre-scaler
                    // clear and update prescaler
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAPR_REG_OFFSET) = 0;
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address,
                                            GPTMTAPR_REG_OFFSET) |= ((uint8_t_) uint16_l_prescaler);

                }

                // calculate number of ticks required
                uint32_l_number_of_ticks = CEIL_NUMBER((((f64_t_)f64_l_desired_delay_in_sec) / ((f64_t_)uint16_l_prescaler / MC_F_CPU_HZ)));

                // load value in GPTM Timer n Interval Load Register GPTMTXILR
                GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAILR_REG_OFFSET) = uint32_l_number_of_ticks;

//                GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAV_REG_OFFSET) = uint32_l_number_of_ticks;

                // set TnEN bit in GPTMCTL register to enable the timer and start counting
                SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMCTL_REG_OFFSET), GPTMCTRL_A_EN_BIT);

                // set stall bit, to pause timer when halted by debugger
                SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMCTL_REG_OFFSET), GPTMCTRL_A_STALL_BIT);

                // poll or wait for interrupt (check config)
                if(GPT_INT_DISABLED == gl_st_gpt_lconfig_arr[en_a_gpt_channel].en_gpt_int_enabled)
                {
                    // poll wait (block)
                    /* Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
                        the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
                        Register (GPTMICR) */

                    while(0 == GET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address ,GPTMRIS_REG_OFFSET), GPTMRIS_TATORIS)); // wait for timer

                    // set bit to clear flag
                    SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
                }
                else
                {
                    /* Return and wait for callback */
                }
            }
        }
        else
        {
            en_gpt_status_retval = GPT_ERROR;
        }
    }

    return en_gpt_status_retval;
}

en_gpt_status_t gpt_stop(en_gpt_channel_t en_a_gpt_channel)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    // args check
    if(
            (en_a_gpt_channel >= GPT_CONFIGURED_TIMERS_CHS_COUNT))
    {
        en_gpt_status_retval = GPT_INVALID_ARGS;
    }
    else
    {
        // clear TnEN bit in GPTMCTL register to stop the timer

        // get base address
        uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(gl_st_gpt_lconfig_arr[en_a_gpt_channel].en_gpt_channel_id);

        if(0 != uint32_l_base_address)
        {
            // Disable Timer - clear TnEN bit in GPTMCTL register
            CLR_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMCTL_REG_OFFSET), GPTMCTRL_A_EN_BIT);

            // Clear any pending interrupts
            // clear interrupt flag - set bit to clear flag
            SET_BIT(GET_ADDRESS_FROM_OFFSET(gpt_get_timer_base_address(en_a_gpt_channel), GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);

            // clear IL register
            // clear value in GPTM Timer n Interval Load Register GPTMTXILR
            GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAILR_REG_OFFSET) = 0;

        }
        else
        {
            en_gpt_status_retval = GPT_ERROR;
        }
    }

    return en_gpt_status_retval;
}


en_gpt_status_t gpt_get_elapsed_time(en_gpt_channel_t en_a_gpt_channel, uint32_t_ * uint32_a_elapsed_time)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;
    if(
            (en_a_gpt_channel >= GPT_CONFIGURED_TIMERS_CHS_COUNT) ||
            (NULL_PTR != uint32_a_elapsed_time)
            )
    {
        en_gpt_status_retval = GPT_INVALID_ARGS;
    }
    else
    {
        uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(en_a_gpt_channel);
        if(0 == uint32_l_base_address)
        {
            en_gpt_status_retval = GPT_ERROR;
        }
        else
        {
            uint32_t_ uint32_l_elapsed =
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAILR_REG_OFFSET) -
                    GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAV_REG_OFFSET);

            // update [out] return pointer
            *uint32_a_elapsed_time = uint32_l_elapsed;
        }
    }

    return en_gpt_status_retval;
}

en_gpt_status_t gpt_get_remaining_time(en_gpt_channel_t en_a_gpt_channel,  uint32_t_ * uint32_a_rem_time)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;
    if(
            (en_a_gpt_channel >= GPT_CONFIGURED_TIMERS_CHS_COUNT) ||
            (NULL_PTR != uint32_a_rem_time)
            )
    {
        en_gpt_status_retval = GPT_INVALID_ARGS;
    }
    else
    {
        uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(en_a_gpt_channel);
        if(0 == uint32_l_base_address)
        {
            en_gpt_status_retval = GPT_ERROR;
        }
        else
        {
            // get remaining time from TAV register (Timer A Value Register)
            uint32_t_ uint32_l_rem = GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMTAV_REG_OFFSET);

            // update [out] return pointer
            * uint32_a_rem_time = uint32_l_rem;
        }
    }

    return en_gpt_status_retval;
}


en_gpt_status_t gpt_set_callback(en_gpt_channel_t en_a_gpt_channel, ptr_vd_fun_vd_t ptr_vd_fun_vd_a_gpt_notification)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    // arguments check
    if(
            (en_a_gpt_channel > GPT_CONFIGURED_TIMERS_CHS_COUNT) ||
            (NULL_PTR == ptr_vd_fun_vd_a_gpt_notification)
    )
    {
        en_gpt_status_retval = GPT_INVALID_ARGS;
    }
    else
    {
        gl_callbacks_array[en_a_gpt_channel] = ptr_vd_fun_vd_a_gpt_notification;
    }

    return en_gpt_status_retval;
}



en_gpt_status_t gpt_enable_notification(en_gpt_channel_t en_a_gpt_channel)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    // get base address
    uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(gl_st_gpt_lconfig_arr[en_a_gpt_channel].en_gpt_channel_id);

    if(0 != uint32_l_base_address)
    {
        // enable GPTM Interrupt mask register GPTMIMR
        SET_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMIMR_REG_OFFSET), GPTMIMR_TIMER_A_TIMEOUT_INT_MASK_BIT);

        // Enable NVIC Timer IRQ
        switch (en_a_gpt_channel) {

            case CH_0:
            {
                NVIC_EnableIRQ(TIMER0A_IRQn);
                break;
            }
            case CH_1:
            {
                NVIC_EnableIRQ(TIMER1A_IRQn);
                break;
            }
            case CH_2:
            {
                NVIC_EnableIRQ(TIMER2A_IRQn);
                break;
            }
            case CH_3:
            {
                NVIC_EnableIRQ(TIMER3A_IRQn);
                break;
            }
            case CH_4:
            {
                NVIC_EnableIRQ(TIMER4A_IRQn);
                break;
            }
            case CH_5:
            {
                NVIC_EnableIRQ(TIMER5A_IRQn);
                break;
            }
            case CH_6_W:
            {
                NVIC_EnableIRQ(WTIMER0A_IRQn);
                break;
            }
            case CH_7_W:
            {
                NVIC_EnableIRQ(WTIMER1A_IRQn);
                break;
            }
            case CH_8_W:
            {
                NVIC_EnableIRQ(WTIMER2A_IRQn);
                break;
            }
            case CH_9_W:
            {
                NVIC_EnableIRQ(WTIMER3A_IRQn);
                break;
            }
            case CH_10_W:
            {
                NVIC_EnableIRQ(WTIMER4A_IRQn);
                break;
            }
            case CH_11_W:
            {
                NVIC_EnableIRQ(WTIMER5A_IRQn);
                break;
            }
            case CH_TOTAL:
            default:
                en_gpt_status_retval = GPT_ERROR;
                break;
        }
        // enable IRQ
        if(en_gpt_status_retval != GPT_ERROR) __enable_irq();
    }
    else
    {
        en_gpt_status_retval = GPT_ERROR;
    }

    return en_gpt_status_retval;
}
en_gpt_status_t gpt_disable_notification(en_gpt_channel_t en_a_gpt_channel)
{
    en_gpt_status_t en_gpt_status_retval = GPT_OK;

    // get base address
    uint32_t_ uint32_l_base_address = gpt_get_timer_base_address(gl_st_gpt_lconfig_arr[en_a_gpt_channel].en_gpt_channel_id);

    if(0 != uint32_l_base_address)
    {
        // disable GPTM Interrupt mask register GPTMIMR
        CLR_BIT(GET_ADDRESS_FROM_OFFSET(uint32_l_base_address, GPTMIMR_REG_OFFSET), GPTMIMR_TIMER_A_TIMEOUT_INT_MASK_BIT);
    }
    else
    {
        en_gpt_status_retval = GPT_ERROR;
    }

    return en_gpt_status_retval;
}

/* Private Functions */
/**
 * @brief                           :   Returns base address for a certain timer channel
 * @param[in]   en_a_gpt_channel    :   timer channel
 *
 * @return      Base Address        :   If Success
 *              NULL                :   If Failed
 *
 */
static uint32_t_ gpt_get_timer_base_address(en_gpt_channel_t en_a_gpt_channel)
{
    uint32_t_ uint32_base_address_retval = 0;

    switch (en_a_gpt_channel) {

        case CH_0:
        {
            uint32_base_address_retval = TIMER_0_BASE_ADDR;
            break;
        }
        case CH_1:
        {
            uint32_base_address_retval = TIMER_1_BASE_ADDR;
            break;
        }
        case CH_2:
        {
            uint32_base_address_retval = TIMER_2_BASE_ADDR;
            break;
        }
        case CH_3:
        {
            uint32_base_address_retval = TIMER_3_BASE_ADDR;
            break;
        }
        case CH_4:
        {
            uint32_base_address_retval = TIMER_4_BASE_ADDR;
            break;
        }
        case CH_5:
        {
            uint32_base_address_retval = TIMER_5_BASE_ADDR;
            break;
        }
        case CH_6_W:
        {
            uint32_base_address_retval = WIDE_TIMER_0_BASE_ADDR;
            break;
        }
        case CH_7_W:
        {
            uint32_base_address_retval = WIDE_TIMER_1_BASE_ADDR;
            break;
        }
        case CH_8_W:
        {
            uint32_base_address_retval = WIDE_TIMER_2_BASE_ADDR;
            break;
        }
        case CH_9_W:
        {
            uint32_base_address_retval = WIDE_TIMER_3_BASE_ADDR;
            break;
        }
        case CH_10_W:
        {
            uint32_base_address_retval = WIDE_TIMER_4_BASE_ADDR;
            break;
        }
        case CH_11_W:
        {
            uint32_base_address_retval = WIDE_TIMER_5_BASE_ADDR;
            break;
        }
        case CH_TOTAL:
        default:
        {
            uint32_base_address_retval = 0;
            break;
        }
    }

    return uint32_base_address_retval;
}

/* GPT ISRs */

// Timer-0 Handler
void TIMER0A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_0])
    {
        gl_callbacks_array[CH_0]();
    }

    // clear interrupt flag
    // set bit to clear flag
    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_0_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Timer-1 Handler
void TIMER1A_Handler(void)
{
    if((NULL_PTR != gl_callbacks_array[CH_1]))
    {
        gl_callbacks_array[CH_1]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_1_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Timer-2 Handler
void TIMER2A_Handler(void)
{
    if((NULL_PTR != gl_callbacks_array[CH_2]))
    {
        gl_callbacks_array[CH_2]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_2_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Timer-3 Handler
void TIMER3A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_3])
    {
        gl_callbacks_array[CH_3]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_3_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Timer-4 Handler
void TIMER4A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_4])
    {
        gl_callbacks_array[CH_4]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_4_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Timer-5 Handler
void TIMER5A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_5])
    {
        gl_callbacks_array[CH_5]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(TIMER_5_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-0 Handler
void WTIMER0A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_6_W])
    {
        gl_callbacks_array[CH_6_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_0_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-1 Handler
void WTIMER1A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_7_W])
    {
        gl_callbacks_array[CH_7_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_1_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-2 Handler
void WTIMER2A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_8_W])
    {
        gl_callbacks_array[CH_8_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_2_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-3 Handler
void WTIMER3A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_9_W])
    {
        gl_callbacks_array[CH_9_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_3_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-4 Handler
void WTIMER4A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_10_W])
    {
        gl_callbacks_array[CH_10_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_4_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}

// Wide Timer-5 Handler
void WTIMER5A_Handler(void)
{
    if(NULL_PTR != gl_callbacks_array[CH_11_W])
    {
        gl_callbacks_array[CH_11_W]();
    }

    SET_BIT(GET_ADDRESS_FROM_OFFSET(WIDE_TIMER_5_BASE_ADDR, GPTMICR_REG_OFFSET), GPTMICR_TATOCINT_BIT);
}
