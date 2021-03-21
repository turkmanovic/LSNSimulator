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
#include "Nodes.h"
#include "TimeBase.h"
#include "Log.h"
#include "Topology.h"

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	Init_ErrorLog();
	Init_ProcessLog();
	Init_DataLog();
	Init_Node();
	Init_Data();
	Init_TimeBase();
	Init_Links();
	Init_Topology();
	puts("Simulation start");
    StartTime(10000);
    puts("Simulation end");
    while(1);
	return 0;
}
