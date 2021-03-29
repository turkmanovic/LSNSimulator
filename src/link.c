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

connection_t**  prvCONNECTION_LIST;

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
	prvCONNECTION_LIST = malloc(sizeof(connection_t*));
	prvLINK_CONNECTION_NUMBERS = 0;;
	return 0;
}

connection_t*		CONNECTION_GetTwoNodesConnection(node_t* node1, node_t* node2){
	if(node1->connectionNumber == 0 || node2->connectionNumber == 0) return NULL;
	uint32_t counter = 0;
	while(counter < node1->connectionNumber){
		if(((node1->connections[counter]->node1->ID == node2->ID && node1->connections[counter]->node2->ID == node1->ID) ||
				(node1->connections[counter]->node1->ID == node1->ID && node1->connections[counter]->node2->ID == node2->ID))){
			return node1->connections[counter];
		}
		counter++;
	}
	counter = 0;
	while(counter < node2->connectionNumber){
		if(((node2->connections[counter]->node1->ID == node2->ID && node2->connections[counter]->node2->ID == node1->ID) ||
				(node2->connections[counter]->node1->ID == node1->ID && node2->connections[counter]->node2->ID == node2->ID))){
			return node2->connections[counter];
		}
		counter++;
	}
	return NULL;
}
connection_t*    	CONNECTION_Create(node_t* node1, node_t* node2, uint32_t LinkId){
	char TempString[30];
//	ReturnConnection = malloc(sizeof(connection_t));
//	if(ReturnConnection == NULL){
//		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Link Allocation");
//		return NULL;
//	}
//	ReturnConnection->ID = prvLINK_CONNECTION_NUMBERS++;
//	ReturnConnection->SourceNodeId = SourceNodeId;
//	ReturnConnection->DestinationNodeId = DestinationNodeId;
//	ReturnConnection->AssignedLink = LINK_GetByID(LinkId);
//	ReturnConnection->bussy		   = False;
//	if(ReturnConnection->AssignedLink == NULL){
//		sprintf(TempString,"%d",LinkId);
//		LOG_ERROR_Print(ERROR_LINK_1,TempString);
//		free(ReturnConnection);
//		return NULL;
//	}
	//Check if connection already exists
	uint32_t counter = 0;
	while(counter < prvLINK_CONNECTION_NUMBERS){
		if((prvCONNECTION_LIST[counter]->node1->ID == node1->ID && prvCONNECTION_LIST[counter]->node2->ID == node2->ID) ||
				(prvCONNECTION_LIST[counter]->node2->ID == node1->ID && prvCONNECTION_LIST[counter]->node1->ID == node2->ID)){
			return prvCONNECTION_LIST[counter];
		}
		counter++;
	}
	//if there is no connection, create new connection
	prvCONNECTION_LIST = realloc(prvCONNECTION_LIST, (prvLINK_CONNECTION_NUMBERS+1)*sizeof(connection_t*));
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS] = malloc(sizeof(connection_t));
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->ID = prvLINK_CONNECTION_NUMBERS;
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->node1 = node1;
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->node2 = node2;
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->currentTransferData = NULL;
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->bussy = False;
	prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->AssignedLink = LINK_GetByID(LinkId);
	if(prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]->AssignedLink == NULL){
		sprintf(TempString,"%d",LinkId);
		LOG_ERROR_Print(ERROR_LINK_1,TempString);
		free(prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS]);
		return NULL;
	}
	prvLINK_CONNECTION_NUMBERS+=1;

	return prvCONNECTION_LIST[prvLINK_CONNECTION_NUMBERS - 1];
}
