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
#include "systick_interface.h"
#include "systick_linking_config.h"

//#define TEST 1
#if TEST
    void test_systick_sync();
    void test_systick_async();
    void test_systick_cb_me();
#endif

int main(void)
{

#if TEST
    //void test_systick_sync();
    void test_systick_async();
    return 0;
#endif
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

void test_systick_cb_me()
{
    // callback from systick
    while(1)
    {

    }
}
#endif