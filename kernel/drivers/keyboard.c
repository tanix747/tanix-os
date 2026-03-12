#include <stdint.h>
#include "../cpu/ports/ports.h"
#include "screen.h"
#include "../shell/input.h"
#define KB_BUFFER_SIZE 128

static char kb_buffer[KB_BUFFER_SIZE];
static int kb_head = 0;
static int kb_tail = 0;

char scancode_table[] =
{
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/',
    0, '*', 0, ' '
};
char scancode_shift_table[] =
{
    0, 27, '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y',
    'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?',
    0, '*', 0, ' '
};

int caps_lock=0;
int shift_pressed=0;

void keyboard_callback()
{
    uint8_t scancode = inb(0x60);

    // SHIFT press
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        return;
    }

    // SHIFT release
    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        return;
    }

    // Ignore other releases
    if (scancode & 0x80)
        return;

    // Caps Lock toggle
    if (scancode == 0x3A)
    {
        caps_lock = !caps_lock;
        return;
    }

    // Backspace
    if (scancode == 0x0E)
    {
        print_char('\b');
        input_add_char('\b');
        return;
    }

    char c;

    if (shift_pressed)
        c = scancode_shift_table[scancode];
    else
        c = scancode_table[scancode];

    if (!c)
        return;

    // Caps logic
    if (!shift_pressed && caps_lock && c >= 'a' && c <= 'z')
        c = c - 32;

    print_char(c);
    input_add_char(c); // also store in shell buffer
}

char keyboard_getchar() {

   // if (kb_tail == kb_head)
        return 0;   // buffer empty

  //  char c = kb_buffer[kb_tail];
 //   kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;

 //   return c;
}


void keyboard_install()
{
    irq_install_handler(1, keyboard_callback);
}
