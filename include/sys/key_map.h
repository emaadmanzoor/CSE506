/* The following array is taken from 
    http://www.osdever.net/bkerndev/Docs/keyboard.htm
*/

#define SHIFT1 0x2A
#define SHIFT2 0x36
#define CTRL 0x1D

unsigned char key_map[128] =
  {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',/* 9 */
    '9', '0', '-', '=', '\b',/* Backspace */
    '\t',/* Tab */
    'q', 'w', 'e', 'r',/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',/* Enter key */
    0,/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',/* 39 */
    '\'', '`',   0,/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',/* 49 */
    'm', ',', '.', '/',   0,/* Right shift */
    '*',
    0,/* Alt */
    ' ',/* Space bar */
    0,/* Caps lock */
    0,/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,/* < ... F10 */
    0,/* 69 - Num lock*/
    0,/* Scroll Lock */
    0,/* Home key */
    0,/* Up Arrow */
    0,/* Page Up */
    '-',
    0,/* Left Arrow */
    0,
    0,/* Right Arrow */
    '+',
    0,/* 79 - End key*/
    0,/* Down Arrow */
    0,/* Page Down */
    0,/* Insert Key */
    0,/* Delete Key */
    0,   0,   0,
    0,/* F11 Key */
    0,/* F12 Key */
    0,/* All other keys are undefined */
  };


/*
 * Taken the key map from xv6/kbd.h
 */
static unsigned char shift_key_map[256] =
  {
     0,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
    '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
    'O',  'P',  '{',  '}',  '\n', 0,   'A',  'S',
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
    '"',  '~',  0,   '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M', '<',  '>',  '?',  0,   '*',  // 0x30
     0,   ' ',  0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  0,   0,   0,   0};
