/**
 * @file    :   systick_private.c
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Header File contains all SYSTICK private typedefs and functions' prototypes
 * @version :   0.1
 * @date    :   2023-06-17
 *
 * @copyright Copyright (c) 2023
 */

#ifndef SYSTICK_PRIVATE_H
#define SYSTICK_PRIVATE_H

#define CORE_PERIPHERALS_BASE_ADDRESS 0xE000E000

/**
 * BRIEF    :   SysTick Control and Status Register
 * ACCESS   :   R/W
 * RESET    :   0x0000.0004
 */
#define STCTRL					*((volatile uint32_t_*) (CORE_PERIPHERALS_BASE_ADDRESS + 0x010))

// STCTRL BITS
#define STCTRL_COUNT        16
#define STCTRL_ENABLE       0
#define STCTRL_INT_ENABLE   1
#define STCTRL_CLK_SRC      2

#define STLOAD_MIN_VALUE 0x00000001 // 24-bits countdown timer min value
#define STLOAD_MAX_VALUE 0x00FFFFFF // 24-bits countdown timer max value

/**
 * BRIEF    :   SysTick Reload Value Register
 * WIDTH    :   24-BITS
 * ACCESS   :   R/W (can only be accessed from privileged mode)
 * RESET    :   -
 * IMP NOTE :   in order to access this register correctly,
 *              the system clock must be faster than 8 MHz
 */
#define STRELOAD				*((volatile uint32_t_*) (CORE_PERIPHERALS_BASE_ADDRESS + 0x014))

/**
 * BRIEF    :   SysTick Current Value Register
 * WIDTH    :   24-BITS
 * ACCESS   :   R/W/C
 * RESET    :   -
 */
#define STCURRENT				*((volatile uint32_t_*) (CORE_PERIPHERALS_BASE_ADDRESS + 0x018))

#endif //SYSTICK_PRIVATE_H
