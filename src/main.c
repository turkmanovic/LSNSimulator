/*
 ============================================================================
 Name        : NetworkSimulator.c
 Author      : Haris Turkmanovic
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Log.h"
#include "nodes.h"
#include "tbase.h"
#include "topology.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	puts("Simulator v0.1"); /* prints !!!Hello World!!! */

	LOG_ERROR_Init();
	Init_ProcessLog();
	Init_DataLog();
	NODE_Init();
	DATA_Init();
	TBASE_Init();
	LINK_Init();
	TPLG_Init();
	puts("Simulation start");
    if(TBASE_Start(1000000) != TBASE_OK){
    	puts("Simulation failed");
    }
    else{
    	puts("Simulation successfully ended");
    }
    while(1);
	return 0;
}
