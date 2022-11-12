#include "asm/encoder.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

static bool dump_header(int fd, struct encoder *enc)
{
	enc->result->hdr.prog_size = bswap_32(enc->result->hdr.prog_size);
	if (!write(fd, &enc->result->hdr, sizeof enc->result->hdr))
	{
		dprintf(2, "Error: could not write header to the file: %s\n", strerror(errno));
		return false;
	}
	return true;
}

static bool dump_assembly(int fd, struct encoder *enc, size_t prog_size)
{
	if (!write(fd, &enc->result->assembly, prog_size * sizeof(uint8_t)))
	{
		dprintf(2, "Error: could not write assembly to the file: %s\n", strerror(errno));
		return false;
	}
	return true;
}

bool dump_to_file(struct encoder *enc)
{
	int fd = open(enc->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		dprintf(2, "Error: could not close file: %s\n", strerror(errno));
		return false;
	}

	size_t size = enc->result->hdr.prog_size;
	if (!(dump_header(fd, enc) && dump_assembly(fd, enc, size)))
		return false;

	if (close(fd) == -1)
	{
		dprintf(2, "Error: could not close file: %s\n", strerror(errno));
		return false;
	}
	return true;
}