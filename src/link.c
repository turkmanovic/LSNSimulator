/*
 * Links.c
 *
 *  Created on: Aug 5, 2019
 *      Author: Puma
 */

#include "Log.h"
#include <string.h>
#include <stdlib.h>
#include "link.h"


uint32_t        prvLINK_CONNECTION_NUMBERS;
uint32_t        prvLINK_NUMBERS;

FILE 			*LinkFile;
link_t** 		DefinedLinks;
uint32_t 		Links;


link_t*   LINK_GetByID(uint32_t LinkID){
	uint32_t i=0;
	while(i < prvLINK_NUMBERS){
		if(DefinedLinks[i]->ID == LinkID){
			return DefinedLinks[i];
		}
		i++;
	}
	return NULL;
}

double       LINK_GetSpeed(connection_t* ConnectionPtr){
	double Temp;
	double Minimum =ConnectionPtr->AssignedLink->Speed-(double)(ConnectionPtr->AssignedLink->Deviation/100)*ConnectionPtr->AssignedLink->Speed;
	Temp = (double)(rand()%(int)((2*((double)ConnectionPtr->AssignedLink->Deviation/100)*ConnectionPtr->AssignedLink->Speed)*10000000+1))/1000000+Minimum;
	return Temp;

}

uint8_t prvLINK_Parse(){
	char TempChar;
	uint32_t TempId;
	float TempSpeed;
	uint32_t Deviation, MTUSize, NumberOfHops;
	char TempString[50];
	double TransmitConsumption, ReceiveConsumption;
	prvLINK_NUMBERS = 0;
	while(1){
		TempChar = (char)fgetc(LinkFile);
		if(TempChar == '-') break;
		fseek(LinkFile,-1,SEEK_CUR);
		fscanf(LinkFile,"%d %f %d %d %d %lf %lf %s",&TempId, &TempSpeed,&Deviation,&MTUSize, &NumberOfHops, &TransmitConsumption, &ReceiveConsumption, TempString);
		if(LINK_GetByID(TempId) != NULL){
			sprintf(TempString,"%d",TempId);
			LOG_ERROR_Print(ERROR_LINK_0,TempString);
			return 1;
		}
		prvLINK_NUMBERS++;
		DefinedLinks = realloc(DefinedLinks,prvLINK_NUMBERS*sizeof(link_t*));
		DefinedLinks[prvLINK_NUMBERS-1]=malloc(sizeof(link_t));
		DefinedLinks[prvLINK_NUMBERS-1]->ID = TempId;
		DefinedLinks[prvLINK_NUMBERS-1]->Speed = TempSpeed;
		DefinedLinks[prvLINK_NUMBERS-1]->Deviation = Deviation;
		DefinedLinks[prvLINK_NUMBERS-1]->MTUSize = MTUSize;
		DefinedLinks[prvLINK_NUMBERS-1]->NumberOfHops = NumberOfHops;
		DefinedLinks[prvLINK_NUMBERS-1]->TransmitConsumption = TransmitConsumption;
		DefinedLinks[prvLINK_NUMBERS-1]->ReceiveConsumption = ReceiveConsumption;
		strcpy(DefinedLinks[prvLINK_NUMBERS-1]->LinkName, TempString);
		sprintf(TempString,"ID: %d, Name: %s",DefinedLinks[prvLINK_NUMBERS-1]->ID,DefinedLinks[prvLINK_NUMBERS-1]->LinkName);
		Print_ProcessLog(LOG_LINK_0,TempString);
		while(TempChar != '\n') TempChar = (char)fgetc(LinkFile);
	}
	Print_ProcessLog(LOG_LINK_1,"");
	fclose(LinkFile);
	return 0;
}



uint8_t LINK_Init(){
	prvLINK_NUMBERS = 0;
	LinkFile = fopen(Link_FileName,"r");
	if(LinkFile == 0){
		LOG_ERROR_Print(ERROR_FILE_0,Link_FileName);
		return 1;
	}
	DefinedLinks=malloc(sizeof(link_t*));
	if(DefinedLinks == 0){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Problem with links allocation");
		return 1;
	}
	if(prvLINK_Parse()!=0){
		return 1;
	}
	return 0;
}

connection_t*    	LINK_CreateConnection(uint32_t DestinationNodeId, uint32_t SourceNodeId, uint32_t LinkId){
	connection_t* ReturnConnection = NULL;
	char TempString[30];
	ReturnConnection = malloc(sizeof(connection_t));
	if(ReturnConnection == NULL){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Link Allocation");
		return NULL;
	}
	ReturnConnection->ID = prvLINK_CONNECTION_NUMBERS++;
	ReturnConnection->SourceNodeId = SourceNodeId;
	ReturnConnection->DestinationNodeId = DestinationNodeId;
	ReturnConnection->AssignedLink = LINK_GetByID(LinkId);
	if(ReturnConnection->AssignedLink == NULL){
		sprintf(TempString,"%d",LinkId);
		LOG_ERROR_Print(ERROR_LINK_1,TempString);
		free(ReturnConnection);
		return NULL;
	}
	return ReturnConnection;
}
