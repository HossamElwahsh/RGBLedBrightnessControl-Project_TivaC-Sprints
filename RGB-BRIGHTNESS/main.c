/**
 ** @file      main.c
 ** @brief     
 ** @author    
 ** @date      June 16, 2023
 ** @version   0.1
 */

/*- INCLUDES
----------------------------------------------*/
#include "app.h"

//#define TEST 1
#if TEST
#include "systick_interface.h"
#include "systick_linking_config.h"
#include "led_interface.h"
#include "gpt_interface.h"
#include "gpt_linking_cfg.h"

#define RED_LED_PORT 		LED_PORT_F // Port F
#define RED_LED_PIN			LED_PIN_1

static boolean gl_bool_led_on = TRUE;

    void test_systick_sync();
    void test_systick_async();
    void test_systick_cb_me();
    void test_gpt_delay();
    void test_gpt_cb_total_time();
    void test_gpt_cb_on_time();
#endif

int main(void)
{

#if TEST
//    test_systick_sync();
//    test_systick_async();
    led_init(RED_LED_PORT, RED_LED_PIN);
    led_on(RED_LED_PORT, RED_LED_PIN);

    test_gpt_delay();

    while(1)
    {
        // superloop
    }

    return 0;
#else

    en_app_error_t en_app_error = APP_OK;

    // initialize app
    en_app_error = app_init();

    if(APP_OK == en_app_error)
    {
        app_start();
    }
    else
    {
        // Do Nothing
        while(1)
        {
            // Do Nothing
        }
    }


	return 0;

#endif
}
#if TEST
void test_systick_sync()
{
    en_systick_error_t en_systick_error =  systick_init(&gl_st_systick_cfg_0);
    en_systick_error = systick_ms_delay(1000);

    while(1)
    {

    }

}

void test_systick_async()
{
    en_systick_error_t en_systick_error = systick_init(&gl_st_systick_cfg_0);
    en_systick_error = systick_set_callback(&test_systick_cb_me);
    en_systick_error = systick_async_ms_delay(1000);

    while(1)
    {

    }
}

void test_gpt_delay()
{
    gpt_init(gl_st_gpt_lconfig_arr);
//    gpt_set_callback(CH_0, test_gpt_cb_on_time);
    gpt_set_callback(CH_1, test_gpt_cb_total_time);
//    gpt_start(CH_0, 150, TIME_IN_MS);
    gpt_start(CH_1, 1, TIME_IN_SEC);
}

void test_gpt_cb_total_time()
{
//    led_on(RED_LED_PORT, RED_LED_PIN);b
    if(gl_bool_led_on == TRUE)
    {
        gl_bool_led_on = FALSE;
        led_off(RED_LED_PORT, RED_LED_PIN);
    }
    else
    {
        gl_bool_led_on = TRUE;
        led_on(RED_LED_PORT, RED_LED_PIN);
    }
}

void test_gpt_cb_on_time()
{
    led_off(RED_LED_PORT, RED_LED_PIN);

    /*
    if(gl_bool_led_on == TRUE)
    {
        gl_bool_led_on = FALSE;
        led_off(RED_LED_PORT, RED_LED_PIN);
    }
    else
    {
        gl_bool_led_on = TRUE;
        led_on(RED_LED_PORT, RED_LED_PIN);
    }*/

//    gl_bool_led_on = !gl_bool_led_on; // toggle state
}

void test_systick_cb_me()
{
    // callback from systick
    while(1)
    {

    }
}
#endif