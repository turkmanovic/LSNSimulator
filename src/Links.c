/*
 * Links.c
 *
 *  Created on: Aug 5, 2019
 *      Author: Puma
 */

#include "Links.h"
#include "Log.h"
#include <string.h>
#include <stdlib.h>


uint32_t        ConnectionNo;
uint32_t        LinksNo;

FILE 			*LinkFile;
Link** 			DefinedLinks;
uint32_t 		Links;


Link*   Get_Link(uint32_t LinkID){
	uint32_t i=0;
	while(i < LinksNo){
		if(DefinedLinks[i]->ID == LinkID){
			return DefinedLinks[i];
		}
		i++;
	}
	return NULL;
}

double       Get_LinkSpeed(Connection* ConnectionPtr){
	double Temp;
	double Minimum =ConnectionPtr->AssignedLink->Speed-(double)(ConnectionPtr->AssignedLink->Deviation/100)*ConnectionPtr->AssignedLink->Speed;
	Temp = (double)(rand()%(int)((2*((double)ConnectionPtr->AssignedLink->Deviation/100)*ConnectionPtr->AssignedLink->Speed)*10000000+1))/1000000+Minimum;
	return Temp;

}

uint8_t Parse_Links(){
	char TempChar;
	uint32_t TempId;
	float TempSpeed;
	uint32_t Deviation, MTUSize, NumberOfHops;
	char TempString[50];
	double TransmitConsumption, ReceiveConsumption;
	LinksNo = 0;
	while(1){
		TempChar = (char)fgetc(LinkFile);
		if(TempChar == '-') break;
		fseek(LinkFile,-1,SEEK_CUR);
		fscanf(LinkFile,"%d %f %d %d %d %lf %lf %s",&TempId, &TempSpeed,&Deviation,&MTUSize, &NumberOfHops, &TransmitConsumption, &ReceiveConsumption, TempString);
		if(Get_Link(TempId) != NULL){
			sprintf(TempString,"%d",TempId);
			Print_ErrorLog(ERROR_LINK_0,TempString);
			return 1;
		}
		LinksNo++;
		DefinedLinks = realloc(DefinedLinks,LinksNo*sizeof(Link*));
		DefinedLinks[LinksNo-1]=malloc(sizeof(Link));
		DefinedLinks[LinksNo-1]->ID = TempId;
		DefinedLinks[LinksNo-1]->Speed = TempSpeed;
		DefinedLinks[LinksNo-1]->Deviation = Deviation;
		DefinedLinks[LinksNo-1]->MTUSize = MTUSize;
		DefinedLinks[LinksNo-1]->NumberOfHops = NumberOfHops;
		DefinedLinks[LinksNo-1]->TransmitConsumption = TransmitConsumption;
		DefinedLinks[LinksNo-1]->ReceiveConsumption = ReceiveConsumption;
		strcpy(DefinedLinks[LinksNo-1]->LinkName, TempString);
		sprintf(TempString,"ID: %d, Name: %s",DefinedLinks[LinksNo-1]->ID,DefinedLinks[LinksNo-1]->LinkName);
		Print_ProcessLog(LOG_LINK_0,TempString);
		while(TempChar != '\n') TempChar = (char)fgetc(LinkFile);
	}
	Print_ProcessLog(LOG_LINK_1,"");
	fclose(LinkFile);
	return 0;
}



uint8_t Init_Links(){
	LinksNo = 0;
	LinkFile = fopen(Link_FileName,"r");
	if(LinkFile == 0){
		Print_ErrorLog(ERROR_FILE_0,Link_FileName);
		return 1;
	}
	DefinedLinks=malloc(sizeof(Link*));
	if(DefinedLinks == 0){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Problem with links allocation");
		return 1;
	}
	if(Parse_Links()!=0){
		return 1;
	}
	return 0;
}

Connection*    	Create_Connection(uint32_t DestinationNodeId, uint32_t SourceNodeId, uint32_t LinkId){
	Connection* ReturnConnection = NULL;
	char TempString[30];
	ReturnConnection = malloc(sizeof(Connection));
	if(ReturnConnection == NULL){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Link Allocation");
		return NULL;
	}
	ReturnConnection->ID = ConnectionNo++;
	ReturnConnection->SourceNodeId = SourceNodeId;
	ReturnConnection->DestinationNodeId = DestinationNodeId;
	ReturnConnection->AssignedLink = Get_Link(LinkId);
	if(ReturnConnection->AssignedLink == NULL){
		sprintf(TempString,"%d",LinkId);
		Print_ErrorLog(ERROR_LINK_1,TempString);
		free(ReturnConnection);
		return NULL;
	}
	return ReturnConnection;
}
