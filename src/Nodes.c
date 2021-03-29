/*
 * Nodes.c
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */
#include <math.h>
#include <stdio.h>

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
	if(NodePtr->receivedDataBufferSize != 0){
		DataToSend = NodePtr->receivedDataBuffer[0];
		for(Counter = 0; Counter < NodePtr->receivedDataBufferSize - 1 ;Counter++){
			NodePtr->receivedDataBuffer[Counter] = NodePtr->receivedDataBuffer[Counter+1];
		}
		NodePtr->receivedDataBufferSize--;
		NodePtr->SizeOfWaitData			-=DataToSend->BytesToProcess;
		NodePtr->receivedDataBuffer		= realloc(NodePtr->receivedDataBuffer,NodePtr->receivedDataBufferSize*sizeof(data_t*));
	}
	return DataToSend;
}
void  			prvNODE_PutDataToReceiveWaitingList(node_t* NodePtr, data_t* DataPtr){
	NodePtr->receivedDataBufferSize++;
	NodePtr->SizeOfWaitData+=DataPtr->BytesToProcess;
	NodePtr->receivedDataBuffer = realloc(NodePtr->receivedDataBuffer,NodePtr->receivedDataBufferSize*sizeof(data_t));
	NodePtr->receivedDataBuffer[NodePtr->receivedDataBufferSize-1] = DataPtr;
	DataPtr->State = DATA_STATE_WAIT;
}
//return NULL - no data to send
//return data_t* - data to send
data_t*	prvNODE_GetDataToSend(node_t* NodePtr){
	if(NodePtr->agregationLevel > 0){
		if(NodePtr->processedDataBufferSize != 0){
			uint32_t	counter = 0;
			while(counter < NodePtr->processedDataBufferSize){
				if((NodePtr->processedDataBuffer[counter]->aggregationFlag == DATA_AGG_FULL) &&
						(NodePtr->processedDataBuffer[counter]->aggreationList->numberOfData == NodePtr->agregationLevel)){
					return  NodePtr->processedDataBuffer[counter];
				}
				counter++;
			}
			return NULL;
		}
	}
	else{
		if(NodePtr->processedDataBufferSize != 0){
			return NodePtr->processedDataBuffer[NodePtr->processedDataBufferSize - 1];
		}
		return NULL;
	}
}
data_t*			prvNODE_FindDataWithSameNextNode(node_t* NodePtr, data_t*	DataPtr){
	if((NodePtr == 0) || (DataPtr == 0)) return NULL;
	if(NodePtr->processedDataBufferSize == 0) return NULL;
	uint32_t counter = 0;
	while(counter < NodePtr->processedDataBufferSize){
		if(	//check that data are on the same node
			(NodePtr->processedDataBuffer[counter]->Path->currentID == DataPtr->Path->currentID) &&
			//check that data have same next node
			(NodePtr->processedDataBuffer[counter]->Path->line[NodePtr->processedDataBuffer[counter]->Path->currentIDPossition +1]
																   == DataPtr->Path->line[DataPtr->Path->currentIDPossition+1]) &&
				(NodePtr->processedDataBuffer[counter]->AssignedProtocol->ID == DataPtr->AssignedProtocol->ID))
			return NodePtr->processedDataBuffer[counter];
		counter++;
	}
	return NULL;
}
//
node_status_t	prvNODE_ChangeDataInProcessedList(node_t* NodePtr, data_t* DataToChange, data_t* NewData){
	if(NodePtr == NULL || DataToChange == NULL || NewData == NULL) return NODE_ERROR;
	if(NodePtr->processedDataBufferSize == 0) return NODE_ERROR;
	uint32_t counter = 0;
	while(counter < NodePtr->processedDataBufferSize){
		if(NodePtr->processedDataBuffer[counter]->ID == DataToChange->ID){
			NodePtr->processedDataBuffer[counter] = NewData;
			return NODE_OK;
		}
		counter++;
	}
	return NODE_ERROR;
}
//return 1 - processing list is full
//return 0 - data is successfully written to processing list
node_status_t	prvNODE_PutDataToProcessedList(node_t* NodePtr, data_t* DataPtr){
	uint8_t	addNewDataFlag = 0;
	data_t* dataToAdd = NULL;
	data_t*	tmpData	= NULL;
	if(NodePtr->agregationLevel == 0 || NodePtr->processedDataBufferSize == 0){
		//if there is no aggregation just add data to this list
		dataToAdd = DataPtr;
	}
	else{
		//try to find two date with same destinatio node and same protocol
		tmpData = prvNODE_FindDataWithSameNextNode(NodePtr, DataPtr);
		if(tmpData != NULL){
			if(tmpData->aggregationFlag == DATA_AGG_RAW){
				//create aggregation data
				uint32_t* path = malloc(2*sizeof(uint32_t));
				path[0] = DataPtr->Path->line[DataPtr->Path->currentIDPossition];
				path[1] = DataPtr->Path->line[DataPtr->Path->currentIDPossition + 1];
				data_path_t* dataPath 	= DATA_PATH_Create(path[1], path);
				data_t*		 data		= DATA_AGG_Create(dataPath, DataPtr->AssignedProtocol->ID, TBASE_GetTime());
				if(data == NULL) return NODE_ERROR;
				//aggregate data
				if( DATA_AGG_AddData(data, tmpData) != DATA_OK) return NODE_ERROR;
				if( DATA_AGG_AddData(data, DataPtr) != DATA_OK) return NODE_ERROR;
				if(prvNODE_ChangeDataInProcessedList(NodePtr, tmpData, data) != NODE_OK) return NODE_ERROR;
			}
			else{
				if( DATA_AGG_AddData(tmpData, DataPtr) != DATA_OK) return NODE_ERROR;
			}
		}
		else{
			dataToAdd = DataPtr;
		}
	}
	if( dataToAdd != NULL){
		NodePtr->processedDataBufferSize++;
		NodePtr->ProcessedDataBytesCount	+= dataToAdd->Size;
		NodePtr->processedDataBuffer 		 = realloc(NodePtr->processedDataBuffer, NodePtr->processedDataBufferSize*sizeof(data_t));
		NodePtr->processedDataBuffer[NodePtr->processedDataBufferSize-1] = dataToAdd;
		NodePtr->ProcessedOverheadBytesCount+= DataPtr->Overhead;
		NodePtr->processingData->State 		 = DATA_STATE_PROCESS_END;
		NodePtr->processedDataCount++;
	}
    return NODE_OK;
}
//return 0 - data is found and it is successfully removed from list
//return 1 - data is not found in list
node_status_t	prvNODE_RemoveDataFromProcessedList(node_t* NodePtr, data_t* DataPtr){
	//find data
	uint32_t counter = 0;
	if(NodePtr->processedDataBufferSize == 0) return NODE_ERROR;
	while(NodePtr->processedDataBuffer[counter]->ID !=  DataPtr->ID){
		counter++;
		//there is no data in the list
		if(counter == NodePtr->processedDataBufferSize) return NODE_ERROR;
	}
	//remove data from processed list
	//shift by 1
	for(; counter < (NodePtr->processedDataBufferSize - 1); counter++){
		NodePtr->processedDataBuffer[counter] = NodePtr->processedDataBuffer[counter + 1];
	}
	NodePtr->processedDataBufferSize--;
	if(NodePtr->processedDataBufferSize > 0)
		realloc(NodePtr->processedDataBuffer, NodePtr->processedDataBufferSize*sizeof(data_t));
	return NODE_OK;
}

void 			NODE_Init(){
	prvNODE_LIST 		= malloc(sizeof(node_t*));
	prvNODE_LIST[0] 	= NULL;
	prvNODE_COUNTER		= 0;
	prvNODE_ID			= malloc(1*sizeof(uint32_t));
}
node_t*			NODE_Create(uint32_t NodeId, double ProcessTime, uint32_t CompressionLevel,uint32_t ReturnSize,uint32_t MTUProcessOverhead, double lpConsumption, double activeConsumption, uint32_t aggreationLevel){
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
	prvNODE_LIST 	= realloc(prvNODE_LIST,(prvNODE_COUNTER+1)*sizeof(node_t*));
	prvNODE_ID 		= realloc(prvNODE_ID,(prvNODE_COUNTER+1)*sizeof(uint32_t));
	prvNODE_ID[prvNODE_COUNTER-1] 								= NodeId;
	prvNODE_LIST[prvNODE_COUNTER-1] 							= malloc(sizeof(node_t));
	prvNODE_LIST[prvNODE_COUNTER-1]->ID 						= NodeId;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessTime 				= ProcessTime;
	prvNODE_LIST[prvNODE_COUNTER-1]->Processing 				= False;
	prvNODE_LIST[prvNODE_COUNTER-1]->lpConsumption 				= lpConsumption;
	prvNODE_LIST[prvNODE_COUNTER-1]->activeConsumption 			= activeConsumption;
	prvNODE_LIST[prvNODE_COUNTER-1]->FullConsumption 			= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->processingData 			= NULL;
	prvNODE_LIST[prvNODE_COUNTER-1]->receivedDataBuffer 		= malloc(sizeof(data_t*));
	prvNODE_LIST[prvNODE_COUNTER-1]->receivedDataBufferSize 	= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->SizeOfWaitData 			= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->lpEnterTime 				= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->MTUProcessOverhead 		= MTUProcessOverhead;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessedDataBytesCount 	= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->compressionLevel 			= CompressionLevel;
	prvNODE_LIST[prvNODE_COUNTER-1]->processedDataCount 		= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->processedDataBufferSize 	= 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->currentConsumption 		= lpConsumption;
	prvNODE_LIST[prvNODE_COUNTER-1]->processedDataBuffer		= malloc(sizeof(data_t*));
	prvNODE_LIST[prvNODE_COUNTER-1]->agregationLevel 			= aggreationLevel;
	prvNODE_LIST[prvNODE_COUNTER-1]->ProcessedOverheadBytesCount = 0;
	prvNODE_LIST[prvNODE_COUNTER-1]->Type 						= NODE_TYPE_CONSUMER;
	prvNODE_LIST[prvNODE_COUNTER-1]->operationalMode 			= NODE_OPMODE_LOWPOWER;
	prvNODE_LIST[prvNODE_COUNTER-1]->DataBufferSize 			= ReturnSize;
	prvNODE_LIST[prvNODE_COUNTER-1]->AdjacentNodes 				= malloc(sizeof(link_t*));
	prvNODE_LIST[prvNODE_COUNTER-1]->AdjacentNodes[0] 			= NULL;
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
	data_path_t* CreatedPath 	= DATA_PATH_Create(DestinationId, 	PathLine);
	data_t* CreatedData 		= DATA_RAW_Create(Size,	CreatedPath, 	ProtocolID);
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

node_status_t 			NODE_StartReceiveData(node_t* NodePtr, data_t* DataPtr){
	double CurrentTime 	= TBASE_GetTime();
	//wake up from LP mode
	if(NODE_GetOperationalMode(NodePtr) != NODE_OPMODE_FULLOPERATONAL){
		if(NODE_WakeFromLPMode(NodePtr, CurrentTime) != NODE_OK){
			printf("NODE: Error during wake-up event on node %d\r\n", NodePtr->ID);
			return NODE_ERROR;
		}
	}
	NodePtr->currentConsumption += DataPtr->linkReceiveConsumption;
	DataPtr->State				= DATA_STATE_RECEIVE_START;
	Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);
	return NODE_OK;
}
node_status_t 			NODE_StartTransmitData(node_t* NodePtr){
	double CurrentTime 	= TBASE_GetTime();
	NodePtr->currentConsumption += NodePtr->processingData->linkTransmitConsumption;
	NodePtr->processingData->State = DATA_STATE_TRANSMIT_START;
	Print_NodeLog(NodePtr, NodePtr->processingData, CurrentTime, 0, 0);
	return NODE_OK;
}
node_status_t 			NODE_ReceiveData(node_t* NodePtr, data_t* DataPtr){
	//Remember to data on which node it is processing
	double CurrentTime 	= TBASE_GetTime();
	if(DATA_SetNode(DataPtr, NodePtr->ID) != DATA_OK){
		printf("Node with id %d is not on data %d path\r\n", NodePtr->ID, DataPtr->ID);
		return NODE_ERROR;
	}
	if(DataPtr->State == DATA_STATE_UNITIALIZED){
		//wake up from LP mode
		if(NODE_GetOperationalMode(NodePtr) != NODE_OPMODE_FULLOPERATONAL){
			if(NODE_WakeFromLPMode(NodePtr, CurrentTime) != NODE_OK){
				printf("NODE: Error during wake-up event on node %d\r\n", NodePtr->ID);
				return NODE_ERROR;
			}
		}
		DataPtr->State = DATA_STATE_CREATED;
		if(DataPtr->Type == DATA_TYPE_REQUEST)DataPtr->CreatedTime = TBASE_GetTime();
	}
	NodePtr->currentConsumption -= DataPtr->linkReceiveConsumption;
	DataPtr->linkReceiveConsumption = 0;
	NodePtr->FullConsumption	+= DataPtr->EnergyToReceiveData;
	DataPtr->EnergyToReceiveData = 0;
	if(NodePtr->Processing == True){
		prvNODE_PutDataToReceiveWaitingList(NodePtr, DataPtr);
        Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);
		return NODE_OK;
	}
	else{
   	    double ProcessOverheadTime = 0;
		NodePtr->processingData = DataPtr;
   	    //connection_t* NextLink = NODE_GetNextLink(NodePtr);
	    NodePtr->ProcessedOverheadBytesCount+= DataPtr->Overhead;
		if(DataPtr->State != DATA_STATE_CREATED){
				DataPtr->State = DATA_STATE_RECEIVE_END;
	    }
//		if(NextLink != 0){
//   	    	int NumberOfPackets = ceil((((double)DataPtr->Size)/((double)NodePtr->compressionLevel))/((double)NextLink->AssignedLink->MTUSize));
//   	    	DataPtr->Overhead = NumberOfPackets*DataPtr->AssignedProtocol->Overhead;
//   	    	ProcessOverheadTime = NumberOfPackets*NodePtr->MTUProcessOverhead;
//		}
		double ProcessTime = 0;
   	    double TimeToProcessArrivedData =0;
   	    if(DataPtr -> aggregationFlag == DATA_AGG_RAW){
   	    	TimeToProcessArrivedData = (double)DataPtr->BytesToProcess/NodePtr->ProcessTime;
   	    }
   	    else{
   	    	TimeToProcessArrivedData = (double)DataPtr->Overhead/NodePtr->MTUProcessOverhead;
   	    }
//		double TimeToProcessPackets 	= ProcessOverheadTime;
		ProcessTime 					= TimeToProcessArrivedData;// + TimeToProcessPackets;
		DataPtr->EnergyToProcessData	= ProcessTime*NodePtr->activeConsumption;

		Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);

		NodePtr->Processing     		= True;
		job_t* 	 StartProcessJob  		= JOB_Create(NodePtr, DataPtr, True, CurrentTime, JOB_TYPE_PROCESS_DATA_START);
		event_t* StartProcessEvent		= TBASE_CreateEvent(StartProcessJob, CurrentTime, 1);
		job_t* 	 EndProcessJob	    	= JOB_Create(NodePtr, DataPtr, True, CurrentTime+ProcessTime, JOB_TYPE_PROCESS_DATA_END);
		event_t* EndProcessEvent 		= TBASE_CreateEvent(EndProcessJob, CurrentTime+ProcessTime, 1);

		TBASE_AddEvent(StartProcessEvent);
		TBASE_AddEvent(EndProcessEvent);
		return NODE_OK;
	}
	return NODE_ERROR;
}
node_status_t 			NODE_StartProcessData(node_t* NodePtr){
	if(NodePtr->processingData == NULL){
		puts("NODE: There is no assigned data for processing");
		return NODE_ERROR;;
	}
	double CurrentTime = TBASE_GetTime();
	NodePtr->processingData->State = DATA_STATE_PROCESS_START;
	Print_NodeLog(NodePtr, NodePtr->processingData, CurrentTime, 0, 0);
	return NODE_OK;
}

node_status_t 			NODE_StartProcessMTUData(node_t* NodePtr){
	if(NodePtr->processingData == NULL){
		puts("NODE: There is no assigned data for processing");
		return NODE_ERROR;;
	}
	double CurrentTime = TBASE_GetTime();
	NodePtr->processingData->State = DATA_STATE_PROCESS_START;
	NodePtr->processingData->overheadProccesFlag = 1;
	Print_NodeLog(NodePtr, NodePtr->processingData, CurrentTime, 0, 0);
	return NODE_OK;
}
node_status_t 			NODE_ProcessData(node_t* NodePtr){
	if(NodePtr->processingData == NULL){
		puts("NODE: There is no assigned data for processing");
		return NODE_ERROR;;
	}
	double CurrentTime = TBASE_GetTime();
	NodePtr->FullConsumption += NodePtr->processingData->EnergyToProcessData;
	NodePtr->processingData->State = DATA_STATE_PROCESS_END;
	//check is this node destination node for data
	if(NodePtr->processingData->Path->destinationID == NodePtr->processingData->Path->currentID){
		 NodePtr->processingData->State = DATA_STATE_CONSUMED;
		 if(NodePtr->processingData->Type == DATA_TYPE_REQUEST){
			 Print_NodeLog(NodePtr,NodePtr->processingData, CurrentTime, 0, 0);
			 NodePtr->processingData->ElapsedRequestTime = TBASE_GetTime() - NodePtr->processingData->CreatedTime;
			 if(NodePtr->processingData->aggregationFlag == DATA_AGG_FULL){
				 uint32_t counter = 0;
				 while(counter < NodePtr->processingData->aggreationList->numberOfData){
					job_t* CreatedJob = JOB_Create(NodePtr, NodePtr->processingData->aggreationList->dataList[counter] ,False,CurrentTime,JOB_TYPE_RECEIVE);
					event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,0);
					TBASE_AddEvent(CreatedEvent);
					counter++;
				 }
			 }
			 if(DATA_CreateResponse(NodePtr->processingData,NodePtr->DataBufferSize,TBASE_GetTime()) != 0){
				 NodePtr->processingData->ElapsedResponseTime = TBASE_GetTime() - NodePtr->processingData->CreatedTime;
				 Print_NodeLog(NodePtr,NodePtr->processingData, CurrentTime, 0, 0);
				 free( NodePtr->processingData);
				 NodePtr->processingData = NULL;
			 }
			 else{
				job_t* CreatedJob = JOB_Create(NodePtr, NodePtr->processingData,False,CurrentTime,JOB_TYPE_RECEIVE);
				event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,2);
				TBASE_AddEvent(CreatedEvent);
			 }

		 }
		 else{
			 NodePtr->processingData->ElapsedResponseTime = TBASE_GetTime() - NodePtr->processingData->ElapsedResponseTime;
			 Print_NodeLog(NodePtr,NodePtr->processingData, CurrentTime, 0, 0);
 			data_t* NextDataToProcess = prvNODE_GetDataFromReceiveWaitingList(NodePtr);
 			if(NextDataToProcess!=NULL){
 				 job_t* CreatedJob = JOB_Create(NodePtr, NextDataToProcess,False,CurrentTime,JOB_TYPE_RECEIVE);
 				 event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,0);
 				 TBASE_AddEvent(CreatedEvent);
 			}
 			else{
 				//go to low power mode if there is no more data to process
 				if(NODE_GoToLPMode(NodePtr, CurrentTime) != NODE_OK){
 					printf("NODE: Error during lp mode on node %d\r\n", NodePtr->ID);
 					return NODE_ERROR;
 				}
 			}
		 }
		 NodePtr->Processing = False;
		return NODE_OK;
	}
	else{
		//this node is not destination node for data
		//put received data to processed data list
		if(prvNODE_PutDataToProcessedList(NodePtr, NodePtr->processingData) != NODE_OK){
			puts("NODE: Processing list is full");
			return NODE_ERROR;
		}
		Print_NodeLog(NodePtr,NodePtr->processingData, CurrentTime, 0, 0);
		//as first, check if there is data ready to be sent from node
		NodePtr->processingData = NULL;
		NodePtr->processingData = prvNODE_GetDataToSend(NodePtr);
		if(NodePtr->processingData != NULL){
//			if(NodePtr->processingData->aggregationFlag == DATA_AGG_FULL){
				//if AGG data is prepare calculate how much time it is required to process MTU packets
		  	    connection_t* NextLink = NODE_GetNextLink(NodePtr);
		  	    if(NextLink != 0){
					int NumberOfPackets = ceil((((double)NodePtr->processingData->Size)/((double)NodePtr->compressionLevel))/((double)NextLink->AssignedLink->MTUSize));
					NodePtr->processingData->Overhead = NumberOfPackets*NodePtr->processingData->AssignedProtocol->Overhead;
					NodePtr->processingData->BytesToProcess =  NodePtr->processingData->Overhead;
					double ProcessOverheadTime = (double)NodePtr->processingData->BytesToProcess/NodePtr->MTUProcessOverhead;
					NodePtr->processingData->EnergyToProcessData = ProcessOverheadTime*NodePtr->activeConsumption;
					NodePtr->Processing     		= True;
					job_t* CreatedJob1  			= JOB_Create(NodePtr, NodePtr->processingData, True, CurrentTime, JOB_TYPE_PROCESS_MTU_START);
					event_t* CreatedEvent1 			= TBASE_CreateEvent(CreatedJob1, CurrentTime, 1);
					job_t* CreatedJob  				= JOB_Create(NodePtr, NodePtr->processingData, True, CurrentTime+ProcessOverheadTime, JOB_TYPE_PROCESS_MTU_END);
					event_t* CreatedEvent 			= TBASE_CreateEvent(CreatedJob, CurrentTime+ProcessOverheadTime, 1);

					TBASE_AddEvent(CreatedEvent1);
					TBASE_AddEvent(CreatedEvent);
		  	    }
		  	    else{
		  	    	printf("NODE: There is no next link on node %d\r\n", NodePtr->ID);
		  	    	return NODE_ERROR;
		  	    }

/*			}
			else{
				//if proccessing of RAW data is done, send it
				connection_t* NextLink 					= NODE_GetNextLink(NodePtr);
				node_t* NextNodePtr 					= NODE_GetById(NextLink->DestinationNodeId);
				NodePtr->processingData->Size 			= NodePtr->processingData->Size/NodePtr->compressionLevel;
				NodePtr->processingData->BytesToProcess =  NodePtr->processingData->Size+NodePtr->processingData->Overhead;
				double TransferDuration = (NextLink->AssignedLink->NumberOfHops+1)*NodePtr->processingData->BytesToProcess/LINK_GetSpeed(NextLink);
				NodePtr->processingData->EnergyToTransmitData 	= TransferDuration * NextLink->AssignedLink->TransmitConsumption;
				NodePtr->processingData->EnergyToReceiveData 	= TransferDuration * NextLink->AssignedLink->ReceiveConsumption;
				job_t* JobSentCreated = JOB_Create(NodePtr, NodePtr->processingData,False,CurrentTime+TransferDuration,JOB_TYPE_SEND);
				job_t* JobReceiveCreated = JOB_Create(NextNodePtr, NodePtr->processingData,False,CurrentTime+TransferDuration,JOB_TYPE_RECEIVE);
				event_t* SendEvent = TBASE_CreateEvent(JobSentCreated,CurrentTime+TransferDuration,1);
				event_t* ReceiveEvent = TBASE_CreateEvent(JobReceiveCreated,CurrentTime+TransferDuration,0);
				TBASE_AddEvent(SendEvent);
				TBASE_AddEvent(ReceiveEvent);
		  	    connection_t* NextLink = NODE_GetNextLink(NodePtr);
				int NumberOfPackets = ceil((((double)NodePtr->processingData->Size)/((double)NodePtr->compressionLevel))/((double)NextLink->AssignedLink->MTUSize));
				NodePtr->processingData->Overhead = NumberOfPackets*NodePtr->processingData->AssignedProtocol->Overhead;
				NodePtr->processingData->BytesToProcess =  NodePtr->processingData->Overhead;
				double ProcessOverheadTime = NumberOfPackets*NodePtr->MTUProcessOverhead;
				NodePtr->processingData->EnergyToProcessData = ProcessOverheadTime*NodePtr->activeConsumption;
				NodePtr->Processing     		= True;
				job_t* CreatedJob  				= JOB_Create(NodePtr, NodePtr->processingData, True, CurrentTime+ProcessOverheadTime, JOB_TYPE_PROCESS_MTU);
				event_t* CreatedEvent 			= TBASE_CreateEvent(CreatedJob, CurrentTime+ProcessOverheadTime, 1);
				TBASE_AddEvent(CreatedEvent);

		}*/
		}
		else{
			data_t* NextDataToProcess = prvNODE_GetDataFromReceiveWaitingList(NodePtr);
			if(NextDataToProcess!=NULL){
				 job_t* CreatedJob = JOB_Create(NodePtr, NextDataToProcess,False,CurrentTime,JOB_TYPE_RECEIVE);
				 event_t* CreatedEvent = TBASE_CreateEvent(CreatedJob, CurrentTime,0);
				 TBASE_AddEvent(CreatedEvent);
			}
			else{
				//go to low power mode if there is no more data to process
				NodePtr->currentConsumption	   -= NodePtr->activeConsumption;
				if(NODE_GoToLPMode(NodePtr, CurrentTime) != NODE_OK){
					printf("NODE: Error during lp mode on node %d\r\n", NodePtr->ID);
					return NODE_ERROR;
				}
			}
			NodePtr->Processing = False;
		}
		return NODE_OK;
	}
	return NODE_ERROR;

}

node_status_t 			NODE_ProcessMTUData(node_t* NodePtr){
	double CurrentTime = TBASE_GetTime();
	if(NodePtr->processingData == NULL){
		puts("NODE: There is no assigned data for processing");
		return NODE_ERROR;;
	}
	NodePtr->FullConsumption += NodePtr->processingData->EnergyToProcessData;
	NodePtr->processingData->State = DATA_STATE_PROCESS_END;
	Print_NodeLog(NodePtr,NodePtr->processingData, CurrentTime, 0, 0);
	connection_t* NextLink 					= NODE_GetNextLink(NodePtr);
	node_t* NextNodePtr 					= NODE_GetById(NextLink->DestinationNodeId);
	NodePtr->processingData->Size 			= NodePtr->processingData->Size/NodePtr->compressionLevel;
	NodePtr->processingData->BytesToProcess =  NodePtr->processingData->Size+NodePtr->processingData->Overhead;
	double TransferDuration = (NextLink->AssignedLink->NumberOfHops+1)*NodePtr->processingData->BytesToProcess/LINK_GetSpeed(NextLink);
	NodePtr->processingData->EnergyToTransmitData 	= TransferDuration*NodePtr->activeConsumption + TransferDuration * NextLink->AssignedLink->TransmitConsumption;
	NodePtr->processingData->EnergyToReceiveData 	= TransferDuration*NodePtr->activeConsumption + TransferDuration * NextLink->AssignedLink->ReceiveConsumption;
	NodePtr->processingData->linkTransmitConsumption= NextLink->AssignedLink->TransmitConsumption;
	NodePtr->processingData->linkReceiveConsumption = NextLink->AssignedLink->ReceiveConsumption;
	NodePtr->processingData->overheadProccesFlag	= 0;
	job_t* JobStartTransmitCreated = JOB_Create(NodePtr, NodePtr->processingData,False,CurrentTime,	JOB_TYPE_TRANSMIT_START);
	job_t* JobStartReceiveCreated = JOB_Create(NextNodePtr, NodePtr->processingData,False,CurrentTime,	JOB_TYPE_RECEIVE_START);
	job_t* JobSentCreated = JOB_Create(NodePtr, NodePtr->processingData,False,CurrentTime+TransferDuration,JOB_TYPE_TRANSMIT);
	job_t* JobReceiveCreated = JOB_Create(NextNodePtr, NodePtr->processingData,False,CurrentTime+TransferDuration,JOB_TYPE_RECEIVE);
	event_t* StartTransmitEvent = TBASE_CreateEvent(JobStartTransmitCreated, CurrentTime,1);
	event_t* StartReceiveEvent = TBASE_CreateEvent(JobStartReceiveCreated,CurrentTime,0);
	event_t* SendEvent = TBASE_CreateEvent(JobSentCreated,CurrentTime+TransferDuration,1);
	event_t* ReceiveEvent = TBASE_CreateEvent(JobReceiveCreated,CurrentTime+TransferDuration,0);
	TBASE_AddEvent(StartTransmitEvent);
	TBASE_AddEvent(StartReceiveEvent);
	TBASE_AddEvent(SendEvent);
	TBASE_AddEvent(ReceiveEvent);
	return NODE_OK;

}
node_status_t 			NODE_TransmitData(node_t* NodePtr){
	if( NodePtr->processingData == NULL){
	    	 return NODE_ERROR;
	}
	if( NodePtr->processingData->State == DATA_STATE_RECEIVE_START){
		double CurrentTime = TBASE_GetTime();
		data_t* TempDataPtr = NodePtr->processingData;
		NodePtr->processingData->State = DATA_STATE_TRANSMIT_END;
		NodePtr->FullConsumption += NodePtr->processingData->EnergyToTransmitData;
		NodePtr->processingData->EnergyToTransmitData = 0;
		if(NodePtr->processingData->aggregationFlag == DATA_AGG_FULL){
			uint32_t counter = 0;
			while(counter < NodePtr->processingData->aggreationList->numberOfData){
				NodePtr->processingData->aggreationList->dataList[counter]->State = DATA_STATE_TRANSMIT_END;
				counter++;
			}
		}
		if(prvNODE_RemoveDataFromProcessedList(NodePtr, NodePtr->processingData) != NODE_OK){
			printf("NODE: There is error during removing data from processing list on node %d\r\n", NodePtr->ID);
			return NODE_OK;
		}
		NodePtr->processingData = NULL;
		NodePtr->Processing = False;
		Print_NodeLog(NodePtr,TempDataPtr, CurrentTime, 0, 0);
		NodePtr->currentConsumption -= TempDataPtr->linkReceiveConsumption;
		//check is there more data to process
		data_t* NextDataToProcess = prvNODE_GetDataFromReceiveWaitingList(NodePtr);
		if(NextDataToProcess!=NULL){
			job_t* 	CreatedJob 		= JOB_Create(NodePtr, NextDataToProcess,False,CurrentTime,JOB_TYPE_RECEIVE);
			event_t* 	CreatedEvent 	= TBASE_CreateEvent(CreatedJob, CurrentTime,0);
			TBASE_AddEvent(CreatedEvent);
			return NODE_OK;
		}
		else{
			//go to low power mode if there is no more data to process
			if(NODE_GoToLPMode(NodePtr, CurrentTime) != NODE_OK){
				printf("NODE: Error during lp mode on node %d\r\n", NodePtr->ID);
				return NODE_ERROR;
			}
			return NODE_OK;
		}
	}
	return NODE_ERROR;
}
connection_t*   	NODE_GetNextLink(node_t* NodePtr){
	if(NodePtr->processingData == NULL || NodePtr->processingData->State==DATA_STATE_CONSUMED){
		 return NULL;;
	}
	uint32_t Counter = 0;
    uint32_t NextNodeId = 0;
    do{
    	if(NodePtr->processingData->Path->line[Counter]==NodePtr->processingData->Path->currentID){
    		NextNodeId = NodePtr->processingData->Path->line[Counter+1];
    		break;
    	}
    	Counter++;
    }while(NodePtr->processingData->Path->line[Counter-1]!=NodePtr->processingData->Path->destinationID);
    Counter = 0;
    while(NodePtr->AdjacentNodes[Counter]!=NULL){
    	if(NodePtr->AdjacentNodes[Counter]->DestinationNodeId == NextNodeId){
    		return NodePtr->AdjacentNodes[Counter];
    	}
    	Counter++;
    }
    return NULL;
}

node_status_t		NODE_GoToLPMode(node_t* NodePtr, double time){
	if(NodePtr->operationalMode == NODE_OPMODE_LOWPOWER) return NODE_ERROR;
	//cant go to LP if there is no finished actions
	NodePtr->currentConsumption	   -= NodePtr->activeConsumption;
	if(	NodePtr->currentConsumption	!= NodePtr->lpConsumption){
		NodePtr->currentConsumption	   += NodePtr->activeConsumption;
		return NODE_OK;
	}
	NodePtr->operationalMode = NODE_OPMODE_LOWPOWER;
	NodePtr->lpEnterTime	 =	time;
	Print_NodeLog(NodePtr,	NULL, time, 1, 1);
	return NODE_OK;
}
node_status_t		NODE_WakeFromLPMode(node_t* NodePtr, double time){
	if(NodePtr->operationalMode == NODE_OPMODE_FULLOPERATONAL) return NODE_ERROR;
	NodePtr->FullConsumption+= (time - NodePtr->lpEnterTime)*NodePtr->lpConsumption;
	Print_NodeLog(NodePtr,	NULL, time, 1, 0);
	NodePtr->currentConsumption	   += NodePtr->activeConsumption;
	NodePtr->operationalMode = NODE_OPMODE_FULLOPERATONAL;
	return NODE_OK;
}
node_operational_mode_t	NODE_GetOperationalMode(node_t* NodePtr){
	return NodePtr->operationalMode;
}






