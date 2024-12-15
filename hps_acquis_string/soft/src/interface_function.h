/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : function.h
 * Author               : Anthony Convers
 * Date                 : 27.07.2022
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Header file for base interface
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 0.0    27.07.2022  ACS           Initial version.
 * 1.0    01.12.2024  CJS          Implement lab5.
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "axi_lw.h"

#define INTERFACE_OFFSET   0x10000
#define INTERFACE_ADDR	   (AXI_LW_HPS_FPGA_BASE_ADD + INTERFACE_OFFSET)

#define INTERFACE_REG(_x_) *(volatile uint32_t *)(INTERFACE_ADDR + _x_)

// Constant ID
#define CONST_ID	   AXI_LW_REG(0x0)
#define INTERFACE_ID	   INTERFACE_REG(0x0)
#define BUTTON_REG	   INTERFACE_REG(0x4)
#define SWITCH_REG	   INTERFACE_REG(0x8)
#define LED_REG		   INTERFACE_REG(0xC)
#define LED_MASK	   0x3FF

#define NUM_KEYS	   4
#define KEY_0		   0
#define KEY_1		   1
#define KEY_2		   2
#define KEY_3		   3

//***********************************//
//****** Global usage function ******//

// Switchs_read function : Read the switchs value
// Parameter : None
// Return : Value of all Switchs (SW9 to SW0)
uint32_t Switchs_read(void);

// Leds_write function : Write a value to all Leds (LED9 to LED0)
// Parameter : "value"= data to be applied to all Leds
// Return : None
void Leds_write(uint32_t value);

// Leds_set function : Set to ON some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a set (maximum 0x3FF)
// Return : None
void Leds_set(uint32_t maskleds);

// Leds_clear function : Clear to OFF some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a clear (maximum 0x3FF)
// Return : None
void Leds_clear(uint32_t maskleds);

// Leds_toggle function : Toggle the curent value of some or all Leds (LED9 to LED0)
// Parameter : "maskleds"= Leds selected to apply a toggle (maximum 0x3FF)
// Return : None
void Leds_toggle(uint32_t maskleds);

// Key_read function : Read one Key status, pressed or not (KEY0 or KEY1 or KEY2 or KEY3)
// Parameter : "key_number"= select the key number to read, from 0 to 3
// Return : True(1) if key is pressed, and False(0) if key is not pressed
bool Key_read(int key_number);
