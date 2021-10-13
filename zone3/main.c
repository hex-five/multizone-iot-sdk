/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h> // strcmp()
#include "multizone.h"

int main (void){

    CSRS(mie, 1<<3);

	while(1){

		MZONE_WFI();

		char msg[16];

		if (MZONE_RECV(1, msg) && strcmp("payload", msg)==0)

			asm("tail sh_buff1_start");

	}

}
