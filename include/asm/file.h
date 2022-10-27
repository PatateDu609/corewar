#ifndef ASM_FILE_H
#define ASM_FILE_H

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

struct asm_file
{
	int fd;
	const char *filename;

	off_t filesize;
	char *content;
};

struct asm_file *setup_file(const char *arg);
void free_file(struct asm_file *file);

#endif
