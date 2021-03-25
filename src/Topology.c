/*
 * Topology.c
 *
 *  Created on: Aug 1, 2019
 *      Author: Puma
 */

#include "Log.h"
#include "topology.h"
#include "job.h"


FILE *	prvTPLG_CONFIG_FILE;

uint8_t prvTPLG_CreateNodes(){
	uint32_t TempId = 0, AggregationLevel,ReturnSize,MTUProcessOverhead;
	double		TempProcessTime, lpConsumption, activeConsumption;
	char TempChar;
	while(1){
		fscanf(prvTPLG_CONFIG_FILE, "%d%lf%d%d%d%lf%lf", &TempId, &TempProcessTime, &AggregationLevel, &ReturnSize, &MTUProcessOverhead, &activeConsumption, &lpConsumption);
		if(TempId == 0) break;
		if(NODE_Create(TempId, TempProcessTime,AggregationLevel,ReturnSize,MTUProcessOverhead, lpConsumption,activeConsumption,CONFIG_NODE_AGG_LEVEL)==NULL){
			return 1;
		}
		do{
			TempChar = fgetc(prvTPLG_CONFIG_FILE);
		}while(TempChar!='\n');
	}
	return 0;
}
uint8_t prvTPLG_CreateConnections(){
	uint32_t CurrentNodeId = 0, AdjacentNodeId ,TempLinkId,a,b;
	double  CurrentNodeTime;
	char TempChar;
	while(1){
		fscanf(prvTPLG_CONFIG_FILE, "%d%lf%d%d%d", &CurrentNodeId, &CurrentNodeTime,&a,&b,&b);
		if(CurrentNodeId == 0) break;
		TempChar = (char)fgetc(prvTPLG_CONFIG_FILE);
		while(TempChar != ';'){
			fscanf(prvTPLG_CONFIG_FILE, "%d-%d",&AdjacentNodeId, &TempLinkId);
			if(NODE_LinkToNode(CurrentNodeId,AdjacentNodeId,TempLinkId)!=0) return 1;
			TempChar = (char)fgetc(prvTPLG_CONFIG_FILE);
		}
		do{
			TempChar = fgetc(prvTPLG_CONFIG_FILE);
		}while(TempChar!='\n');
	}
	return 0;
}

uint8_t prvTPLG_CreateDataLink(){
	uint32_t* 	DataLine;
	uint32_t  	DataLineLength;
	uint32_t 	TempId, Rate,Size,ProtocolId;

	Boolean Periodic;
	char TempChar;
	while(1){
		    DataLineLength=0;
		    DataLine = NULL;
		    Periodic = False;
			fscanf(prvTPLG_CONFIG_FILE, "%d", &TempId);
			if(TempId == 0) break;
			do{
				TempChar = (char)fgetc(prvTPLG_CONFIG_FILE);
			}while(TempChar != ';') ;
			TempChar = (char)fgetc(prvTPLG_CONFIG_FILE);
			if(TempChar == '\n')continue;
			if(TempChar == 'p'){
				Periodic = True;
			}
			else{
				fseek(prvTPLG_CONFIG_FILE,-1,SEEK_CUR);
			}
			fscanf(prvTPLG_CONFIG_FILE,"%d-%d ",&Rate,&ProtocolId);
			fscanf(prvTPLG_CONFIG_FILE,"%d ",&Size);
			DataLine=malloc((DataLineLength+1)*sizeof(uint32_t));
			do{
				DataLineLength++;
				DataLine=realloc(DataLine,(DataLineLength)*sizeof(uint32_t));

				fscanf(prvTPLG_CONFIG_FILE,"%d",&DataLine[DataLineLength-1]);
			    TempChar = (char)fgetc(prvTPLG_CONFIG_FILE);
			}while(TempChar != ';') ;
			NODE_MakeProducerNode(TempId,Rate,Periodic,Size,DataLine,DataLine[DataLineLength-1],False,ProtocolId);
	}
	return 0;
}
uint8_t TPLG_Init(){
	prvTPLG_CONFIG_FILE = fopen(Topology_FileName,"r");
	if(prvTPLG_CONFIG_FILE == NULL){
		LOG_ERROR_Print(ERROR_FILE_0,Topology_FileName);
		return 1;
	}
	if(prvTPLG_CreateNodes()!=0) return 1;
	Print_ProcessLog(LOG_CREATED_ALL_NODES,"");
	rewind(prvTPLG_CONFIG_FILE);
	if(prvTPLG_CreateConnections()!=0) return 2;
	Print_ProcessLog(LOG_LINKED_ALL_NODES,"");
	rewind(prvTPLG_CONFIG_FILE);
	if(prvTPLG_CreateDataLink()!=0) return 3;
	fclose(prvTPLG_CONFIG_FILE);
	return 0;
}



