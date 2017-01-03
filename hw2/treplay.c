/*
 * CSE-506 : Operating Systems HW2
 * Author  : Meng-Chieh Yang
 * ID      : 110452591
 * Date    : 11/1/16
 * 
 * treplay.c:
 * - not quite done
 * - only can output binary format of record data
 *
 */


#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

#include "record.h"


int main(int argc, char *argv[]){
	
	
	int ch;
	int i;
	char * pathname;
	//struct record *r;
	FILE *file_ptr;
	unsigned char buffer[4096];

	if(argc<2){
		printf("error: not enough argument.\n");
		exit(1);
	}

	pathname = argv[argc-1];
	file_ptr = fopen(pathname,"rb");

	if(argc==2){
		// here no option
		fread(buffer, sizeof(buffer), 1, file_ptr);
		for(i=0; i<sizeof(buffer); i++){
					printf("%u ", buffer[i]);
		}
		printf("\n");
		goto exit;
	}


	if((ch = getopt(argc, argv, "ns"))!=-1){
		switch(ch){
			case 'n':
				fread(buffer, sizeof(buffer), 1, file_ptr);
				for(i=0; i<sizeof(buffer); i++){
					printf("%u ", buffer[i]);
				}
				printf("\n");
				break;
			case 's':
				fread(buffer, sizeof(buffer), 1, file_ptr);
				for(i=0; i<sizeof(buffer); i++){
					printf("%u ", buffer[i]);
				}
				printf("\n");
				break;
			case '?':
				printf("unknown flags\n");
				break;
		}
	}

exit:
	fclose(file_ptr);
	exit(0);

}