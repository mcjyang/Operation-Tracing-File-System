obj-m += trctl.o

INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

default: trctl treplay

trctl: trctl.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi trctl.c -o trctl

treplay: treplay.c
	gcc -Wall -Werror -I$(INC)/generated/uapi -I$(INC)/uapi treplay.c -o treplay

clean: 
	rm -f trctl treplay