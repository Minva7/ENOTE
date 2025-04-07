#include <Arduino.h>
#include <EEPROM.h>
#include "../lib/ex_flash.h"

void user_write_eeprom(unsigned int addr, char *str  ,unsigned int len)
{
  unsigned int addr_temp = addr;

  for(unsigned int i = 0; i < len; i++)
  {
    EEPROM.write(addr_temp, *str);
    delay(1);  
    str++;
    addr_temp++;

  }

  EEPROM.commit();delay(1);
}

void user_read_eeprom(unsigned int addr,char *str, unsigned int len)
{
  unsigned int addr_temp = addr;
  unsigned int i = 0;

  for(i = 0; i < len; i++)
  {
    *str = EEPROM.read(addr_temp++);
    str++;
  }

}


void get_str_to_gui(char *str , unsigned int addr)
{
  unsigned int len = 49;

  user_read_eeprom(addr, str, len);
  Serial.println( str );
  if(*str == '*') 
  {
    unsigned char i = 0;
    while( ((*str) != '\r') && (i < len))
    {
       *str = *(str + 1);
       str++;
       i++;
       printf("%c", *(str));
    } 
    str--;
    *str = '\0';
  }
}

void clear_all(void)
{
  unsigned int i = 0;
  for(i = 0; i < 4096;i++) 
  {
    EEPROM.write(i,0xff);
    delay(1);
  }  
  printf("clear all success !\r\n");
}