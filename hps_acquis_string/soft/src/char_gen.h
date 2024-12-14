/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingénierie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : char_gen.h
 * Author               : Colin Jaques & Alexandre Iorio
 * Date                 : 01.12.2024
 *
 * Context              : ARE lab
 *
 *****************************************************************************************
 * Brief: Header file for char generator interface
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 * 1.0    01.12.2024  CJS          Initial version
 *
 *****************************************************************************************/

#ifndef CHAR_GEN_H
#define CHAR_GEN_H

#include <stdint.h>
#include <stdbool.h>
#include "interface_function.h"

// Pseudo-macro to simplify register access
#define CHAR_GEN_REG(_x_)    (*(volatile uint32_t *)(CHAR_GEN_BASE_ADDR + (_x_)))

// Register offsets for char generator functionalities
#define CHAR_GEN_INIT_OFFSET 0x10 // Command to initialize the generator
#define CHAR_GEN_MODE_OFFSET 0x14 // Command to set generation mode
#define CHAR_GEN_NEW_CHAR_OFFSET \
	0x10 // Command to generate a new char (manual mode)
#define CHAR_GEN_DELAY_OFFSET \
	0x14 // Command to set generation delay (auto mode)
#define CHAR_GEN_STATUS_OFFSET 0x10 // Read status of the generator
#define CHAR_GEN_CHAR_GROUP_1  0x20 // Read characters 1-4
#define CHAR_GEN_CHAR_GROUP_2  0x24 // Read characters 5-8
#define CHAR_GEN_CHAR_GROUP_3  0x28 // Read characters 9-12
#define CHAR_GEN_CHAR_GROUP_4  0x2C // Read characters 13-16
#define CHAR_GEN_CHECKSUM_OFFSET \
	0x30 // Read checksum of the 16-character string
//
//Register used to lock the update of string from the fpga
#define CHAR_GEN_LOCK_READ_OFFSET 0x18

//Register used to define if safe mode is enabled
#define CHAR_GEN_SAFE_MODE	  0x1C

// Masks for specific bit operations
#define INIT_MASK		  0x01 // Mask for initialization command
#define NEW_CHAR_MASK		  0x10 // Mask for generating new characters
#define MODE_MASK		  0x10 // Mask for generation mode (manual/auto)
#define DELAY_MASK		  0x03 // Mask for generation delay (2 bits)

//***********************************//
//******** Function Prototypes ******//

/**
 * @brief Initializes the char generator to its default state.
 *        This resets the 16 characters and checksum to their initial values.
 * @param None
 * @return None
 */
void generator_init(void);

/**
 * @brief Changes the generation mode of the char generator.
 * @param mode The desired mode:
 *             - 0: Manual mode
 *             - 1: Automatic mode
 * @return None
 */
void generator_change_mode(int mode);

/**
 * @brief Gets the current generation mode of the char generator.
 * @param None
 * @return int The current mode:
 *             - 0: Manual mode
 *             - 1: Automatic mode
 */
int generator_get_mode(void);

/**
 * @brief Changes the generation speed of the char generator (automatic mode only).
 * @param speed The desired speed (2 bits):
 *              - 0: 1 Hz
 *              - 1: 1 KHz
 *              - 2: 100 KHz
 *              - 3: 1 MHz
 * @return None
 */
void generator_change_speed(int speed);

/**
 * @brief Gets the current generation speed of the char generator.
 * @param None
 * @return int The current speed:
 *             - 0: 1 Hz
 *             - 1: 1 KHz
 *             - 2: 100 KHz
 *             - 3: 1 MHz
 */
int generator_get_speed(void);

/**
 * @brief Generates a new 16-character string (manual mode only).
 * @param None
 * @return None
 */
void generator_generate(void);

/**
 * @brief Reads a specific character from the 16-character string.
 * @param char_n The index of the character to read (0-15).
 * @return char The ASCII value of the character.
 */
char read_char(uint8_t char_n);

/**
 * @brief Reads a group of 4 characters from the 16-character string.
 * @param chargroup_n The group index to read (0-3).
 * @return uint32_t The 4 characters packed into a 32-bit value.
 */
uint32_t get_4_char(uint8_t chargroup_n);

/**
 * @brief Reads the checksum of the 16-character string.
 * @param None
 * @return uint8_t The checksum value.
 */
uint8_t get_checksum(void);

/**
 * @brief Reads characters in bulk (4 at a time), calculates the integrity, 
 *        and displays the result.
 * @param mode The generation mode to use (0: unsafe, 1: safe).
 */
void calculate_integrity_bulk(uint8_t mode);

#endif // CHAR_GEN_H
