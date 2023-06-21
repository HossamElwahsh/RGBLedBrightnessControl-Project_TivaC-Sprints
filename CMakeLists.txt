cmake_minimum_required(VERSION 3.25)
project(shared C)

set(CMAKE_C_STANDARD 11)

include_directories(RGB-BRIGHTNESS)
include_directories(RGB-BRIGHTNESS/APP)
include_directories(RGB-BRIGHTNESS/HAL)
include_directories(RGB-BRIGHTNESS/HAL/led)
include_directories(RGB-BRIGHTNESS/HAL/btn)
include_directories(RGB-BRIGHTNESS/LIB)
include_directories(RGB-BRIGHTNESS/MCAL)
include_directories(RGB-BRIGHTNESS/MCAL/gpio)
include_directories(RGB-BRIGHTNESS/MCAL/systick)
include_directories(RGB-BRIGHTNESS/RTE/_Target_1)

add_executable(shared

        RGB-BRIGHTNESS/APP/app.c
        RGB-BRIGHTNESS/APP/app.h
        RGB-BRIGHTNESS/HAL/led/led_interface.h
        RGB-BRIGHTNESS/HAL/led/led_program.c
        RGB-BRIGHTNESS/LIB/bit_math.h
        RGB-BRIGHTNESS/LIB/std.h
        RGB-BRIGHTNESS/MCAL/gpio/gpio_interface.h
        RGB-BRIGHTNESS/MCAL/gpio/gpio_private.h
        RGB-BRIGHTNESS/MCAL/gpio/gpio_program.c
        RGB-BRIGHTNESS/RTE/_Target_1/RTE_Components.h
        RGB-BRIGHTNESS/RTE/Device/TM4C123GH6PM/system_TM4C123.c
        RGB-BRIGHTNESS/led_interface.h
        RGB-BRIGHTNESS/main.c
        RGB-BRIGHTNESS/MCAL/systick/systick_program.c
        RGB-BRIGHTNESS/HAL/btn/btn_program.c RGB-BRIGHTNESS/MCAL/systick/systick_linking_config.c RGB-BRIGHTNESS/MCAL/systick/systick_linking_config.h RGB-BRIGHTNESS/MCAL/gpt/gpt_program.c RGB-BRIGHTNESS/MCAL/gpt/gpt_interface.h RGB-BRIGHTNESS/MCAL/gpt/gpt_private.h RGB-BRIGHTNESS/MCAL/gpt/gpt_linking_cfg.c RGB-BRIGHTNESS/MCAL/gpt/gpt_linking_cfg.h)
