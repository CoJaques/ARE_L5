
/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingénierie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : main.c
 * Author               : Colin Jaques
 * Date                 : 01.12.2024
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Main program to control the char generator interface
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    01.12.2024  CJS          Initial version
 * 1.1    02.12.2024  CJS          Optimized bulk read and switch updates
 *
 *****************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "char_gen.h"
#include "interface_function.h"

int __auto_semihosting;

#define NUM_KEYS 4 // Number of keys on the board

/**
 * @brief Reads the current state of all keys.
 * @param keys_state Array to store the state of each key (pressed or not).
 */
void read_keys(bool *keys_state)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state[i] = Key_read(i);
	}
}

/**
 * @brief Updates the old state of all keys.
 * @param keys_state Array containing the current state of the keys.
 * @param keys_state_old Array to store the old state of the keys.
 */
void update_old_keys(bool *keys_state, bool *keys_state_old)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state_old[i] = keys_state[i];
	}
}

/**
 * @brief Reads characters in bulk (4 at a time), calculates the integrity, 
 *        and displays the result.
 */
void calculate_integrity_bulk()
{
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

int main(void)
{
	printf("Laboratoire: Conception d'une interface fiable\n");

	// Display the design constants
	printf("Constant AXI ID: 0x%08lX\n", (unsigned long)CONST_ID);
	printf("Constant Avalon ID: 0x%08lX\n",
	       (unsigned long)INTERFACE_ID);

	// Initialize the LEDs to OFF
	Leds_write(0);

	bool keys_state[NUM_KEYS] = { false }; // Current state of the keys
	bool keys_state_old[NUM_KEYS] = { false }; // Previous state of the keys
	uint32_t old_switches =
		0xFFFFFFFF; // Previous state of the switches (forced to update at startup)

	while (1) {
		// Read the current state of switches
		uint32_t switches = Switchs_read();
		read_keys(keys_state);

		// Update the LEDs based on the switches only if their state has changed
		if (switches != old_switches) {
			Leds_write(switches & LED_MASK);
		}

		// Detect rising edge on KEY0: Initialize characters
		if (keys_state[KEY_0] && !keys_state_old[KEY_0]) {
			printf("Initializing characters...\n");
			generator_init(); // Reset the character generator
		}

		// Detect rising edge on KEY1: Generate a new string (manual mode only)
		if (keys_state[KEY_1] && !keys_state_old[KEY_1]) {
			if (generator_get_mode() ==
			    0) { // Check if in manual mode
				printf("Generating new character string...\n");
				generator_generate(); // Generate a new string
			}
		}

		// While KEY2 is active: Calculate integrity continuously
		if (keys_state[KEY_2]) {
			calculate_integrity_bulk();
		}

		// Update the generator mode if SW7 state has changed
		if ((switches & (1 << 7)) != (old_switches & (1 << 7))) {
			generator_change_mode((switches >> 7) & 0x1);
		}

		// Update the generation speed if SW9-8 state has changed
		if ((switches & (3 << 8)) != (old_switches & (3 << 8))) {
			generator_change_speed((switches >> 8) & 0x3);
		}

		// Update the previous state of keys and switches
		update_old_keys(keys_state, keys_state_old);
		old_switches = switches;
	}

	return 0;
}
