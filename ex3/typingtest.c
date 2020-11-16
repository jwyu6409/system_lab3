#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#define PASSWORDSIZE 12
static inline long myclock(){
	struct timeval tv;
	gettimeofday (&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int count = 0;

int main(void) {
	long t, dt;
	int fd;
	int nread, cnt = 0, errcnt = 0;
	char ch, text[] = "System Programming Lab3";

	struct termios init_attr, new_attr;

	fd = open(ttyname(fileno (stdin)), O_RDWR);
	tcgetattr(fd, &init_attr);
	new_attr = init_attr;
	new_attr.c_lflag &= ~ICANON;
	new_attr.c_lflag &= ~ECHO;
	new_attr.c_cc[VMIN] = 1;
	new_attr.c_cc[VTIME] = 0;
	if(tcsetattr(fd, TCSANOW, &new_attr) != 0){
		fprintf(stderr, "터미널 속성 설정 불가!!");
	}
	printf("다음 문장을 입력하시오. \n%s\n", text);
	t = myclock();

	while((nread = read(fd, &ch, 1)) > 0 && ch != '\n'){
		if(ch == text[cnt++]){
			write(fd, &ch, 1);
			count++;
		}
		else{
			write(fd, "*", 1);
			errcnt++;
		}
	}

	printf("오타 횟수 : %d\n", errcnt);
	dt = myclock() - t;
	printf("분당 평균 타자수 : %d // %d sec \n", count, dt/1000);
	tcsetattr(fd, TCSANOW, &init_attr);
	close(fd);
	return 0;
}
