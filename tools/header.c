/*
 *      header.c
 *      
 *      Dustin Dorroh <ddorroh@aplopteng.com>
 *      
 *      
 */


#include <stdio.h>

char *read_file(const char *filename, size_t * length)
{
	int fd;
	struct stat file_info;
	char *buffer;
	fd = open(filename, O_RDONLY);
	fstat(fd, &file_info);
	*length = file_info.st_size;
	if (!S_ISREG(file_info.st_mode)) {
		close(fd);
		return NULL;
	}
	buffer = (char *)malloc(*length);
	read(fd, buffer, *length);
	close(fd);
	return buffer;
}

char * write_file()
{
	FILE *write_file;
	char *header_str = (char *)malloc(BUFSIZ);

	sprintf(header_str, "%s.h", argv[1]);
	printf("%s\n", header_str);
	file = fopen(header_str, "w+");

	fprintf(file, "This is just an example\n");
	fclose(file);
	return 0;
}
int main(int argc, char **argv)
{
	if (argc < 2) {
		puts("Usage: \v$ header [FILE]");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
