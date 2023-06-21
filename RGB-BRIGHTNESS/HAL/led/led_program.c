/**
 * @file    :   led_program.c
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Program File contains all LED functions' implementation
 * @version :   2.0
 * @date    :   2023-05-29
 *
 * @copyright Copyright (c) 2023
 */

#include "led_interface.h"

// private includes
#include "gpio_interface.h"

/**
 * @brief                       :   Initializes LED on given port & pin
 *
 * @param[in]   en_a_led_port    :   LED Port
 * @param[in]   en_a_led_pin     :   LED Pin number in en_a_led_port
 *
 * @return  LED_OK              :   In case of Successful Operation
 *          LED_ERROR           :   In case of Failed Operation
 */
en_led_error_t_ led_init(en_led_port_t_ en_a_led_port, en_led_pin_t_ en_a_led_pin)
{
    en_led_error_t_ en_led_error_retval = LED_OK;
    if((LED_PORT_TOTAL <= en_a_led_port) || (LED_PIN_TOTAL <= en_a_led_pin))
    {
        en_led_error_retval = LED_ERROR;
    }
    else
    {
        // prepare pin cfg
        st_gpio_cfg_t st_gpio_cfg_led = {
            .port = (en_gpio_port_t) en_a_led_port,
            .pin = (en_gpio_pin_t) en_a_led_pin,
            .current = PIN_CURRENT_2MA,
            .pin_cfg = OUTPUT
        };
        en_gpio_error_t en_dio_error = gpio_pin_init(&st_gpio_cfg_led);

        en_led_error_retval = (en_dio_error == GPIO_OK ? LED_OK : LED_ERROR);
    }

    return en_led_error_retval;
}

/**
 * @brief                       :   Turns on LED at given port/pin
 *
 * @param[in]   en_a_led_port    :   LED Port
 * @param[in]   en_a_led_pin     :   LED Pin number in en_a_led_port
 *
 * @return  LED_OK              :   In case of Successful Operation
 *          LED_ERROR           :   In case of Failed Operation
 */
en_led_error_t_ led_on(en_led_port_t_ en_a_led_port, en_led_pin_t_ en_a_led_pin)
{
    en_led_error_t_ en_led_error_retval = LED_OK;
    if((LED_PORT_TOTAL <= en_a_led_port) || (LED_PIN_TOTAL <= en_a_led_pin))
    {
        en_led_error_retval = LED_ERROR;
    }
    else
    {
        en_gpio_error_t en_dio_error = gpio_setPinVal((en_gpio_port_t) en_a_led_port,
                                                       (en_gpio_pin_t) en_a_led_pin,
                                                       HIGH);

        en_led_error_retval = (en_dio_error != GPIO_OK ? LED_ERROR : LED_OK);
    }

    return en_led_error_retval;
}

/**
 * @brief                       :   Turns off LED at given port/pin
 *
 * @param[in]   en_a_led_port    :   LED Port
 * @param[in]   en_a_led_pin     :   LED Pin number in en_a_led_port
 *
 * @return  LED_OK              :   In case of Successful Operation
 *          LED_ERROR           :   In case of Failed Operation
 */
en_led_error_t_ led_off(en_led_port_t_ en_a_led_port, en_led_pin_t_ en_a_led_pin)
{
    en_led_error_t_ en_led_error_retval = LED_OK;

    if((LED_PORT_TOTAL <= en_a_led_port) || (LED_PIN_TOTAL <= en_a_led_pin))
    {
        en_led_error_retval = LED_ERROR;
    }
    else
    {
        en_gpio_error_t en_dio_error = gpio_setPinVal((en_gpio_port_t) en_a_led_port,
                                                       (en_gpio_pin_t) en_a_led_pin,
                                                       LOW);
        en_led_error_retval = (en_dio_error != GPIO_OK ? LED_ERROR : LED_OK);
    }

    return en_led_error_retval;
}

/**
 * @brief                       :   Toggles LED at given port/pin
 *
 * @param[in]   en_a_led_port    :   LED Port
 * @param[in]   en_a_led_pin     :   LED Pin number in en_a_led_port
 *
 * @return  LED_OK              :   In case of Successful Operation
 *          LED_ERROR           :   In case of Failed Operation
 */
en_led_error_t_ led_toggle(en_led_port_t_ en_a_led_port, en_led_pin_t_ en_a_led_pin)
{
    en_led_error_t_ en_led_error_retval = LED_OK;

    if((LED_PORT_TOTAL <= en_a_led_port) || (LED_PIN_TOTAL <= en_a_led_pin))
    {
        en_led_error_retval = LED_ERROR;
    }
    else
    {
        en_gpio_error_t en_dio_error = gpio_togPinVal((en_gpio_port_t) en_a_led_port,
                                                    (en_gpio_pin_t) en_a_led_pin);
        en_led_error_retval = (en_dio_error != GPIO_OK ? LED_ERROR : LED_OK);
    }

    return en_led_error_retval;
}
