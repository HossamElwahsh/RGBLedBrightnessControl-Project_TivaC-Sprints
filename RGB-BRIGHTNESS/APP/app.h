/**
 * @file    :   app.h
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Header File contains all APP typedefs and functions' prototypes
 * @version :   0.1
 * @date    :   2023-06-17
 *
 * @copyright Copyright (c) 2023
 */

#ifndef APP_H_
#define APP_H_

typedef enum{
    APP_OK,
    APP_FAIL
}en_app_error_t;


/**
 * @brief                       : Initializes the required modules by the app
 *
 * @return  APP_OK              :   In case of Successful Operation
 *          APP_FAIL            :   In case of Failed Operation
 */
en_app_error_t app_init(void);


/**
 * @brief                       :   Starts the application program and keeps it running indefinitely
 */
void app_start(void);

#endif
