#define _GNU_SOURCE

#include <asm/file.h>
#include <libft.h>
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static bool check_ext(const char *arg)
{
	char *res = ft_strrchr(arg, '.');

	if (!res)
		return false;
	if (ft_strlen(res) != 2)
		return false;
	return ft_strcmp(res, ".s") == 0;
}

static void fill_info(struct asm_file *info)
{
	info->filesize = lseek(info->fd, 0, SEEK_END);
	if (info->filesize == -1)
	{
		dprintf(2, "Error: %s: %s.\n", info->filename, strerror(errno));
		close(info->fd);
		free(info);
		exit(EXIT_FAILURE);
	}

	info->content = ft_calloc(info->filesize + 1, sizeof *info->content);
	if (!info->content)
	{
		perror("Error");
		close(info->fd);
		free(info);
		exit(EXIT_FAILURE);
	}

	lseek(info->fd, 0, SEEK_SET);

	ssize_t res = read(info->fd, info->content, info->filesize);
	if (res == -1)
	{
		perror("Error");
		close(info->fd);
		free(info->content);
		free(info);
		exit(EXIT_FAILURE);
	}
	lseek(info->fd, 0, SEEK_SET);
}

static bool is_valid(int fd)
{
	char tmp;
	ssize_t res = read(fd, &tmp, sizeof tmp);

	if (res == -1)
		return false;
	lseek(fd, 0, SEEK_SET);
	return true;
}

struct asm_file *setup_file(const char *arg)
{
	if (!check_ext(arg))
	{
		dprintf(2, "Error: %s: Wrong file extension, was expecting assembler file (.s).\n", arg);
		exit(EXIT_FAILURE);
	}

	int fd = open(arg, O_RDONLY);
	if (fd == -1 || !is_valid(fd))
	{
		dprintf(2, "Error: %s: %s.\n", arg, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct asm_file *info = malloc(sizeof *info);
	if (!info)
	{
		perror("Error");
		close(fd);
		exit(EXIT_FAILURE);
	}
	info->fd = fd;
	info->filename = arg;

	fill_info(info);
	return info;
}

void free_file(struct asm_file *file)
{
	close(file->fd);
	free(file->content);
	ft_memset(file, 0, sizeof *file);
	free(file);
}