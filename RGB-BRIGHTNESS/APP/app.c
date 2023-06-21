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
#include "systick_interface.h"
#include "systick_linking_config.h"

/*
 * Private Typedefs */
typedef enum{
    ALL_OFF = 0 ,
    RED_LED      ,
    GREEN_LED    ,
    BLUE_LED     ,
    ALL_LEDS      ,
    SUB_STATES_TOTAL
}en_app_sub_state_t;

typedef enum{
    IDLE        = 0 ,   // Idle Doing nothing
    SWITCHING       ,   // Switching sub-state
    TURNING_OFF     ,   // Turn off LEDs
    STATES_TOTAL
}en_app_state_t;

/*
 * Private MACROS/Defines */
#define RED_LED_PORT 		LED_PORT_F // Port F
#define RED_LED_PIN			LED_PIN_1

#define GREEN_LED_PORT 	LED_PORT_F // Port F
#define GREEN_LED_PIN		LED_PIN_3

#define BLUE_LED_PORT 	LED_PORT_F // Port F
#define BLUE_LED_PIN		LED_PIN_2

#define USER_BTN_PORT		BTN_PORT_F // Port F
#define USER_BTN_PIN		BTN_PIN_4

#define LED_BLINK_DURATION  1000
/*
 * Private Variables */
static en_app_state_t gl_en_app_state = IDLE;
static en_app_sub_state_t gl_en_app_sub_state = ALL_OFF;

static st_btn_config_t_ gl_st_user_btn_cfg = {
        .en_btn_port = USER_BTN_PORT,
        .en_btn_pin  = USER_BTN_PIN,
        .en_btn_activation = BTN_ACTIVATED,
        .en_btn_pull_type = BTN_INTERNAL_PULL_UP
};

static void app_switch_state(void);
static void app_systick_cb(void);

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
    en_systick_error_t en_systick_error = ST_OK;

    /* init RGB LED */

    // init RED LED
    en_led_error = led_init(RED_LED_PORT, RED_LED_PIN);
    if(LED_OK != en_led_error) en_app_error_retval = APP_FAIL;

    // init blue led
    en_led_error = led_init(GREEN_LED_PORT, GREEN_LED_PIN);
    if(LED_OK != en_led_error) en_app_error_retval = APP_FAIL;

    // init green LED
    en_led_error = led_init(BLUE_LED_PORT, BLUE_LED_PIN);
    if(LED_OK != en_led_error) en_app_error_retval = APP_FAIL;

    // init button
    en_btn_status_code = btn_init(&gl_st_user_btn_cfg);
    if(BTN_STATUS_OK != en_btn_status_code) en_app_error_retval = APP_FAIL;

    // Init Systick
    en_systick_error = systick_init(&gl_st_systick_cfg_0);
    if(ST_OK != en_systick_error) en_app_error_retval = APP_FAIL;

    en_systick_error = systick_set_callback(&app_systick_cb);
    if(ST_OK != en_systick_error) en_app_error_retval = APP_FAIL;

    return en_app_error_retval;
}

void app_start(void)
{
    while(1)
    {
        en_btn_state_t_ en_btn_state = BTN_STATE_NOT_PRESSED;
        btn_read(&gl_st_user_btn_cfg, &en_btn_state);

        if(BTN_STATE_PRESSED == en_btn_state)
        {
            if(ALL_LEDS == gl_en_app_sub_state)
            {
                gl_en_app_sub_state = ALL_OFF;
            }
            else
            {
                gl_en_app_sub_state += 1;
            }

            // set app state -> switching
            gl_en_app_state = SWITCHING;
        }
        else
        {
            /* Do Nothing */
        }

        switch (gl_en_app_state) {

            case IDLE:
            {
                /* Do Nothing */
                break;
            }
            case SWITCHING:
            {
                app_switch_state();
                break;
            }
            case TURNING_OFF:
            {
                led_off(RED_LED_PORT, RED_LED_PIN);
                led_off(GREEN_LED_PORT, GREEN_LED_PIN);
                led_off(BLUE_LED_PORT, BLUE_LED_PIN);
                gl_en_app_state = IDLE;
                break;
            }
            case STATES_TOTAL:
            default:
            {
                /* Bad state reset to idle */
                gl_en_app_state  = IDLE;
                break;
            }
        }

    }
}

static void app_switch_state(void)
{
    switch (gl_en_app_sub_state) {

        case ALL_OFF:
        {
            led_off(RED_LED_PORT, RED_LED_PIN);
            led_off(GREEN_LED_PORT, GREEN_LED_PIN);
            led_off(BLUE_LED_PORT, BLUE_LED_PIN);
            break;
        }
        case RED_LED:
        {
            led_on(RED_LED_PORT, RED_LED_PIN);
            systick_async_ms_delay(LED_BLINK_DURATION);
            break;
        }
        case GREEN_LED:
        {
            led_off(RED_LED_PORT, RED_LED_PIN);
            led_on(GREEN_LED_PORT, GREEN_LED_PIN);
            systick_async_ms_delay(LED_BLINK_DURATION);
            break;
        }
        case BLUE_LED:
        {
            led_off(GREEN_LED_PORT, GREEN_LED_PIN);
            led_on(BLUE_LED_PORT, BLUE_LED_PIN);
            systick_async_ms_delay(LED_BLINK_DURATION);
            break;
        }
        case ALL_LEDS:
        {
            led_on(RED_LED_PORT, RED_LED_PIN);
            led_on(GREEN_LED_PORT, GREEN_LED_PIN);
            led_on(BLUE_LED_PORT, BLUE_LED_PIN);
            systick_async_ms_delay(LED_BLINK_DURATION);
            break;
        }
        case SUB_STATES_TOTAL:
        default:
        {
            // bad state, reset to ALL_OFF
            gl_en_app_sub_state = ALL_OFF;
            break;
        }
    }

    // switching complete - back to IDLE
    gl_en_app_state = IDLE;
}

static void app_systick_cb(void)
{
    gl_en_app_state = TURNING_OFF;
}
