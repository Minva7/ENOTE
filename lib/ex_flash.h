#ifndef __EX_FLASH_H
#define __EX_FLASH_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
extern void user_read_eeprom(unsigned int addr,char *str, unsigned int len);
extern void user_write_eeprom(unsigned int addr, char *str  ,unsigned int len);
extern void get_str_to_gui(char *str , unsigned int addr);
extern void clear_all(void);
#ifdef __cplusplus
} /* extern "C" */

#endif
#endif
