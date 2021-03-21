/*
 * Nodes.c
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */
#include <math.h>
#include "Job.h"
#include "TimeBase.h"
#include "Log.h"


Node** 			NodesList;
uint32_t 		NodesNumber;
uint32_t*  		NodesId;


char 			LogText[50];


Data*  			Get_NextWaitingData_Node(Node* NodePtr){
	Data* DataToSend = NULL;
	uint32_t Counter = 0;
	if(NodePtr->NoOfWaitData != 0){
		DataToSend = NodePtr->WaitData[0];
		for(Counter = 0; Counter < NodePtr->NoOfWaitData - 1 ;Counter++){
			NodePtr->WaitData[Counter] = NodePtr->WaitData[Counter+1];
		}
		NodePtr->NoOfWaitData--;
		NodePtr->SizeOfWaitData-=DataToSend->BytesToProcess;
		NodePtr->WaitData= realloc(NodePtr->WaitData,NodePtr->NoOfWaitData*sizeof(Data*));
	}
	return DataToSend;
}
void  			Add_DataToWaitingList_Node(Node* NodePtr, Data* DataPtr){
	NodePtr->NoOfWaitData++;
	NodePtr->SizeOfWaitData+=DataPtr->BytesToProcess;
	NodePtr->WaitData = realloc(NodePtr->WaitData,NodePtr->NoOfWaitData*sizeof(Data));
	NodePtr->WaitData[NodePtr->NoOfWaitData-1] = DataPtr;
	DataPtr->State = Wait;
}

void 			Init_Node(){
	NodesList 		= malloc(sizeof(Node*));
	NodesList[0] 	= NULL;
	NodesNumber		= 0;
	NodesId			= malloc(1*sizeof(uint32_t));
}
Node*			Create_Node(uint32_t NodeId, double ProcessTime, uint32_t AgregationLevel,uint32_t ReturnSize,uint32_t MTUProcessOverhead, double lpConsumption, double activeConsumption){
	if(NodeId < 1){
		sprintf(LogText, "%d", NodeId);
		Print_ErrorLog(ERROR_NODE_1, LogText);
		return NULL;
	}
	int i = 0;
	for (i =0; i< NodesNumber;i++){
		if(NodesId[i] == NodeId){
			sprintf(LogText, "%d", NodeId);
            Print_ErrorLog(ERROR_NODE_0, LogText);
			return NULL;
		}
		i++;
	}
	NodesNumber++;
	NodesList = realloc(NodesList,(NodesNumber+1)*sizeof(Node*));
	NodesId = realloc(NodesId,(NodesNumber+1)*sizeof(uint32_t));
	NodesId[NodesNumber-1] = NodeId;
	NodesList[NodesNumber-1] = malloc(sizeof(Node));
	NodesList[NodesNumber-1]->ID = NodeId;
	NodesList[NodesNumber-1]->ProcessTime = ProcessTime;
	NodesList[NodesNumber-1]->Processing = False;
	NodesList[NodesNumber-1]->lpConsumption = lpConsumption;
	NodesList[NodesNumber-1]->activeConsumption = activeConsumption;
	NodesList[NodesNumber-1]->FullConsumption = 0;
	NodesList[NodesNumber-1]->ProcessingData = NULL;
	NodesList[NodesNumber-1]->WaitData = malloc(sizeof(Data*));
	NodesList[NodesNumber-1]->NoOfWaitData = 0;
	NodesList[NodesNumber-1]->SizeOfWaitData = 0;
	NodesList[NodesNumber-1]->lpEnterTime = 0;
	NodesList[NodesNumber-1]->MTUProcessOverhead = MTUProcessOverhead;
	NodesList[NodesNumber-1]->ProcessedDataBytesCount = 0;
	NodesList[NodesNumber-1]->AggregationLevel = AgregationLevel;
	NodesList[NodesNumber-1]->ProcessedOverheadBytesCount = 0;
	NodesList[NodesNumber-1]->Type = Consumer;
	NodesList[NodesNumber-1]->operationalMode = NODE_OPMODE_LOWPOWER;
	NodesList[NodesNumber-1]->DataBufferSize = ReturnSize;
	NodesList[NodesNumber-1]->AdjacentNodes = malloc(sizeof(Link*));
	NodesList[NodesNumber-1]->AdjacentNodes[0] = NULL;
	memset(NodesList[NodesNumber-1]->LogFilename, 0, NODE_FILENAME_SIZE);
	sprintf(NodesList[NodesNumber-1]->LogFilename, "Log/Nodes/%d.txt", NodeId);
	//Try to open file for testing purpose
	NodesList[NodesNumber-1]->NodeLogFile=fopen(NodesList[NodesNumber-1]->LogFilename, "w");
	if(NodesList[NodesNumber-1]->NodeLogFile == NULL){
		Print_ErrorLog(ERROR_FILE_0,NodesList[NodesNumber-1]->LogFilename);
		return NULL;
	}
	fclose(NodesList[NodesNumber-1]->NodeLogFile);
	sprintf(LogText, "%d", NodeId);
	Print_ProcessLog(LOG_CREATED_NODE, LogText);
	NodesList[NodesNumber] = NULL;
	return NodesList[NodesNumber-1];
}
Node*   		Get_Node(uint32_t NodeId){
	int CurrentNodesList = 0;
	for(CurrentNodesList=0; CurrentNodesList<NodesNumber; CurrentNodesList++){
		if(NodesList[CurrentNodesList]->ID == NodeId){
			return NodesList[CurrentNodesList];
		}
	}
	return NULL;
}
uint8_t			Link_Node(uint32_t Node1Id, uint32_t Node2Id,uint32_t LinkID){
	Node* NodeFrom= Get_Node(Node1Id);
	Node* NodeTo= Get_Node(Node2Id);
	if(NodeFrom == NULL){
		sprintf(LogText,"%d",Node1Id);
        Print_ErrorLog(ERROR_LINKING_1,LogText);
		return 0;
	}
	if(NodeTo == NULL){
		sprintf(LogText,"%d",Node2Id);
        Print_ErrorLog(ERROR_LINKING_1,LogText);
		return 0;
	}
	int i = 0;
	uint32_t AdjacentNodesNumber = 0;
	while(NodeFrom->AdjacentNodes[i]!=NULL){
		if(NodeFrom->AdjacentNodes[i]->DestinationNodeId == NodeTo->ID){
			sprintf(LogText,"%d linking with %d",NodeFrom->ID,NodeTo->ID);
            Print_ErrorLog(ERROR_LINKING_0,LogText);
			return 0;
		}
		AdjacentNodesNumber++;
		i++;
	}
	Connection* TempConnection = Create_Connection(NodeTo->ID, NodeFrom->ID,LinkID);
	if(TempConnection == NULL){
		return 1;
	}
	NodeFrom->AdjacentNodes = realloc(NodeFrom->AdjacentNodes, (AdjacentNodesNumber+2)*sizeof(Connection*));
	NodeFrom->AdjacentNodes[AdjacentNodesNumber]=TempConnection;
	NodeFrom->AdjacentNodes[AdjacentNodesNumber+1]=NULL;
	sprintf(LogText, "%d and %d via Link %d", NodeFrom->ID, NodeTo->ID, LinkID);
	Print_ProcessLog(LOG_LINKED_NODE, LogText);
	return 0;
}
void    		Make_Producer_Node(uint32_t NodeId, uint32_t Rate, Boolean Periodic,uint32_t Size, uint32_t* PathLine,uint32_t DestinationId,Boolean RelativeFlag,uint32_t ProtocolID){
	Node* NodePrt = Get_Node(NodeId);
	NodePrt->Type = Producer;
	DataPath* CreatedPath 	= Create_Path_Data(DestinationId, 	PathLine);
	Data* CreatedData 		= Create_Data(Size,	CreatedPath, 	ProtocolID);
	Job* CreatedJob 		= Create_Job(NodePrt, CreatedData, Periodic, Rate, Create);
    double CurrentTime 		= Get_CurrentTime();
    if(RelativeFlag == False){
    	Event* CreatedEvent 	= Create_Event(CreatedJob, CurrentTime,0);
    	Add_Event(CreatedEvent);
    }
    else{
    	Event* CreatedEvent 	= Create_Event(CreatedJob, CurrentTime+Rate,0);
    	Add_Event(CreatedEvent);
    }
}
void 			Receive_DataOn_Node(Node* NodePtr, Data* DataPtr){
	//Remember to data on which node it is processing
	DataPtr->Path->CurrentID = NodePtr->ID;
	double CurrentTime 	= Get_CurrentTime();
	if(DataPtr->State == Unitialized){
		DataPtr->State = Created;
		if(DataPtr->Type == Request)DataPtr->CreatedTime = Get_CurrentTime();
	}
	if(NodePtr->Processing == True){
		Add_DataToWaitingList_Node(NodePtr, DataPtr);
        Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);
	}
	else{
   	    double ProcessOverheadTime = 0;
		NodePtr->ProcessingData = DataPtr;
   	    Connection* NextLink = Get_NextLink_Node(NodePtr);
	    NodePtr->ProcessedOverheadBytesCount+= DataPtr->Overhead;
		if(DataPtr->State != Created){
				DataPtr->State = Received;
	    }
		if(NodePtr->operationalMode == NODE_OPMODE_LOWPOWER){
			NodePtr->FullConsumption+= (CurrentTime - NodePtr->lpEnterTime)*NodePtr->lpConsumption;
	        Print_NodeLog(NodePtr, DataPtr, CurrentTime, 1, 0);
			NodePtr->operationalMode = NODE_OPMODE_FULLOPERATONAL;
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

		Print_NodeLog(NodePtr, DataPtr, CurrentTime, 0, 0);
		NodePtr->Processing     		= True;
		Job* CreatedJob  				= Create_Job(NodePtr, DataPtr, True, CurrentTime+ProcessTime, Process);
		Event* CreatedEvent 			= Create_Event(CreatedJob, CurrentTime+ProcessTime, 1);

		Add_Event(CreatedEvent);
	}
}
void 			Process_DataOn_Node(Node* NodePtr){
     if(NodePtr->ProcessingData == NULL){
    	 while(1);
     }
	 double CurrentTime = Get_CurrentTime();
	 NodePtr->ProcessedOverheadBytesCount+= NodePtr->ProcessingData->Overhead;
     NodePtr->ProcessingData->State = Processed;
     if(NodePtr->ProcessingData->Path->DestinationID == NodePtr->ProcessingData->Path->CurrentID){
    	 NodePtr->ProcessingData->State = Consumed;
    	 if(NodePtr->ProcessingData->Type == Request){
    		 NodePtr->ProcessingData->ElapsedRequestTime = Get_CurrentTime() - NodePtr->ProcessingData->CreatedTime;
    	 }
    	 if(Create_Response_Data(NodePtr->ProcessingData,NodePtr->DataBufferSize)!=0){
        	 NodePtr->ProcessingData->ElapsedResponseTime = Get_CurrentTime() - NodePtr->ProcessingData->CreatedTime;
        	 Print_NodeLog(NodePtr,NodePtr->ProcessingData, CurrentTime, 0, 0);
			 free( NodePtr->ProcessingData);
			 NodePtr->ProcessingData = NULL;
    	 }
    	 else{
    		Job* CreatedJob = Create_Job(NodePtr, NodePtr->ProcessingData,False,CurrentTime,Receive);
			Event* CreatedEvent = Create_Event(CreatedJob, CurrentTime,0);
			Add_Event(CreatedEvent);
    	 }
    	 NodePtr->Processing = False;
    	 Data* NextDataToProcess = Get_NextWaitingData_Node(NodePtr);
    	 if(NextDataToProcess!=NULL){
    		 Job* CreatedJob = Create_Job(NodePtr, NextDataToProcess,False,CurrentTime,Receive);
    		 Event* CreatedEvent = Create_Event(CreatedJob, CurrentTime,0);
    		 Add_Event(CreatedEvent);
    	 }
     }
     else{
    	 Print_NodeLog(NodePtr,NodePtr->ProcessingData, CurrentTime, 0, 0);

    	 Connection* NextLink = Get_NextLink_Node(NodePtr);
    	 Node* NextNodePtr = Get_Node(NextLink->DestinationNodeId);
    	 NodePtr->ProcessingData->Size = NodePtr->ProcessingData->Size/NodePtr->AggregationLevel;
     	 NodePtr->ProcessingData->BytesToProcess =  NodePtr->ProcessingData->Size+NodePtr->ProcessingData->Overhead;
    	 double TransferDuration = (NextLink->AssignedLink->NumberOfHops+1)*NodePtr->ProcessingData->BytesToProcess/Get_LinkSpeed(NextLink);
    	 NodePtr->ProcessingData->EnergyToTransmitData = TransferDuration * NextLink->AssignedLink->TransmitConsumption;
    	 NodePtr->ProcessingData->EnergyToReceiveData = TransferDuration * NextLink->AssignedLink->ReceiveConsumption;
    	 Job* JobSentCreated = Create_Job(NodePtr, NodePtr->ProcessingData,False,CurrentTime+TransferDuration,Send);
    	 Job* JobReceiveCreated = Create_Job(NextNodePtr, NodePtr->ProcessingData,False,CurrentTime+TransferDuration,Receive);
    	 Event* SendEvent = Create_Event(JobSentCreated,CurrentTime+TransferDuration,1);
    	 Event* ReceiveEvent = Create_Event(JobReceiveCreated,CurrentTime+TransferDuration,0);
    	 Add_Event(SendEvent);
    	 Add_Event(ReceiveEvent);
     }

}
void 			Send_DataFrom_Node(Node* NodePtr){
	if( NodePtr->ProcessingData == NULL){
	    	 while(1);
	}
	if( NodePtr->ProcessingData->State == Processed){
		double CurrentTime = Get_CurrentTime();
		Data* TempDataPtr = NodePtr->ProcessingData;
		NodePtr->ProcessingData->State = Sent;
		NodePtr->ProcessingData = NULL;
		NodePtr->Processing = False;
		Print_NodeLog(NodePtr,TempDataPtr, CurrentTime, 0, 0);
		//check is there more data to process
		Data* NextDataToProcess = Get_NextWaitingData_Node(NodePtr);
		if(NextDataToProcess!=NULL){
			Job* 	CreatedJob 		= Create_Job(NodePtr, NextDataToProcess,False,CurrentTime,Receive);
			Event* 	CreatedEvent 	= Create_Event(CreatedJob, CurrentTime,0);
			Add_Event(CreatedEvent);
		}
		else{
			//go to low power mode if there is no more data to process
			NodePtr->operationalMode = NODE_OPMODE_LOWPOWER;
			NodePtr->lpEnterTime	=	CurrentTime;
			Print_NodeLog(NodePtr,TempDataPtr, CurrentTime, 1, 1);
		}
	}
}
Connection*   	Get_NextLink_Node(Node* NodePtr){
	if(NodePtr->ProcessingData == NULL || NodePtr->ProcessingData->State==Consumed){
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






