/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the CC3220S_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC3220S_LAUNCHXL

#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== GPIO ========
 */

/* P64 */
#define GPIO_RS                     0
/* P50 */
#define GPIO_E                      1
/* P18 */
#define GPIO_D4                     2
/* P08 */
#define GPIO_D5                     3
/* P07 */
#define GPIO_D6                     4
/* P06 */
#define GPIO_D7                     5

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== I2C ========
 */

/*
 *  SCL: P01
 *  SDA: P02
 *  LaunchPad I2C
 */
#define I2C                         0

/* ======== I2C Addresses and Speeds ======== */
#include <ti/drivers/I2C.h>

/* ---- I2C I2C bus components ---- */

/* BMA222E address and max speed */
#define I2C_BMA222E_ADDR     (0x18)
#define I2C_BMA222E_MAXSPEED (400U) /* Kbps */

/* TMP006 address and max speed */
#define I2C_TMP006_ADDR     (0x41)
#define I2C_TMP006_MAXSPEED (3400U) /* Kbps */

/* I2C max speed (specified by the I2C.maxBitRate config setting) */
#define I2C_MAXSPEED   (10U) /* Kbps */
#define I2C_MAXBITRATE ((I2C_BitRate)I2C_100kHz)


/*
 *  ======== UART ========
 */

/*
 *  TX: P03
 *  RX: P04
 */
#define UART                        0
/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
#define UART_LOCAL                  1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
