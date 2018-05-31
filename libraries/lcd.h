/*
 * lcd.h
 *
 *  Created on: 24 May 2018
 *      Author: rafae
 */

#ifndef LIBRARIES_LCD_H_
#define LIBRARIES_LCD_H_


/*...........................................................................
  Name:     lcd_init
  Purpose:  set configuration of ports and pins
  Entry:    no parameters
  Exit:     no parameters
  Notes:
*/
void init_lcd();


/*...........................................................................
  Name:     lcd_write_4
  Purpose:  send a nibble (4-bits) of information to the LCD module
  Entry:    (theByte) contains a byte of data with the desired 4-bits in the upper nibble
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/
void lcd_write_4(uint8_t);


/*...........................................................................
  Name:     lcd_write_instruction_4f
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    configures RW (busy flag is implemented)
*/
void lcd_write_instruction_4f(uint8_t);


/*...........................................................................
  Name:     lcd_write_character_4f
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    configures RW (busy flag is implemented)
*/
void lcd_write_character_4f(uint8_t);


/*...........................................................................
  Name:     lcd_write_string_4f
  Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses the busy flag instead of time delays
*/
void lcd_write_string_4f(uint8_t *);


/*
  Name:     lcd_init_4f
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses the busy flag instead of time delays when possible
*/
void lcd_init_4f();


/*...........................................................................
  Name:     lcd_check_BF_4
  Purpose:  check busy flag, wait until LCD is ready
  Entry:    no parameters
  Exit:     no parameters
  Notes:    main program will hang if LCD module is defective or missing
            data is read while 'E' is high
            both nibbles must be read even though desired information is only in the high nibble
*/
void lcd_check_BF_4();


void print_lcd(uint8_t* str, uint8_t line, uint8_t offset);

void lcd_function();

void clear_lcd();

#endif /* LIBRARIES_LCD_H_ */
