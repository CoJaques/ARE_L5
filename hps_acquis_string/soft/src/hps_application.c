/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingénierie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : hps_application.c
 * Author               : Colin Jaques
 * Date                 : 12.10.2024
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Turn ON leds and 7 segment display depending on the value of button and switch, for DE1-SoC board
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    12.10.2024 CJS          Initial version.
 *
*****************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "axi_lw.h"
#include "pio_function.h"

int __auto_semihosting;

void read_keys(bool *keys_state)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state[i] = Key_read(i);
	}
}

void update_old_keys(bool *keys_state, bool *keys_state_old)
{
	for (int i = 0; i < NUM_KEYS; i++) {
		keys_state_old[i] = keys_state[i];
	}
}

int main(void)
{
	printf("Laboratoire: Programme simple avec E/S du HPS \n");
	printf("Constant ID = 0x%lx\n", (unsigned long)CONST_ID);

	Switchs_init();
	Leds_init();
	Keys_init();
	Segs7_init();

	bool keys_state[4] = { false, false, false, false };
	bool keys_state_old[4] = { false, false, false, false };

	uint32_t counter = 0;
	uint32_t rotate_sequence = 8;

	while (1) {
		const uint32_t switchs_value = Switchs_read();
		read_keys(keys_state);

		uint8_t switch_70 = switchs_value & SWITCHS_70_MASK;

		if (keys_state[KEY_0] && !keys_state_old[KEY_0]) {
			uint8_t switch_98 = (switchs_value >> 8);

			switch (switch_98) {
			case 0:
				Leds_write(switch_70);
				break;
			case 1:
				Leds_set(switch_70);
				break;
			case 2:
				Leds_clear(switch_70);
				break;
			case 3:
				Leds_toggle(LEDS_70_MASK);
				break;
			}
		}

		if (keys_state[KEY_1] && !keys_state_old[KEY_1]) {
			Seg7_write_hex(0, (switch_70 & 0x0F));
			Seg7_write_hex(1, (switch_70 >> 4));
		}

		if (keys_state[KEY_2] && !keys_state_old[KEY_2]) {
			counter = (counter + 1) % MAX_7SEG_VALUE;
			Seg7_write_hex(2, counter);
		}

		if (keys_state[KEY_3] && !keys_state_old[KEY_3]) {
			// Rotate the sequence to the left, wrapping around when the leftmost bit is set
			if ((rotate_sequence & 0b0100000) != 0) {
				// Wrap around to the least significant bit
				rotate_sequence = 0b0000001;
			} else {
				// Shift left by one position
				rotate_sequence <<= 1;
			}

			Seg7_write(3, rotate_sequence);
		}

		update_old_keys(keys_state, keys_state_old);
	}
}
