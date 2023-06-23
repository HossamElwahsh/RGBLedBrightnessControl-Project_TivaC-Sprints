/**
 * @file    :   app.c
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Program File contains all APP functions' implementation
 * @version :   0.1
 * @date    :   2023-06-17
 *
 * @copyright Copyright (c) 2023
 */

/*
 * Includes */
#include "app.h"
#include "led_interface.h"
#include "btn_interface.h"
#include "pwm_interface.h"

/*
 * Private Typedefs */
typedef enum{
    SUB_STATE_1_DUTY_IN_PERCENT = 30 ,
    SUB_STATE_2_DUTY_IN_PERCENT = 60 ,
    SUB_STATE_3_DUTY_IN_PERCENT = 90 ,
    SUB_STATES_TOTAL
}en_app_sub_state_t;

typedef enum{
    RUNNING        = 0  ,   // Running
    SWITCHING           ,   // Switching sub-state
    STARTED             ,   // switched and started successfully
    STATES_TOTAL
}en_app_state_t;

/*
 * Private MACROS/Defines */
#define LED_BLINK_TOT_MS_PERIOD_DURATION  500.0   // 500 ms

#define RGB_LEDS_COUNT 3
#define RGB_LED_RED_INDEX 0
#define RGB_LED_BLUE_INDEX 1
#define RGB_LED_GREEN_INDEX 2

#define RED_LED_PORT 		LED_PORT_F // Port F
#define RED_LED_PIN			LED_PIN_1

#define GREEN_LED_PORT 	    LED_PORT_F // Port F
#define GREEN_LED_PIN		LED_PIN_3

#define BLUE_LED_PORT 	    LED_PORT_F // Port F
#define BLUE_LED_PIN		LED_PIN_2

#define USER_BTN_PORT		BTN_PORT_F // Port F
#define USER_BTN_PIN		BTN_PIN_4


typedef struct{
    en_led_port_t_ en_led_port;
    en_led_pin_t_ en_led_pin;
}st_app_led_t;

static st_app_led_t gl_st_app_leds[RGB_LEDS_COUNT] =
{
        {RED_LED_PORT, RED_LED_PIN},
        {BLUE_LED_PORT, BLUE_LED_PIN},
        {GREEN_LED_PORT, GREEN_LED_PIN}
};

/*
 * Private Variables */
static en_app_state_t gl_en_app_state = SWITCHING;
static en_app_sub_state_t gl_en_app_sub_state = SUB_STATE_1_DUTY_IN_PERCENT;
static uint8_t_ gl_u8_current_led_idx = RGB_LED_GREEN_INDEX;

static st_btn_config_t_ gl_st_user_btn_cfg = {
        .en_btn_port = USER_BTN_PORT,
        .en_btn_pin  = USER_BTN_PIN,
        .en_btn_activation = BTN_ACTIVATED,
        .en_btn_pull_type = BTN_INTERNAL_PULL_UP
};

static void app_next_state(void);

/**
 * @brief                      : Initializes the required modules by the app
 *
 * @return  APP_OK              :   In case of Successful Operation
 *          APP_FAIL            :   In case of Failed Operation
 */
en_app_error_t app_init(void)
{
    en_app_error_t en_app_error_retval = APP_OK;
    en_btn_status_code_t_ en_btn_status_code = BTN_STATUS_OK;
    en_led_error_t_ en_led_error = LED_OK;

    /* init RGB LEDS */
    for (int i = ZERO; i < RGB_LEDS_COUNT; ++i) {
        en_led_error = led_init(gl_st_app_leds[i].en_led_port, gl_st_app_leds[i].en_led_pin);
        if(LED_OK != en_led_error) en_app_error_retval = APP_FAIL;
    }

    // init button
    en_btn_status_code = btn_init(&gl_st_user_btn_cfg);
    if(BTN_STATUS_OK != en_btn_status_code) en_app_error_retval = APP_FAIL;

    /* Init PWM module */
    en_pwm_error_t en_pwm_status = pwm_init();
    if(PWM_OK != en_pwm_status) en_app_error_retval = APP_FAIL;


    return en_app_error_retval;
}

void app_start(void)
{
    while(TRUE)
    {
        en_btn_state_t_ en_btn_state = BTN_STATE_NOT_PRESSED;
        btn_read(&gl_st_user_btn_cfg, &en_btn_state);

        if(BTN_STATE_PRESSED == en_btn_state)
        {
            // set app state -> switching
            gl_en_app_state = SWITCHING;

            switch (gl_en_app_sub_state) {
                case SUB_STATE_1_DUTY_IN_PERCENT:
                    gl_en_app_sub_state = SUB_STATE_2_DUTY_IN_PERCENT;
                    break;

                case SUB_STATE_2_DUTY_IN_PERCENT:
                    gl_en_app_sub_state = SUB_STATE_3_DUTY_IN_PERCENT;
                    break;

                case SUB_STATE_3_DUTY_IN_PERCENT:
                case SUB_STATES_TOTAL:
                default:
                    gl_en_app_sub_state = SUB_STATE_1_DUTY_IN_PERCENT;
                    break;
            }

            

        }
        else
        {
            /* Do Nothing */
        }

        switch (gl_en_app_state) {

            case RUNNING:
            {
                /* Do Nothing */
                break;
            }
            case SWITCHING:
            {
                app_next_state();

                gl_en_app_state = RUNNING;
                break;
            }
//            case STARTED:
//            {
//                // initial start delay finished
//                // turn off LED, start time on delay timer for every upcoming period
//
//                led_off(gl_st_app_leds[gl_u8_current_led_idx].en_led_port, gl_st_app_leds[gl_u8_current_led_idx].en_led_pin);
//                gpt_start(TIMER_ON_TIMER,
//                          (uint32_t_)(LED_BLINK_TOT_MS_PERIOD_DURATION),
//                          TIME_IN_MS);
//
//                gl_en_app_state = RUNNING;
//                break;
//            }
            case STATES_TOTAL:
            default:
            {
                /* Bad state reset to idle */
                gl_en_app_state  = SWITCHING;
                break;
            }
        }

    }
}

static void app_next_state(void)
{
    // stop timers
    pwm_stop((en_pwm_channel_id_t)gl_u8_current_led_idx);

    // turn off previous color led
    led_off(gl_st_app_leds[gl_u8_current_led_idx].en_led_port, gl_st_app_leds[gl_u8_current_led_idx].en_led_pin);

    // next state led color
    gl_u8_current_led_idx = INC_WITH_MOD(gl_u8_current_led_idx, RGB_LEDS_COUNT);

    // turn on next state color
    //led_on(gl_st_app_leds[gl_u8_current_led_idx].en_led_port, gl_st_app_leds[gl_u8_current_led_idx].en_led_pin);

		pwm_adjust_signal((en_pwm_channel_id_t)gl_u8_current_led_idx, gl_en_app_sub_state, LED_BLINK_TOT_MS_PERIOD_DURATION);
    pwm_start((en_pwm_channel_id_t)gl_u8_current_led_idx);
}
