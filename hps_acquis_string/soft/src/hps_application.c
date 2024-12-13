
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
#define SWITCH_SAFE_MODE (1 << 0) // Switch to enable safe mode
#define SWITCH_GEN_MODE (1 << 7) // Switch to change generation mode
#define SWITCHS_GEN_SPEED (3 << 8) // Switch to change generation speed

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
void update_previous_keys(bool *keys_state, bool *keys_state_old)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state_old[i] = keys_state[i];
	}
}

int main(void)
{
	uint32_t switches; // Current state of switches
	uint32_t previous_switches; // Previous state of switches
	bool keys_state[NUM_KEYS] = { false }; // Current state of the keys
	bool keys_state_old[NUM_KEYS] = { false }; // Previous state of the keys

	// Display the design constants
	printf("Constant AXI ID: 0x%08lX\n", (unsigned long)CONST_ID);
	printf("Constant Avalon ID: 0x%08lX\n", (unsigned long)INTERFACE_ID);

	// Initialize the LEDs to OFF
	Leds_write(0);

	// Initialize the switches
	switches = Switchs_read();
	previous_switches = ~switches;

	while (1) {
		// Read the current state of switches
		switches = Switchs_read();
		read_keys(keys_state);

		// Update the LEDs based on the switches only if their state has changed
		if (switches != previous_switches) {
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

		// Update the generator mode if SW7 state has changed
		if ((switches & SWITCH_GEN_MODE) !=
		    (previous_switches & SWITCH_GEN_MODE)) {
			generator_change_mode((switches >> 7) & 0x1);
		}

		// Update the generation speed if SW9-8 state has changed
		if ((switches & SWITCHS_GEN_SPEED) !=
		    (previous_switches & SWITCHS_GEN_SPEED)) {
			generator_change_speed((switches >> 8) & 0x3);
		}

		// While KEY2 is active: Calculate integrity continuously
		if (keys_state[KEY_2]) {
			calculate_integrity_bulk(switches & SWITCH_SAFE_MODE);
		}

		// Update the previous state of keys and switches
		update_previous_keys(keys_state, keys_state_old);
		previous_switches = switches;
	}

	return 0;
}
