#include "Arduino.h"
#include "pinmux.inc"

const uint16_t P400_b[] = {
PIN_PWM|CHANNEL_6|PWM_CHANNEL_A|GPT_ODD_CFG,
PIN_SCL|CHANNEL_0,
PIN_INTERRUPT|CHANNEL_0,
SCI_CHANNEL|PIN_SCK|CHANNEL_0|SCI_EVEN_CFG|LAST_ITEM_GUARD
};
#define P400 P400_b

const uint16_t P408_b[] = {
PIN_PWM|CHANNEL_5|PWM_CHANNEL_B|GPT_ODD_CFG,
PIN_INTERRUPT|CHANNEL_7,
SCI_CHANNEL|PIN_CTS_RTS_SS|CHANNEL_1|SCI_EVEN_CFG,
SCI_CHANNEL|PIN_RX_MISO_SCL|CHANNEL_9|SCI_ODD_CFG|LAST_ITEM_GUARD
};
#define P408 P408_b

const uint16_t P014_b[] = {
PIN_DAC|CHANNEL_0,
PIN_PWM|CHANNEL_9|PWM_CHANNEL_A|GPT_ODD_CFG,
PIN_ANALOG|CHANNEL_9|LAST_ITEM_GUARD
};
#define P014 P014_b

extern "C" const PinMuxCfg_t g_pin_cfg[] = { 
  { BSP_IO_PORT_00_PIN_14,    P014   }, /* (0) D0/A0  */
  { BSP_IO_PORT_00_PIN_00,    P000   }, /* (1) D1/A1  */
  { BSP_IO_PORT_00_PIN_01,    P001   }, /* (2) D2/A2  */
  { BSP_IO_PORT_00_PIN_02,    P002   }, /* (3) D3/A3  */
  { BSP_IO_PORT_02_PIN_06,    P206   }, /* (4) D4  */
  { BSP_IO_PORT_01_PIN_00,    P100   }, /* (5) D5/A5  */
  { BSP_IO_PORT_03_PIN_02,    P302   }, /* (6) D6 */
  { BSP_IO_PORT_03_PIN_01,    P301   }, /* (7) D7 */
  { BSP_IO_PORT_01_PIN_11,    P111   }, /* (8) D8 */
  { BSP_IO_PORT_01_PIN_10,    P110   }, /* (9) D9 */
  { BSP_IO_PORT_01_PIN_09,    P109   }, /* (10) D10 */

  { BSP_IO_PORT_04_PIN_08,    P408   }, /* (11) D11 */
  { BSP_IO_PORT_04_PIN_09,    P409   }, /* (12) D12 */
  { BSP_IO_PORT_00_PIN_13,    P013   }, /* (13) D13/SCL */
  { BSP_IO_PORT_00_PIN_12,    P012   }, /* (14) D14/SDA */
  { BSP_IO_PORT_01_PIN_01,    P101   }, /* (15) D15 */
  { BSP_IO_PORT_01_PIN_04,    P104   }, /* (16) D16 */
  { BSP_IO_PORT_01_PIN_02,    P102   }, /* (17) D17 */
  { BSP_IO_PORT_01_PIN_03,    P103   }, /* (18) D18 */

  { BSP_IO_PORT_00_PIN_11,    P011   }, /* (19) User LED pin */
  { BSP_IO_PORT_01_PIN_12,    P112   }, /* (20) RGB pin */
  { BSP_IO_PORT_05_PIN_00,    P500   }, /* (21) RGB EN pin  */

  { BSP_IO_PORT_00_PIN_15,    P015   }, /* (22) Battery voltage detection pin */
  { BSP_IO_PORT_04_PIN_00,    P400   }, /* (23) BAT Read EN pin */

  { BSP_IO_PORT_01_PIN_08,    P108   }, /* (24) SWDIO  */
  { BSP_IO_PORT_03_PIN_00,    P300   }, /* (25) SWCLK  */

  { BSP_IO_PORT_02_PIN_01,    P201   }, /* (26) BOOT */
};

extern "C" {
    unsigned int PINCOUNT_fn() {
        return (sizeof(g_pin_cfg) / sizeof(g_pin_cfg[0]));
    }
}

int32_t getPinIndex(bsp_io_port_pin_t p) {
  int max_index = PINS_COUNT;
  int rv = -1;
  for(int i = 0; i < max_index; i++) {
    if(g_pin_cfg[i].pin == p) {
      rv = i;
      break;
    }
  }
  return rv;
}

#include "FspTimer.h"

void usb_post_initialization() {
  ((R_USB_FS0_Type*)R_USB_FS0_BASE)->USBMC_b.VDCEN = 1;
}

void enableSubclockInputPins() {
  R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_CGC);
  R_SYSTEM->SOSCCR_b.SOSTP = 1;
  R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_CGC);
}

void initVariant() {
  // bootloader configures LED_BUILTIN as PWM output, deconfigure it to avoid spurious signals
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(3, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(5, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(6, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(9, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(10, PIN_CFG_REQ_PWM)[0]));
  FspTimer::set_initial_timer_channel_as_pwm(GPT_TIMER, GET_CHANNEL(getPinCfgs(11, PIN_CFG_REQ_PWM)[0]));
}