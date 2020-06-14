#include <linux/fs.h>
#include <stdio.h>
int main()
{
	char buffer[64];
	int count;
	int fd;
	fd = open("/dev/position", 0);
	read(fd, buffer, 30);
	printf("%s", buffer);
	close(fd);
	return 0;
}

