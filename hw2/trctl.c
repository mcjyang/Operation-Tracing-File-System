/*
 * CSE-506 : Operating Systems HW2
 * Author  : Meng-Chieh Yang
 * ID      : 110452591
 * Date    : 11/1/16
 *
 * trctl.c:
 * - for trfs file system for bimapping (user level)
 *
 */

#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include "trctl.h"


#ifndef EXTRA_CREDIT

int main(int argc, char * const argv[]){
	
	int rc;
	unsigned int hex_num;
	int fd;
	char* CMD = NULL; //argv[1] = CMD
	const char *mount_point = NULL; //arv[2]
	regex_t reg;

	if(argc<2){
		printf("error: not enough argument\n");
		exit(1);
	}

	mount_point = (const char *) argv[argc-1];
	fd = open(mount_point, O_RDONLY, 0);
	if (fd == -1) {
		printf("failed to open file\n");
		exit(fd);
	}

	if(argc==2){
		rc = ioctl(fd, RETRIEVE, &hex_num);
		printf("current bitmap: %x \n",hex_num);
		exit(rc);
	}

	CMD = (char *) argv[1];
	if(strcmp(CMD, "all")==0){
		rc = ioctl(fd, ALL, 0xff);

		
	}else if(strcmp(CMD, "none")==0){
		rc = ioctl(fd, NONE, 0);
		
	}else{
		// bitmap or not
		rc = regcomp(&reg, "^0[xX][0-9a-fA-F]{8}$", REG_EXTENDED);
		if(rc){
			printf("could not compile reg.\n");
			exit(rc);
		}

		rc = regexec(&reg, CMD, 0, NULL, 0);
		if(!rc){
			//bitmap format
			hex_num = (unsigned int)strtol(CMD, NULL, 0);
			rc = ioctl(fd, BITMAP, hex_num);

		}else{
			printf("this is not correct format. \n");
		}
		regfree(&reg);
	}

	//rc = 0;
	exit(rc);

}

#else

int main(int argc, char *const argv[]){
	printf("extra credit\n");
	exit(0);
}

#endif