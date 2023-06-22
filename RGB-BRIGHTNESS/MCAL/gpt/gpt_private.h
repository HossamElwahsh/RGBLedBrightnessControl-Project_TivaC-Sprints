/**
 * @file    :   gpt_private.c
 * @author  :   Hossam Elwahsh - https://github.com/HossamElwahsh
 * @brief   :   Header File contains all GPT private typedefs and functions' prototypes
 * @version :   0.1
 * @date    :   2023-06-21
 *
 * @copyright Copyright (c) 2023
 */

#ifndef GPT_PRIVATE_H
#define GPT_PRIVATE_H
#include "std.h"

/* Helper Macros
 * -----------------------------------------------------*/
#define GET_ADDRESS_FROM_OFFSET(BASE,OFFSET) *((volatile uint32_t_ *) (BASE + OFFSET))

#define PRESCALER_FINDER_HELPER(MAX_COUNTS,DESIRED_DELAY) ((((f64_t_)(((f64_t_)DESIRED_DELAY * MC_F_CPU_HZ)/(MAX_COUNTS)))))

#define CEIL_NUMBER(x) ((x - (int)(x)) > 0 ? (int)(x + 1.0) : (int)(x))

/* Registers
 * -----------------------------------------------------*/

/* Gating Control Registers
 * -----------------------------------------------------*/

/* 16/32 bits Timers Clock Gating Control */
#define RCGCTIMER_REG_ADDRESS    *(( volatile uint32_t_ *)(0x400FE604))

/* 32/64 bits Wide Timers Clock Gating Control */
#define RCGCWTIMER_REG_ADDRESS    *(( volatile uint32_t_ *)(0x400FE65C))

/* Timers Base addresses
 * -----------------------------------------------------*/
#define     TIMER_0_BASE_ADDR       0x40030000
#define     TIMER_1_BASE_ADDR       0x40031000
#define     TIMER_2_BASE_ADDR       0x40032000
#define     TIMER_3_BASE_ADDR       0x40033000
#define     TIMER_4_BASE_ADDR       0x40034000
#define     TIMER_5_BASE_ADDR       0x40035000
#define     WIDE_TIMER_0_BASE_ADDR  0x40036000
#define     WIDE_TIMER_1_BASE_ADDR  0x40037000
#define     WIDE_TIMER_2_BASE_ADDR  0x4003C000
#define     WIDE_TIMER_3_BASE_ADDR  0x4003D000
#define     WIDE_TIMER_4_BASE_ADDR  0x4003E000
#define     WIDE_TIMER_5_BASE_ADDR  0x4003F000

/* Timer Registers
 * -----------------------------------------------------*/

/* GPTM Configuration
 * Default  :   0x0000.0000
 * */
#define     GPTMCFG_REG_OFFSET          0x000

/* SET: no concat, timer is 16 bit, wide timer is 32 bit */
#define     GPTMCFG_NOT_CONCAT_BIT      2

/* GPTM Timer A Mode
 * Default  :   0x0000.0000
 * */			
#define     GPTMTAMR_REG_OFFSET         					0x004

#define     GPTMTXMR_ONE_SHOT_BIT         					0
#define     GPTMTXMR_PERIODIC_BIT         					1

/* CLEAR    :   Count Down
 * SET      :   Count Up
 * */
#define     GPTMTXMR_DIR_BIT         					    4

/* GPTM Timer B Mode
 * Default  :   0x0000.0000
 * */
#define     GPTMTBMR_REG_OFFSET         					0x008

/* GPTM Control
 * Default  :   0x0000.0000
 * */
#define     GPTMCTL_REG_OFFSET										0x00C

#define     GPTMCTRL_A_EN_BIT        0

/* if set Timer stops counting if processor is halted by the debugger */
#define     GPTMCTRL_A_STALL_BIT     1
#define     GPTMCTRL_B_EN_BIT        8

/* if set Timer stops counting if processor is halted by the debugger */
#define     GPTMCTRL_B_STALL_BIT     9



/* GPTM Synchronize
 * Default  :   0x0000.0000
 * */
#define     GPTMSYNC_REG_OFFSET        						0x010

/* GPTM Interrupt Mask
 * Default  :   0x0000.0000
 * */
#define     GPTMIMR_REG_OFFSET                     0x018

#define GPTMIMR_TIMER_B_TIMEOUT_INT_MASK_BIT    8
#define GPTMIMR_TIMER_A_TIMEOUT_INT_MASK_BIT    0

/* GPTM Raw Interrupt Status
 * Default  :   0x0000.0000
 * */
#define     GPTMRIS_REG_OFFSET                     0x01C

/* TIMER_A_TIME_OUT_RAW_INT */
#define     GPTMRIS_TATORIS 0
#define     GPTMRIS_TBTORIS 8

/* GPTM Masked Interrupt Status
 * Default  :   0x0000.0000
 * */
#define     GPTMMIS_REG_OFFSET                     0x020

/* GPTM Interrupt Clear
 * Default  :   0x0000.0000
 * */
#define     GPTMICR_REG_OFFSET                     0x024

/* GPTM Timer A Time-out raw interrupt clear */
#define     GPTMICR_TATOCINT_BIT    0
#define     GPTMICR_TBTOCINT_BIT    8

/* GPTM Timer A Interval Load
 * Default  :   0xFFFF.FFFF
 * */
#define     GPTMTAILR_REG_OFFSET                   0x028

/* GPTM Timer B Interval Load
 * Default  :   -
 * */
#define     GPTMTBILR_REG_OFFSET                   0x02C


/* GPTM Timer A Match
 * Default  :   0xFFFF.FFFF
 * */
#define     GPTMTAMATCHR_REG_OFFSET                0x030

/* GPTM Timer B Match
 * Default  :   -
 * */
#define     GPTMTBMATCHR_REG_OFFSET                0x034


/* GPTM Timer A Prescale
 * Default  :   0x0000.0000
 * */
#define     GPTMTAPR_REG_OFFSET                   	0x038

/* GPTM Timer B Prescale
 * Default  :   0x0000.0000
 * */
#define     GPTMTBPR_REG_OFFSET                   	0x03C


/* GPTM Timer A Prescale Match
 * Default  :   0x0000.0000
 * */
#define     GPTMTAPMR_REG_OFFSET                   0x040

/* GPTM Timer B Prescale Match
 * Default  :   0x0000.0000
 * */
#define     GPTMTBPMR_REG_OFFSET                   0x044


/* GPTM Timer A
 * Default  :   0xFFFF.FFFF
 * READONLY
 * */
#define     GPTMTAR_REG_OFFSET                   	0x048

/* GPTM Timer B
 * Default  :   0xFFFF.FFFF
 * READONLY
 * */
#define     GPTMTBR_REG_OFFSET                   	0x04C


/* GPTM Timer A Value
 * Default  :   0xFFFF.FFFF
 * */
#define     GPTMTAV_REG_OFFSET                   	0x050

/* GPTM Timer B
 * Default  :   -
 * */
#define     GPTMTBV_REG_OFFSET                   	0x054

/* GPTM RTC Predivide
 * Default  :   0x0000.7FFF
 * */
#define     GPTMRTCPD_REG_OFFSET               		0x058

/* GPTM Timer A Prescale Snapshot
 * Default  :   0x0000.0000
 * READONLY
 * */
#define     GPTMTAPS_REG_OFFSET                		0x05C

/* GPTM Timer B Prescale Snapshot
 * Default  :   0x0000.0000
 * READONLY
 * */
#define     GPTMTBPS_REG_OFFSET                		0x060

/* GPTM Timer A Prescale Value
 * Default  :   0x0000.0000
 * READONLY
 * */
#define     GPTMTAPV_REG_OFFSET                		0x064

/* GPTM Timer B Prescale Snapshot
 * Default  :   0x0000.0000
 * READONLY
 * */
#define     GPTMTBPV_REG_OFFSET                		0x068

/* GPTM Peripheral Properties
 * Default  :   0x0000.0000
 * READONLY
 * */
#define     GPTMPP_REG_OFFSET                  		0xFC0

#endif //GPT_PRIVATE_H
