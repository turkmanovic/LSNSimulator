/*
 * Nodes.c
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */
#include <math.h>

#include "job.h"
#include "Log.h"
#include "tbase.h"


node_t** 		prvNODE_LIST;
uint32_t 		prvNODE_COUNTER;
uint32_t*  		prvNODE_ID;


char 			LogText[50];


data_t*  		prvNODE_GetDataFromReceiveWaitingList(node_t* NodePtr){
	data_t* DataToSend = NULL;
	uint32_t Counter = 0;
	if(NodePtr->NoOfWaitData != 0){
		DataToSend = NodePtr->WaitData[0];
		for(Counter = 0; Counter < NodePtr->NoOfWaitData - 1 ;Counter++){
			NodePtr->WaitData[Counter] = NodePtr->WaitData[Counter+1];
		}
		NodePtr->NoOfWaitData--;
		NodePtr->SizeOfWaitData-=DataToSend->BytesToProcess;
		NodePtr->WaitData= realloc(NodePtr->WaitData,NodePtr->NoOfWaitData*sizeof(data_t*));
	}
	return DataToSend;
}
void  			prvNODE_PutDataToReceiveWaitingList(node_t* NodePtr, data_t* DataPtr){
	NodePtr->NoOfWaitData++;
	NodePtr->SizeOfWaitData+=DataPtr->BytesToProcess;
	NodePtr->WaitData = realloc(NodePtr->WaitData,NodePtr->NoOfWaitData*sizeof(data_t));
	NodePtr->WaitData[NodePtr->NoOfWaitData-1] = DataPtr;
	DataPtr->State = DATA_STATE_WAIT;
}

void 			NODE_Init(){
	prvNODE_LIST 		= malloc(sizeof(node_t*));
	prvNODE_LIST[0] 	= NULL;
	prvNODE_COUNTER		= 0;
	prvNODE_ID			= malloc(1*sizeof(uint32_t));
}
node_t*			NODE_Create(uint32_t NodeId, double ProcessTime, uint32_t AgregationLevel,uint32_t ReturnSize,uint32_t MTUProcessOverhead, double lpConsumption, double activeConsumption){
	if(NodeId < 1){
		sprintf(LogText, "%d", NodeId);
		LOG_ERROR_Print(ERROR_NODE_1, LogText);
		return NULL;
	}
	int i = 0;
	for (i =0; i< prvNODE_COUNTER;i++){
		if(prvNODE_ID[i] == NodeId){
			sprintf(LogText, "%d", NodeId);
            LOG_ERROR_Print(ERROR_NODE_0, LogText);
			return NULL;
		}
		i++;
	}
	prvNODE_COUNTER++;
	prvNODE_LIST = realloc(prvNODE_LIST,(prvNODE_COUNTER+1)*sizeof(node_t*));
	prvNODE_ID = realloc(prvNODE_ID,(prvNODE_COUNTER+1)*sizeof(uint32_t));
	prvNODE_ID[prvNODE_COUNTER-1] = NodeId;
	prvNODE_LIST[prvNODE_COUNTER-1] = malloc(sizeof(node_t));
	prvNODE_LIST[prvNODE_COUNTER-1]->ID = NodeId;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessTime = ProcessTime;
	prvNODE_LIST[prvNODE_COUNTER-1]->Processing = False;
	prvNODE_LIST[prvNODE_COUNTER-1]->lpConsumption = lpConsumption;
	prvNODE_LIST[prvNODE_COUNTER-1]->activeConsumption = activeConsumption;
	prvNODE_LIST[prvNODE_COUNTER-1]->FullConsumption = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessingData = NULL;
	prvNODE_LIST[prvNODE_COUNTER-1]->WaitData = malloc(sizeof(data_t*));
	prvNODE_LIST[prvNODE_COUNTER-1]->NoOfWaitData = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->SizeOfWaitData = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->lpEnterTime = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->MTUProcessOverhead = MTUProcessOverhead;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessedDataBytesCount = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->AggregationLevel = AgregationLevel;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessedOverheadBytesCount = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->Type = NODE_TYPE_CONSUMER;
	prvNODE_LIST[prvNODE_COUNTER-1]->operationalMode = NODE_OPMODE_LOWPOWER;
	prvNODE_LIST[prvNODE_COUNTER-1]->DataBufferSize = ReturnSize;
	prvNODE_LIST[prvNODE_COUNTER-1]->AdjacentNodes = malloc(sizeof(link_t*));
	prvNODE_LIST[prvNODE_COUNTER-1]->AdjacentNodes[0] = NULL;
	memset(prvNODE_LIST[prvNODE_COUNTER-1]->LogFilename, 0, NODE_FILENAME_SIZE);
	sprintf(prvNODE_LIST[prvNODE_COUNTER-1]->LogFilename, "Log/Nodes/%d.txt", NodeId);
	//Try to open file for testing purpose
	prvNODE_LIST[prvNODE_COUNTER-1]->NodeLogFile=fopen(prvNODE_LIST[prvNODE_COUNTER-1]->LogFilename, "w");
	if(prvNODE_LIST[prvNODE_COUNTER-1]->NodeLogFile == NULL){
		LOG_ERROR_Print(ERROR_FILE_0,prvNODE_LIST[prvNODE_COUNTER-1]->LogFilename);
		return NULL;
	}
	fclose(prvNODE_LIST[prvNODE_COUNTER-1]->NodeLogFile);
	sprintf(LogText, "%d", NodeId);
	Print_ProcessLog(LOG_CREATED_NODE, LogText);
	prvNODE_LIST[prvNODE_COUNTER] = NULL;
	return prvNODE_LIST[prvNODE_COUNTER-1];
}
node_t*   		NODE_GetById(uint32_t NodeId){
	int CurrentNodesList = 0;
	for(CurrentNodesList=0; CurrentNodesList<prvNODE_COUNTER; CurrentNodesList++){
		if(prvNODE_LIST[CurrentNodesList]->ID == NodeId){
			return prvNODE_LIST[CurrentNodesList];
		}
	}
	return NULL;
}
uint8_t			NODE_LinkToNode(uint32_t Node1Id, uint32_t Node2Id,uint32_t LinkID){
	node_t* NodeFrom= NODE_GetById(Node1Id);
	node_t* NodeTo= NODE_GetById(Node2Id);
	if(NodeFrom == NULL){
		sprintf(LogText,"%d",Node1Id);
        LOG_ERROR_Print(ERROR_LINKING_1,LogText);
		return 0;
	}
	if(NodeTo == NULL){
		sprintf(LogText,"%d",Node2Id);
        LOG_ERROR_Print(ERROR_LINKING_1,LogText);
		return 0;
	}
	int i = 0;
	uint32_t AdjacentNodesNumber = 0;
	while(NodeFrom->AdjacentNodes[i]!=NULL){
		if(NodeFrom->AdjacentNodes[i]->DestinationNodeId == NodeTo->ID){
			sprintf(LogText,"%d linking with %d",NodeFrom->ID,NodeTo->ID);
            LOG_ERROR_Print(ERROR_LINKING_0,LogText);
			return 0;
		}
		AdjacentNodesNumber++;
		i++;
	}
	connection_t* TempConnection = LINK_CreateConnection(NodeTo->ID, NodeFrom->ID,LinkID);
	if(TempConnection == NULL){
		return 1;
	}
	NodeFrom->AdjacentNodes = realloc(NodeFrom->AdjacentNodes, (AdjacentNodesNumber+2)*sizeof(connection_t*));
	NodeFrom->AdjacentNodes[AdjacentNodesNumber]=TempConnection;
	NodeFrom->AdjacentNodes[AdjacentNodesNumber+1]=NULL;
	sprintf(LogText, "%d and %d via Link %d", NodeFrom->ID, NodeTo->ID, LinkID);
	Print_ProcessLog(LOG_LINKED_NODE, LogText);
	return 0;
}
void    		NODE_MakeProducerNode(uint32_t NodeId, uint32_t Rate, Boolean Periodic,uint32_t Size, uint32_t* PathLine,uint32_t DestinationId,Boolean RelativeFlag,uint32_t ProtocolID){
	node_t* NodePrt = NODE_GetById(NodeId);
	NodePrt->Type = NODE_TYPE_PRODUCER;
	data_path_t* CreatedPath 	= DATA_Create_Path(DestinationId, 	PathLine);
	data_t* CreatedData 		= DATA_Create(Size,	CreatedPath, 	ProtocolID);
	job_t* CreatedJob 		= JOB_Create(NodePrt, CreatedData, Periodic, Rate, JOB_TYPE_CREATE);
    double CurrentTime 		= TBASE_GetTime();
    if(RelativeFlag == False){
    	event_t* CreatedEvent 	= TBASE_CreateEvent(CreatedJob, CurrentTime,0);
    	TBASE_AddEvent(CreatedEvent);
    }
    else{
    	event_t* CreatedEvent 	= TBASE_CreateEvent(CreatedJob, CurrentTime+Rate,0);
    	TBASE_AddEvent(CreatedEvent);
    }
}
void 			NODE_ReceiveData(node_t* NodePtr, data_t* DataPtr){
	//Remember to data on which node it is processing
	DataPtr->Path->CurrentID = NodePtr->ID;
	double CurrentTime 	= TBASE_GetTime();
	if(DataPtr->State == DATA_STATE_UNITIALIZED){
		DataPtr->State = DATA_STATE_CREATED;
		if(DataPtr->Type == DATA_TYPE_REQUEST)DataPtr->CreatedTime = TBASE_GetTime();
	}
	if(NodePtr->Processing == True){
		prvNODE_PutDataToReceiveWaitingList(NodePtr, DataPtr);
        Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);
	}
	else{
   	    double ProcessOverheadTime = 0;
		NodePtr->ProcessingData = DataPtr;
   	    connection_t* NextLink = NODE_GetNextLink(NodePtr);
	    NodePtr->ProcessedOverheadBytesCount+= DataPtr->Overhead;
		if(DataPtr->State != DATA_STATE_CREATED){
				DataPtr->State = DATA_STATE_RECEIVED;
	    }
		if(NextLink != 0){
   	    	int NumberOfPackets = ceil((((double)DataPtr->Size)/((double)NodePtr->AggregationLevel))/((double)NextLink->AssignedLink->MTUSize));
   	    	DataPtr->Overhead = NumberOfPackets*DataPtr->AssignedProtocol->Overhead;
   	    	ProcessOverheadTime = NumberOfPackets*NodePtr->MTUProcessOverhead;
   	}
        NodePtr->ProcessedDataBytesCount+= DataPtr->Size;
   	    double TimeToProcessArrivedData = (double)DataPtr->BytesToProcess/NodePtr->ProcessTime;
		double TimeToProcessPackets 	= ProcessOverheadTime;
		double ProcessTime 				= TimeToProcessArrivedData + TimeToProcessPackets;
		DataPtr->EnergyToProcessData	+= ProcessTime*NodePtr->activeConsumption;

		Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);

		NodePtr->Processing     		= True;
		job_t* CreatedJob  				= JOB_Create(NodePtr, DataPtr, True, CurrentTime+ProcessTime, JOB_TYPE_PROCESS);
		event_t* CreatedEvent 			= TBASE_CreateEvent(CreatedJob, CurrentTime+ProcessTime, 1);

		TBASE_AddEvent(CreatedEvent);
	}
}
void 			NODE_ProcessData(node_t* NodePtr){
	if(NodePtr->ProcessingData == NULL){
	 while(1);
	}
	double CurrentTime = TBASE_GetTime();
	NodePtr->ProcessedOverheadBytesCount+= NodePtr->ProcessingData->Overhead;
	NodePtr->ProcessingData->State = DATA_STATE_PROCESSED;
	if(NodePtr->operationalMode == NODE_OPMODE_LOWPOWER){
		NodePtr->FullConsumption+= (CurrentTime - NodePtr->lpEnterTime)*NodePtr->lpConsumption;
		Print_NodeLog(NodePtr, NULL, CurrentTime, 1, 0);
		NodePtr->operationalMode = NODE_OPMODE_FULLOPERATONAL;
	}
	if(NodePtr->ProcessingData->Path->DestinationID == NodePtr->ProcessingData->Path->CurrentID){
		 NodePtr->ProcessingData->State = DATA_STATE_CONSUMED;
		 if(NodePtr->ProcessingData->Type == DATA_TYPE_REQUEST){
			 NodePtr->ProcessingData->ElapsedRequestTime = TBASE_GetTime() - NodePtr->ProcessingData->CreatedTime;
		 }
		 if(DATA_CreateResponse(NodePtr->ProcessingData,NodePtr->DataBufferSize)!=0){
			 NodePtr->ProcessingData->ElapsedResponseTime = TBASE_GetTime() - NodePtr->ProcessingData->CreatedTime;
			 Print_NodeLog(NodePtr,NodePtr->ProcessingData, CurrentTime, 0, 0);
			 free( NodePtr->ProcessingData);
			 NodePtr->ProcessingData = NULL;
		 }
		 else{
			job_t* CreatedJob = JOB_Create(NodePtr, NodePtr->ProcessingData,False,CurrentTime,JOB_TYPE_RECEIVE);
			event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,0);
			TBASE_AddEvent(CreatedEvent);
		 }
		 NodePtr->Processing = False;
		 data_t* NextDataToProcess = prvNODE_GetDataFromReceiveWaitingList(NodePtr);
		 if(NextDataToProcess!=NULL){
			 job_t* CreatedJob = JOB_Create(NodePtr, NextDataToProcess,False,CurrentTime,JOB_TYPE_RECEIVE);
			 event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,0);
			 TBASE_AddEvent(CreatedEvent);
		 }
		else{
			//go to low power mode if there is no more data to process
			NodePtr->operationalMode = NODE_OPMODE_LOWPOWER;
			NodePtr->lpEnterTime	=	CurrentTime;
			Print_NodeLog(NodePtr,NULL, CurrentTime, 1, 1);
		}
	}
	else{
		NodePtr->FullConsumption += NodePtr->ProcessingData->EnergyToProcessData;
		Print_NodeLog(NodePtr,NodePtr->ProcessingData, CurrentTime, 0, 0);

		connection_t* NextLink 					= NODE_GetNextLink(NodePtr);
		node_t* NextNodePtr 						= NODE_GetById(NextLink->DestinationNodeId);
		NodePtr->ProcessingData->Size 			= NodePtr->ProcessingData->Size/NodePtr->AggregationLevel;
		NodePtr->ProcessingData->BytesToProcess =  NodePtr->ProcessingData->Size+NodePtr->ProcessingData->Overhead;
		double TransferDuration = (NextLink->AssignedLink->NumberOfHops+1)*NodePtr->ProcessingData->BytesToProcess/LINK_GetSpeed(NextLink);
		NodePtr->ProcessingData->EnergyToTransmitData 	= TransferDuration * NextLink->AssignedLink->TransmitConsumption;
		NodePtr->ProcessingData->EnergyToReceiveData 	= TransferDuration * NextLink->AssignedLink->ReceiveConsumption;
		job_t* JobSentCreated = JOB_Create(NodePtr, NodePtr->ProcessingData,False,CurrentTime+TransferDuration,JOB_TYPE_SEND);
		job_t* JobReceiveCreated = JOB_Create(NextNodePtr, NodePtr->ProcessingData,False,CurrentTime+TransferDuration,JOB_TYPE_RECEIVE);
		event_t* SendEvent = TBASE_CreateEvent(JobSentCreated,CurrentTime+TransferDuration,1);
		event_t* ReceiveEvent = TBASE_CreateEvent(JobReceiveCreated,CurrentTime+TransferDuration,0);
		TBASE_AddEvent(SendEvent);
		TBASE_AddEvent(ReceiveEvent);
	}

}
void 			NODE_SendData(node_t* NodePtr){
	if( NodePtr->ProcessingData == NULL){
	    	 while(1);
	}
	if( NodePtr->ProcessingData->State == DATA_STATE_PROCESSED){
		double CurrentTime = TBASE_GetTime();
		data_t* TempDataPtr = NodePtr->ProcessingData;
		NodePtr->ProcessingData->State = DATA_STATE_SENT;
		NodePtr->ProcessingData = NULL;
		NodePtr->Processing = False;
		Print_NodeLog(NodePtr,TempDataPtr, CurrentTime, 0, 0);
		//check is there more data to process
		data_t* NextDataToProcess = prvNODE_GetDataFromReceiveWaitingList(NodePtr);
		if(NextDataToProcess!=NULL){
			job_t* 	CreatedJob 		= JOB_Create(NodePtr, NextDataToProcess,False,CurrentTime,JOB_TYPE_RECEIVE);
			event_t* 	CreatedEvent 	= TBASE_CreateEvent(CreatedJob, CurrentTime,0);
			TBASE_AddEvent(CreatedEvent);
		}
		else{
			//go to low power mode if there is no more data to process
			NodePtr->operationalMode = NODE_OPMODE_LOWPOWER;
			NodePtr->lpEnterTime	=	CurrentTime;
			Print_NodeLog(NodePtr,TempDataPtr, CurrentTime, 1, 1);
		}
	}
}
connection_t*   	NODE_GetNextLink(node_t* NodePtr){
	if(NodePtr->ProcessingData == NULL || NodePtr->ProcessingData->State==DATA_STATE_CONSUMED){
		return NULL;
	}
	uint32_t Counter = 0;
    uint32_t NextNodeId = 0;
    do{
    	if(NodePtr->ProcessingData->Path->Line[Counter]==NodePtr->ProcessingData->Path->CurrentID){
    		NextNodeId = NodePtr->ProcessingData->Path->Line[Counter+1];
    		break;
    	}
    	Counter++;
    }while(NodePtr->ProcessingData->Path->Line[Counter-1]!=NodePtr->ProcessingData->Path->DestinationID);
    Counter = 0;
    while(NodePtr->AdjacentNodes[Counter]!=NULL){
    	if(NodePtr->AdjacentNodes[Counter]->DestinationNodeId == NextNodeId){
    		return NodePtr->AdjacentNodes[Counter];
    	}
    	Counter++;
    }
    return NULL;
}






