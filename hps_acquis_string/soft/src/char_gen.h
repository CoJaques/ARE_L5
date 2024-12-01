/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : char_gen.h
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

#ifndef CHAR_GEN_H
#define CHAR_GEN_H

#include <stdint.h>
#include <stdbool.h>

// Base address for char generator
#define CHAR_GEN_BASE_ADDR	 0x010000

// Offsets for specific functionalities
#define CHAR_GEN_INIT_OFFSET	 0x10
#define CHAR_GEN_MODE_OFFSET	 0x14
#define CHAR_GEN_NEW_CHAR_OFFSET 0x10
#define CHAR_GEN_DELAY_OFFSET	 0x14
#define CHAR_GEN_STATUS_OFFSET	 0x10
#define CHAR_GEN_CHAR_GROUP_1	 0x20
#define CHAR_GEN_CHAR_GROUP_2	 0x24
#define CHAR_GEN_CHAR_GROUP_3	 0x28
#define CHAR_GEN_CHAR_GROUP_4	 0x2C
#define CHAR_GEN_CHECKSUM_OFFSET 0x30

// Masks for specific bits
#define INIT_MASK		 0x01
#define NEW_CHAR_MASK		 0x10
#define MODE_MASK		 0x10
#define DELAY_MASK		 0x03

void generator_init(void);
void generator_change_mode(int mode);
int generator_get_mode(void);
void generator_change_speed(int speed);
int generator_get_speed(void);
void generator_generate(void);
char read_char(uint8_t char_n);
uint32_t get_4_char(uint8_t chargroup_n);

#endif // CHAR_GEN_H
