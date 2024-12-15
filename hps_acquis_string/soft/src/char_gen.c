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
#include <stdint.h>
#include <stdio.h>

void generator_init(void)
{
	INTERFACE_REG(CHAR_GEN_INIT_OFFSET) = INIT_MASK; // Set the init bit
	INTERFACE_REG(CHAR_GEN_LOCK_READ_OFFSET) = 0;
}

void generator_change_mode(int mode)
{
	if (mode) {
		INTERFACE_REG(CHAR_GEN_MODE_OFFSET) |=
			MODE_MASK; // Set to automatic mode
	} else {
		INTERFACE_REG(CHAR_GEN_MODE_OFFSET) &=
			~MODE_MASK; // Set to manual mode
	}
}

int generator_get_mode(void)
{
	return (INTERFACE_REG(CHAR_GEN_MODE_OFFSET) & MODE_MASK) ? 1 : 0;
}

void generator_change_speed(int speed)
{
	INTERFACE_REG(CHAR_GEN_MODE_OFFSET) &= ~DELAY_MASK; // Clear delay bits
	INTERFACE_REG(CHAR_GEN_MODE_OFFSET) |=
		(speed & DELAY_MASK); // Set new speed
}

int generator_get_speed(void)
{
	return (INTERFACE_REG(CHAR_GEN_MODE_OFFSET) & DELAY_MASK);
}

void generator_generate(void)
{
	INTERFACE_REG(CHAR_GEN_NEW_CHAR_OFFSET) |=
		NEW_CHAR_MASK; // Trigger new char
}

uint32_t get_4_char(uint8_t chargroup_n)
{
	if (chargroup_n > 3)
		return 0; // Invalid group number
	return INTERFACE_REG(CHAR_GEN_CHAR_GROUP_1 + chargroup_n * 4);
}

uint8_t get_checksum(void)
{
	return INTERFACE_REG(CHAR_GEN_CHECKSUM_OFFSET);
}

void calculate_integrity_bulk(uint8_t mode)
{
	if (mode) {
		INTERFACE_REG(CHAR_GEN_LOCK_READ_OFFSET) = 1;
	}

	uint8_t checksum = get_checksum(); // Retrieve the checksum value
	uint32_t sum = 0;
	uint32_t checksum_computed = 0;
	char str[17] = { 0 }; // Buffer to hold the full 16-character string

	// Read characters in groups of 4
	for (int i = 0; i < 4; i++) {
		uint32_t group = get_4_char(
			i); // Read 4 characters as a single 32-bit value
		for (int j = 0; j < 4; j++) {
			char c = (group >> (8 * (3 - j))) &
				 0xFF; // Extract each character
			sum += c;
			str[i * 4 + j] =
				c; // Store character in the string buffer
		}
	}

	if (mode) {
		INTERFACE_REG(CHAR_GEN_LOCK_READ_OFFSET) = 0;
	}

	// Add the checksum to the sum and apply modulo 256
	checksum_computed = (sum + checksum) % 256;

	// Check if the integrity is valid
	if (checksum_computed == 0) {
		printf("OK: checksum: 0x%02X, calculated: 0x%02X, string: %s\n",
		       checksum, sum % 256, str);
	} else {
		static uint32_t error_count = 0; // Persistent error counter
		error_count++;
		printf("ER: checksum: 0x%02X, calculated: 0x%02X, string: %s\n",
		       checksum, sum, str);
		printf("ER: error count: %d\n", error_count);
	}
}
