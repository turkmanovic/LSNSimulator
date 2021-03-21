/*
 * Topology.c
 *
 *  Created on: Aug 1, 2019
 *      Author: Puma
 */

#include "Topology.h"
#include "Log.h"
#include "Job.h"


FILE *ConfigFile;

uint8_t CreateNodes(){
	uint32_t TempId = 0, AggregationLevel,ReturnSize,MTUProcessOverhead;
	double		TempProcessTime, lpConsumption, activeConsumption;
	char TempChar;
	while(1){
		fscanf(ConfigFile, "%d%lf%d%d%d%lf%lf", &TempId, &TempProcessTime, &AggregationLevel, &ReturnSize, &MTUProcessOverhead, &activeConsumption, &lpConsumption);
		if(TempId == 0) break;
		if(Create_Node(TempId, TempProcessTime,AggregationLevel,ReturnSize,MTUProcessOverhead, lpConsumption,activeConsumption)==NULL){
			return 1;
		}
		do{
			TempChar = fgetc(ConfigFile);
		}while(TempChar!='\n');
	}
	return 0;
}
uint8_t CreateConnections(){
	uint32_t CurrentNodeId = 0, AdjacentNodeId ,TempLinkId,a,b;
	double  CurrentNodeTime;
	char TempChar;
	while(1){
		fscanf(ConfigFile, "%d%lf%d%d%d", &CurrentNodeId, &CurrentNodeTime,&a,&b,&b);
		if(CurrentNodeId == 0) break;
		TempChar = (char)fgetc(ConfigFile);
		while(TempChar != ';'){
			fscanf(ConfigFile, "%d-%d",&AdjacentNodeId, &TempLinkId);
			if(Link_Node(CurrentNodeId,AdjacentNodeId,TempLinkId)!=0) return 1;
			TempChar = (char)fgetc(ConfigFile);
		}
		do{
			TempChar = fgetc(ConfigFile);
		}while(TempChar!='\n');
	}
	return 0;
}

uint8_t CreateDataLink(){
	uint32_t* DataLine;
	uint32_t  DataLineLength;
	uint32_t TempId, Rate,Size,ProtocolId;

	Boolean Periodic;
	char TempChar;
	while(1){
		    DataLineLength=0;
		    DataLine = NULL;
		    Periodic = False;
			fscanf(ConfigFile, "%d", &TempId);
			if(TempId == 0) break;
			do{
				TempChar = (char)fgetc(ConfigFile);
			}while(TempChar != ';') ;
			TempChar = (char)fgetc(ConfigFile);
			if(TempChar == '\n')continue;
			if(TempChar == 'p'){
				Periodic = True;
			}
			else{
				fseek(ConfigFile,-1,SEEK_CUR);
			}
			fscanf(ConfigFile,"%d-%d ",&Rate,&ProtocolId);
			fscanf(ConfigFile,"%d ",&Size);
			DataLine=malloc((DataLineLength+1)*sizeof(uint32_t));
			do{
				DataLineLength++;
				DataLine=realloc(DataLine,(DataLineLength)*sizeof(uint32_t));

				fscanf(ConfigFile,"%d",&DataLine[DataLineLength-1]);
			    TempChar = (char)fgetc(ConfigFile);
			}while(TempChar != ';') ;
			Make_Producer_Node(TempId,Rate,Periodic,Size,DataLine,DataLine[DataLineLength-1],False,ProtocolId);
	}
	return 0;
}
uint8_t Init_Topology(){
	ConfigFile = fopen(Topology_FileName,"r");
	if(ConfigFile == NULL){
		Print_ErrorLog(ERROR_FILE_0,Topology_FileName);
		return 1;
	}
	if(CreateNodes()!=0) return 1;
	Print_ProcessLog(LOG_CREATED_ALL_NODES,"");
	rewind(ConfigFile);
	if(CreateConnections()!=0) return 2;
	Print_ProcessLog(LOG_LINKED_ALL_NODES,"");
	rewind(ConfigFile);
	if(CreateDataLink()!=0) return 3;
	fclose(ConfigFile);
	return 0;
}



