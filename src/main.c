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
    TBASE_Start(10000);
    puts("Simulation end");
    while(1);
	return 0;
}
