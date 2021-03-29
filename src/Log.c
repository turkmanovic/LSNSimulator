/*
 * Error.c
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */


#include "Log.h"


FILE *ErrorFile;
FILE *LogFile;
FILE *DataElapsedTimeFile;


uint8_t LOG_ERROR_Init(){
	ErrorFile = fopen(ErrorLog_FileName,"w");
	if(ErrorFile == NULL){
		LOG_ERROR_Print(ERROR_FILE_0,ErrorLog_FileName);
		return 1;
	}
	return 0;

}
uint8_t Init_DataLog(){
	DataElapsedTimeFile = fopen(ElapsedTimeLog_FileName,"w");
	if(DataElapsedTimeFile == NULL){
		LOG_ERROR_Print(ERROR_FILE_0,ElapsedTimeLog_FileName);
		return 1;
	}
	fclose(DataElapsedTimeFile);
	return 0;

}
uint8_t Init_ProcessLog(){
	LogFile = fopen(ProcessLog_FileName,"w");
	if(LogFile == NULL){
		LOG_ERROR_Print(ERROR_FILE_0,ProcessLog_FileName);
		return 1;
	}
	return 0;
}
void LOG_ERROR_Print(uint16_t ErrorNumber, const char AddInfo[]){
	switch(ErrorNumber){
		case ERROR_NODE_0:
			fprintf(ErrorFile,"Two nodes with same ID (");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile,")\n");
			break;
	    case ERROR_NODE_1:
			fprintf(ErrorFile,"Node ID must be greater than 0 ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile,"\n");
			break;
		case ERROR_LINKING_0:
			fprintf(ErrorFile,"Double linking ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," \n");
			break;
		case ERROR_LINKING_1:
			fprintf(ErrorFile,"Link with ID ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," cant be found to link with other nodes\n");
			break;
		case ERROR_FILE_0:
			fprintf(ErrorFile,"File ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile,"cant be open\n");
			break;
		case ERROR_ALLOCATING_0:
			fprintf(ErrorFile,"Problem with memory allocation! AddInfo: ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," \n");
			break;
		case ERROR_PROTOCOL_0:
			fprintf(ErrorFile,"There is multiple protocols with id ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," \n");
			break;
		case ERROR_PROTOCOL_1:
			fprintf(ErrorFile,"Protocol with id ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," can't be found\n");
			break;
		case ERROR_LINK_0:
			fprintf(ErrorFile,"There is multiple links with id ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," \n");
			break;
		case ERROR_LINK_1:
			fprintf(ErrorFile,"Link with id ");
			fprintf(ErrorFile,AddInfo);
			fprintf(ErrorFile," can't be found\n");
			break;
	}
}
void Print_ProcessLog(uint16_t LogID, const char AddInfo[]){
	switch(LogID){
	case LOG_CREATED_NODE :
		fprintf(LogFile,"Node ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile," Created\n");
		break;
	case LOG_LINKED_NODE:
		fprintf(LogFile,"Nodes ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile," are linked\n");
		break;
	case LOG_CREATED_ALL_NODES:
		fprintf(LogFile,"Nodes are successfully created - ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile,"\n\n\n");
		break;
	case LOG_LINKED_ALL_NODES:
		fprintf(LogFile,"Node's network are created - ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile,"\n");
		break;
	case LOG_PROTOCOL_0:
		fprintf(LogFile,"Protocol \" ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile," \" is created \n");
		break;
	case LOG_PROTOCOL_1:
		fprintf(LogFile,"All protocols are created ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile,"\n\n\n");
		break;
	case LOG_LINK_0:
		fprintf(LogFile,"Link \" ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile," \" is created \n");
		break;
	case LOG_LINK_1:
		fprintf(LogFile,"All links are created ");
		fprintf(LogFile,AddInfo);
		fprintf(LogFile,"\n\n\n");
		break;
	}
}



void Print_DataLog(data_t* DataPtr,FILE* FileToWrite,double Time){
	char Text[30];
	sprintf(Text,"%d",DataPtr->BytesToProcess);
	fprintf(FileToWrite,Text);
	fprintf(FileToWrite,",");
	if(DataPtr->aggregationFlag == DATA_AGG_FULL){
		fprintf(FileToWrite,"A_");
		for(uint32_t i = 0; i < DataPtr->aggreationList->numberOfData;i++){
			fprintf(FileToWrite,"%d_", DataPtr->aggreationList->dataList[i]->ID);
		}
	}
	switch(DataPtr->State){
		case DATA_STATE_CREATED:
			fprintf(FileToWrite,"C");
			break;
		case DATA_STATE_WAIT:
			fprintf(FileToWrite,"W");
			break;
		case DATA_STATE_RECEIVE_END:
			fprintf(FileToWrite,"R_E");
			break;
		case DATA_STATE_RECEIVE_START:
			fprintf(FileToWrite,"R_S");
			break;
		case DATA_STATE_PROCESS_END:
			fprintf(FileToWrite,"P_");
			if(DataPtr->overheadProccesFlag == 1){
				fprintf(FileToWrite,"O_");
			}
			else{
				fprintf(FileToWrite,"D_");
			}
			fprintf(FileToWrite,"E");
			break;
		case DATA_STATE_PROCESS_START:
			fprintf(FileToWrite,"P_");
			if(DataPtr->overheadProccesFlag == 1){
				fprintf(FileToWrite,"O_");
			}
			else{
				fprintf(FileToWrite,"D_");
			}
			fprintf(FileToWrite,"S");
			break;
		case DATA_STATE_CONSUMED:
			fprintf(FileToWrite,"M");
			if(DataPtr->AssignedProtocol->Response == True){
				if(DataPtr->Type == DATA_TYPE_RESPONSE){
					DataElapsedTimeFile = fopen(ElapsedTimeLog_FileName,"a");
					fprintf(DataElapsedTimeFile,"%d,%.3lf,%.3lf,%.3lf,%.3lf\n",DataPtr->ID,DataPtr->CreatedTime,DataPtr->ElapsedRequestTime, DataPtr->ElapsedResponseTime,DataPtr->ElapsedRequestTime + DataPtr->ElapsedResponseTime);
					fclose(DataElapsedTimeFile);
				}
			}
			else{
				DataElapsedTimeFile = fopen(ElapsedTimeLog_FileName,"a");
				fprintf(DataElapsedTimeFile,"%d,%lf,%lf,%lf,%.3lf\n",DataPtr->ID,DataPtr->CreatedTime, DataPtr->ElapsedRequestTime, 0.0, DataPtr->ElapsedRequestTime);
				fclose(DataElapsedTimeFile);
			}
			break;
		case DATA_STATE_TRANSMIT_END:
			fprintf(FileToWrite,"T_E");
			break;
		case DATA_STATE_TRANSMIT_START:
			fprintf(FileToWrite,"T_S");
			break;
	}
	if(DataPtr->Type == DATA_TYPE_REQUEST)fprintf(FileToWrite,"_Req");
	if(DataPtr->Type == DATA_TYPE_RESPONSE)fprintf(FileToWrite,"_Res");
	fprintf(FileToWrite,",");
	sprintf(Text,"%d",DataPtr->ID);
	fprintf(FileToWrite,Text);
}
void Print_NodeLog(node_t* NodePtr,data_t* DataPtr, double Time, uint8_t lpStatusFlag, uint8_t lpMode){
	char Text[30];
	FILE* fp = fopen(NodePtr->LogFilename, "a");
	sprintf(Text,"%.3f",Time);
	fprintf(fp,Text);
	fprintf(fp,",");
	if(NodePtr->operationalMode == NODE_OPMODE_FULLOPERATONAL){
		sprintf(Text,"%C",'A');
	}
	else{
		sprintf(Text,"%s","LP");
	}
	fprintf(fp,Text);
	fprintf(fp,",");
	if(lpStatusFlag == 0){
		Print_DataLog(DataPtr,fp,Time);
		if(NodePtr->processingData == NULL || DataPtr->State == DATA_STATE_CONSUMED){
			sprintf(Text,"%d",NodePtr->SizeOfWaitData);
		}
		else{
			sprintf(Text,"%d",NodePtr->SizeOfWaitData+NodePtr->processingData->BytesToProcess);
		}
		fprintf(fp,",");
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->SizeOfWaitData);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->ProcessedDataBytesCount);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->ProcessedOverheadBytesCount);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->processedDataBufferSize);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->currentConsumption);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%.3lf",NodePtr->FullConsumption);
		fprintf(fp,Text);
		fprintf(fp,"\n");
	}
	else{
		if(lpMode == 1){
			sprintf(Text,"%s","ENTER");
		}
		else{
			sprintf(Text,"%s","EXIT");
		}
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%d",NodePtr->currentConsumption);
		fprintf(fp,Text);
		fprintf(fp,",");
		sprintf(Text,"%.3lf",NodePtr->FullConsumption);
		fprintf(fp,Text);
		fprintf(fp,"\n");

	}
	fclose(fp);
}
