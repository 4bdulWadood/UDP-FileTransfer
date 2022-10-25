#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFSIZE 64;

#define MSG "Any Message"

int k = 1;

int main(int argc, char **argv){

while(1){
	char *host = "localhost";
	int port = 3000;

	struct hostend *phe; /*Pointer to host information entry*/
	struct sockaddr_in sin; /*An internet endpoint address*/
	
	struct pdu{
	   char type;
	   char data[100];
	};		

	int s, n, type; /*Socket Descriptor and Socket Type */

	switch(argc){
		case 1: 
		    break;
		case 2: 
		    host = argv[1];
	      case 3:
		    host = argv[1];
                port = atoi(argv[2]);
		    break;
		default:
		    fprintf(stderr, "Usage: UDP time [Host [Port]]\n");
		    exit(1);
	}	

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	/*Map host name to IP address, allowing for dotted decimal */
	if(phe = gethostbyname(host)){
	    memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	}
	else if((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
		fprintf(stderr, "Can't get host entry \n");

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0){
		fprintf(stderr, "Can't create socket \n");
	};

	if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "Can't connect to %s %s \n", host, "Time");

	struct pdu spdu;
	spdu.type = 'C';
	printf("Enter file name: \n");
	fscanf(stdin, "%s", &spdu.data);
	
	write(s, &spdu, 101);

	write_file(s);

	printf("\nType 0 to exit the program or 1 to download another file");
	int num;
	scanf("%d", &num);
	if(num==0){
	     exit(0);
	}
}
}

void write_file(int s){
	FILE *fp;

	struct pdu {
		char type;
		char data[100];
	}

	char filename[16];
	sprintf(filename, "file%d.txt", k);
	k++;
	int N;

	while(1){
		struct pdu spdu2;
		char buffer[101];
		
		N = recv(s, buffer, 101, 0);
		spdu2.type = buffer[0];
		int b;
		for(b = 0; b < sizeof(buffer)-1; b++){
			buffer[b]  = buffer[b+1];
		}

		strcpy(spdu2.data, buffer);

		if(spdu2.type == 'E'){
			printf("File not found on server");
			break;
		}

		if(spdu2.type == 'D' || spdu2.type == 'F'){
			fp = fopen(filename, "a");
			fprintf(fp, "%s", spdu2.data);
			fflush(fp);
		}

		if(spdu2.type == 'F'){
			printf("File Download Complete");
			break;
		}
	}	
}
