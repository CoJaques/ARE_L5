/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : pio_function.c
 * Author               : Colin Jaques
 * Date                 : 
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Pio function
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    13.10.2024  CJS          INITIAL VERSION
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "axi_lw.h"
#include "pio_function.h"

uint32_t Switchs_read(void)
{
	return SWITCH_REG;
}

void Leds_set(uint32_t maskleds)
{
	LED_REG |= maskleds & LED_MASK;
}

void Leds_clear(uint32_t maskleds)
{
	LED_REG &= ~(maskleds & LED_MASK);
}

void Leds_toggle(uint32_t maskleds)
{
	LED_REG ^= (maskleds)&LED_MASK;
}

bool Key_read(int key_number)
{
	return !(BUTTON_REG & (1 << key_number));
}
