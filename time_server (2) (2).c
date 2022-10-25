#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	struct sockaddr_in fsin; /*The from address of the client */
	char buf[101];	/*Input buffer; any size > 0 */
	char *pts;
	int sock;	
	time_t now;
	int alen;
	struct sockaddr_in sin;
	int s, type;
	int port = 3000;
	int n;

	struct pdu {
		char type;
		char data[100];
	};

	switch(argc){
		case 1: 
			break;
		case 2:
			port = atoi(argv[1]);
			break;	
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);	
			exit(1);
	};

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0){
		fprintf(stderr, "Can't create a socket \n");	
	}

	if(bind(s, (struct sockaddr *)&sin, sizeof(sin))<0){
		fprintf(stderr, "can't bind to %d port \n", port);
	}

	while(1){
		listen(s, 5);
		alen = sizeof(fsin);
		struct pdu spdu;	
		int b;
		if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&fsin, &alen) < 0)
			fprintf(stderr, "recvfrom error \n");

		spdu.type = buf[0];
		
		for(b = 0; b < sizeof(buf)-1; b++){
			buf[b] = buf[b+1];
		}

		strcpy(spdu.data, buf);
		
		printf("%s\n", spdu.data);

		bzero(buf,101);

		FILE *fptr;

		fptr = fopen(spdu.data, "r");
	
		char temp[100];
		if(fptr == NULL){
			char zeroarray[100] = {[0 ... 99] = 'X'};
			struct pdu spdu2;
			spdu2.type = 'E';
			printf("Error Message Code: %c\n", spdu2.type);
			strcpy(spdu2.data, zeroarray);
			
			if(sendto(s, &spdu2, 101, 0, (struct sockaddr *)&fsin, sizeof(fsin)) == -1){
				printf("Error in Sending Data");
				continue;
			}
			continue;
		} 

		struct stat buffer;

		fptr = fopen(spdu.data, "r");
		int fd = fileno(fptr);
		fstat(fd, &buffer);
		int size = buffer.st_size;
		printf("%ld\n", size);
			
		while(fgets(temp, 100, fptr) != NULL){
			struct pdu spdu3;
			spdu3.type = 'D';
			size = size - 100;
			if(size<0){
				spdu3.type = 'F';
			}
			
			memcpy(spdu3.data, &temp[0], 99);

			if(sendto(s, &spdu3, 101, 0, (struct sockaddr *)&fsin, sizeof(fsin)) == -1){
				printf("Error in Sending Data");
				continue;
			}
		}

		bzero(temp, 100);
		fflush(fptr);
	}
}
