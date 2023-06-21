#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

#define GPIO_APB

#ifdef GPIO_APB
#define GPIO_OFFSET(X)			(X<4?(0x40004000 + (X*0x1000)):(0x40024000 + ((X-4)*0x1000)))
#elif  GPIO_AHB
#define GPIO_OFFSET(X)			(0x40058000 + (X*0x1000))
#else
#error "Please define a valid bus"
#endif

#define RCGCGPIO					*((volatile uint32_t_*) 0x400FE608) /* GPIO Run Mode Clock Gating Control */

#define GPIODATA(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x3FC))		/* GPIO Data */
#define GPIODIR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x400))		/* GPIO Direction */
#define GPIOIS(X)					*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x404))		/* GPIO Interrupt Sense */
#define GPIOIBE(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x408))		/* GPIO Interrupt Both Edges */
#define GPIOIEV(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x40C))		/* GPIO Interrupt Event */
#define GPIOIM(X)					*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x410))		/* GPIO Interrupt Mask */
#define GPIORIS(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x414))		/* GPIO Raw Interrupt Status */
#define GPIOMIS(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x418))		/* GPIO Masked Interrupt Status */
#define GPIOICR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x41C))		/* GPIO Interrupt Clear */
#define GPIOAFSEL(X)			*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x420))		/* GPIO Alternate Function Select */
#define GPIODR2R(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x500))		/* GPIO 2-mA Drive Select */
#define GPIODR4R(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x504))		/* GPIO 4-mA Drive Select */
#define GPIODR8R(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x508))		/* GPIO 8-mA Drive Select */
#define GPIOODR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x50C))		/* GPIO Open Drain Select */
#define GPIOPUR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x510))		/*  */
#define GPIOPDR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x514))		/*  */
#define GPIOSLR(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x518))		/* GPIO Slew Rate Control Select */
#define GPIODEN(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x51C))		/* GPIO Digital Enable */
#define GPIOLOCK(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x520))		/* GPIO Lock */
#define GPIOCR(X)					*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x524))		/* GPIO Commit */
#define GPIOAMSEL(X)			*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x528))		/* GPIO Analog Mode Select */
#define GPIOPCTL(X)				*((volatile uint32_t_*)(GPIO_OFFSET(X)+0x52C))		/* GPIO Port Control */


#define GPIO_INT_SENSE_MASK		0
#define GPIO_INT_LEVEL_MASK		1

/** 
 ** @breif Function to validate the values of the given port and pin
 *
 ** @Parameters
 *				[in]  en_a_port  	 : The port value to validate
 *				[in]  en_a_pin   	 : The pin value to validate
 *
 ** @return	GPIO_OK          : If the port and pin have valid values
 *					GPIO_INVALID_PORT: If the passed port is not a valid port
 *					GPIO_INVALID_PIN : If the passed pin is not a valid pin
 */
static en_gpio_error_t port_pin_check(en_gpio_port_t port, en_gpio_pin_t pin);

#endif
