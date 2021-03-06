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


uint8_t Init_ErrorLog(){
	ErrorFile = fopen(ErrorLog_FileName,"w");
	if(ErrorFile == NULL){
		Print_ErrorLog(ERROR_FILE_0,ErrorLog_FileName);
		return 1;
	}
	return 0;

}
uint8_t Init_DataLog(){
	DataElapsedTimeFile = fopen(ElapsedTimeLog_FileName,"w");
	if(DataElapsedTimeFile == NULL){
		Print_ErrorLog(ERROR_FILE_0,ElapsedTimeLog_FileName);
		return 1;
	}
	return 0;

}
uint8_t Init_ProcessLog(){
	LogFile = fopen(ProcessLog_FileName,"w");
	if(LogFile == NULL){
		Print_ErrorLog(ERROR_FILE_0,ProcessLog_FileName);
		return 1;
	}
	return 0;
}
void Print_ErrorLog(uint16_t ErrorNumber, const char AddInfo[]){
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



void Print_DataLog(Data* DataPtr,FILE* FileToWrite,double Time){
	char Text[30];
	sprintf(Text,"%f",Time);
	fprintf(FileToWrite,Text);
	fprintf(FileToWrite,",");
	sprintf(Text,"%d",DataPtr->BytesToProcess);
	fprintf(FileToWrite,Text);
	fprintf(FileToWrite,",");
	switch(DataPtr->State){
	case Created:
		fprintf(FileToWrite,"C");
		break;
	case Wait:
		fprintf(FileToWrite,"W");
		break;
	case Received:
		fprintf(FileToWrite,"R");
		break;
	case Processed:
		fprintf(FileToWrite,"P");
		break;
	case Consumed:
		fprintf(FileToWrite,"M");
		if(DataPtr->AssignedProtocol->Response == True){
			if(DataPtr->Type == Response){
				fprintf(DataElapsedTimeFile,"%d,%lf,%lf,%lf\n",DataPtr->ID,DataPtr->CreatedTime,DataPtr->ElapsedRequestTime, DataPtr->ElapsedResponseTime);
			}
		}
		else{
			fprintf(DataElapsedTimeFile,"%d,%lf,%lf,%lf\n",DataPtr->ID,DataPtr->CreatedTime, DataPtr->ElapsedRequestTime, 0.0);
		}
		break;
	case Sent:
		fprintf(FileToWrite,"S");
		break;
	}
	if(DataPtr->Type == Request)fprintf(FileToWrite,"_Req");
	if(DataPtr->Type == Response)fprintf(FileToWrite,"_Res");
	fprintf(FileToWrite,",");
	sprintf(Text,"%d",DataPtr->ID);
	fprintf(FileToWrite,Text);
}
void Print_NodeLog(Node* NodePtr,Data* DataPtr,double Time){
	FILE* fp = fopen(NodePtr->LogFilename, "a");
	Print_DataLog(DataPtr,fp,Time);
	char Text[30];
	if(NodePtr->ProcessingData == NULL || DataPtr->State == Consumed){
		sprintf(Text,"%d",NodePtr->SizeOfWaitData);
	}
	else{
		sprintf(Text,"%d",NodePtr->SizeOfWaitData+NodePtr->ProcessingData->BytesToProcess);
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
	sprintf(Text,"%lf",NodePtr->FullConsumption);
	fprintf(fp,Text);
	fprintf(fp,"\n");
	fclose(fp);
}
