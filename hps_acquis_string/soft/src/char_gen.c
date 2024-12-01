/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : char_gen.c
 * Author               : Colin Jaques & Alexandre Iorio
 * Date                 : 
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: char generator interface
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    1.12.2024  CJS          INITIAL VERSION
 *
*****************************************************************************************/

#include "char_gen.h"

// Pseudo-macro to simplify register access
#define CHAR_GEN_REG(_x_) (*(volatile uint32_t *)(CHAR_GEN_BASE_ADDR + (_x_)))

void generator_init(void)
{
	CHAR_GEN_REG(CHAR_GEN_INIT_OFFSET) |= INIT_MASK; // Set the init bit
	CHAR_GEN_REG(CHAR_GEN_INIT_OFFSET) &= ~INIT_MASK; // Clear the init bit
}

void generator_change_mode(int mode)
{
	if (mode) {
		CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) |=
			MODE_MASK; // Set to automatic mode
	} else {
		CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) &=
			~MODE_MASK; // Set to manual mode
	}
}

int generator_get_mode(void)
{
	return (CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) & MODE_MASK) ? 1 : 0;
}

void generator_change_speed(int speed)
{
	CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) &= ~DELAY_MASK; // Clear delay bits
	CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) |=
		(speed & DELAY_MASK); // Set new speed
}

int generator_get_speed(void)
{
	return (CHAR_GEN_REG(CHAR_GEN_MODE_OFFSET) & DELAY_MASK);
}

void generator_generate(void)
{
	CHAR_GEN_REG(CHAR_GEN_NEW_CHAR_OFFSET) |=
		NEW_CHAR_MASK; // Trigger new char
}

char read_char(uint8_t char_n)
{
	if (char_n > 15)
		return 0; // Invalid character number

	uint32_t group_offset = CHAR_GEN_CHAR_GROUP_1 + (char_n / 4) * 4;
	uint32_t shift = (3 - (char_n % 4)) * 8;

	return (CHAR_GEN_REG(group_offset) >> shift) & 0xFF;
}

uint32_t get_4_char(uint8_t chargroup_n)
{
	if (chargroup_n > 3)
		return 0; // Invalid group number
	return CHAR_GEN_REG(CHAR_GEN_CHAR_GROUP_1 + chargroup_n * 4);
}

uint8_t get_checksum(void)
{
	return CHAR_GEN_REG(CHAR_GEN_CHECKSUM_OFFSET);
}
