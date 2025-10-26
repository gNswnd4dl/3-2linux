#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAXLINE 100

/*이름있는 파이프를 통해 읽은 내용을 프린트함*/
int readLine(int fd, char *str){
	int n;
	do {
		n = read(fd, str, 1);
	} while (n > 0 && *str++ != '\0');
	return (n > 0);
}

int main() {
	int fd;
	char str[MAXLINE];
	unlink("myPipe");
	mkfifo("myPipe", 0660);
	fd = open("myPipo",O_RDONLY);

	while (readLine(fd,str))
		printf("%s\n", str);
	close(fd);
	return 0;
}

