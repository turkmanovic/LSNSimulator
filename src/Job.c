/** @file 	Job.c
 *  @brief  Data Network's part declarations
 *
 *   This file contains data and function declarations which are used in Data processing over the
 *   network. Data is one of the network parts and it is created to store information which represents
 *   data exchanged between Nodes in network. Data travel between nodes based on Path defined by user in
 *   Topology configuration file. Each Data has assigned protocols which defined properties of communication.
 *   Protocols properties are defined in Protocol configuration file.
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */
#include "Job.h"

uint32_t JobIdCounter;

void Init_Job(){
	JobIdCounter = 0;
}

Job* Create_Job(Node* AssignedNodePtr, Data* AssignedDataPtr, Boolean Periodic, uint32_t Rate,JobType Type){
	Job* CreatedJob 			= malloc(sizeof(Job));
	CreatedJob->ID 				= JobIdCounter++;
	CreatedJob->Period 			= Rate;
	CreatedJob->Periodic 		= Periodic;
	CreatedJob->ProcessedNode 	= AssignedNodePtr;
	CreatedJob->ProcessedData 	= AssignedDataPtr;
	CreatedJob->Type 			= Type;
	return CreatedJob;
}

uint8_t Process_Job(Job* JobToProcess){
	switch(JobToProcess->Type){
	case Create:
		Receive_DataOn_Node(JobToProcess->ProcessedNode, JobToProcess->ProcessedData);
		if(JobToProcess->Periodic == True){
			Make_Producer_Node(JobToProcess->ProcessedNode->ID,	JobToProcess->Period, True, JobToProcess->ProcessedData->Size, JobToProcess->ProcessedData->Path->Line, JobToProcess->ProcessedData->Path->DestinationID,True, JobToProcess->ProcessedData->AssignedProtocol->ID);
		}
		break;
	case Process:
		Process_DataOn_Node(JobToProcess->ProcessedNode);
		break;
	case Send:
		Send_DataFrom_Node(JobToProcess->ProcessedNode);
		break;
	case Receive:
		Receive_DataOn_Node(JobToProcess->ProcessedNode, JobToProcess->ProcessedData);
		break;
	}
	free(JobToProcess);
	return 1;
}
