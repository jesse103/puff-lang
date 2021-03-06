#ifndef PUFF_IO_H
#define PUFF_IO_H

char* sh(const char* cmd);
char* puff_read_file(const char* filename);
void puff_write_file(const char* filename, char* outbuffer);

#endif