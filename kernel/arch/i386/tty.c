#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

    size_t y, x;
	for (y = 0; y < VGA_HEIGHT; y++) {
		for (x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(int line) {
    size_t i;
    for(i = 0; i < VGA_WIDTH; i++)
    {
        terminal_buffer[(line - 1) * VGA_WIDTH + i] = terminal_buffer[line * VGA_WIDTH + i];
    }    
}

void terminal_delete_last_line() {
    const size_t last_line = (VGA_HEIGHT - 1) * VGA_WIDTH;
    size_t i;
    for (i = 0; i < VGA_WIDTH; i++) {
        terminal_buffer[last_line + i] = vga_entry(' ', terminal_color);
    }
}

void terminal_putchar(char c) {
    size_t line;
    unsigned char uc = c;
 
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            for(line = 1; line <= VGA_HEIGHT - 1; line++)
            {
                terminal_scroll(line);
            }
            terminal_delete_last_line();
            terminal_row = VGA_HEIGHT - 1;
        }
        return;
    }

    terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            for(line = 1; line <= VGA_HEIGHT - 1; line++)
            {
                terminal_scroll(line);
            }
            terminal_delete_last_line();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write(const char* data, size_t size) {
    size_t i;
	for (i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
