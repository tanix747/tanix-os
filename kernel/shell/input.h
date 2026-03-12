#ifndef INPUT_H
#define INPUT_H

#define INPUT_SIZE 256

void input_add_char(char c);
int input_ready();
void input_read(char* dest);

#endif
