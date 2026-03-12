#include <stdint.h>
#include "screen.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* video_memory = (uint16_t*) VGA_ADDRESS;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint8_t color = 0x0F; // white on black

void print_char(char c)
{
if (c == '\b')//backspace
{
    if (cursor_x > 0)
    {
        cursor_x--;
        uint16_t position = cursor_y * VGA_WIDTH + cursor_x;
        video_memory[position] = (color << 8) | ' ';
    }
    return;
}

    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    uint16_t position = cursor_y * VGA_WIDTH + cursor_x;
    video_memory[position] = (color << 8) | c;

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }
}

void print(const char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        print_char(str[i]);
    }
}

void print_int(int value)
{
    if (value == 0)
    {
        print_char('0');
        return;
    }

    if (value < 0)
    {
        print_char('-');
        value = -value;
    }

    char buffer[12];
    int i = 0;

    while (value > 0)
    {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0)
        print_char(buffer[--i]);
}

void clear_screen()
{
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            uint16_t pos = y * VGA_WIDTH + x;
            video_memory[pos] = (color << 8) | ' ';
        }
    }

    cursor_x = 0;
    cursor_y = 0;
}
