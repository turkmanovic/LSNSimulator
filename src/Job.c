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
#include "job.h"

uint32_t prvJOB_ID_COUNTER;

void JOB_Init(){
	prvJOB_ID_COUNTER = 0;
}

job_t* JOB_Create(node_t* AssignedNodePtr, data_t* AssignedDataPtr, Boolean Periodic, uint32_t Rate,job_type_t Type){
	job_t* CreatedJob 			= malloc(sizeof(job_t));
	CreatedJob->ID 				= prvJOB_ID_COUNTER++;
	CreatedJob->Period 			= Rate;
	CreatedJob->Periodic 		= Periodic;
	CreatedJob->ProcessedNode 	= AssignedNodePtr;
	CreatedJob->ProcessedData 	= AssignedDataPtr;
	CreatedJob->Type 			= Type;
	return CreatedJob;
}

uint8_t JOB_Process(job_t* JobToProcess){
	switch(JobToProcess->Type){
	case JOB_TYPE_CREATE:
		NODE_ReceiveData(JobToProcess->ProcessedNode, JobToProcess->ProcessedData);
		if(JobToProcess->Periodic == True){
			NODE_MakeProducerNode(JobToProcess->ProcessedNode->ID,	JobToProcess->Period, True, JobToProcess->ProcessedData->Size, JobToProcess->ProcessedData->Path->Line, JobToProcess->ProcessedData->Path->DestinationID,True, JobToProcess->ProcessedData->AssignedProtocol->ID);
		}
		break;
	case JOB_TYPE_PROCESS:
		NODE_ProcessData(JobToProcess->ProcessedNode);
		break;
	case JOB_TYPE_SEND:
		NODE_SendData(JobToProcess->ProcessedNode);
		break;
	case JOB_TYPE_RECEIVE:
		NODE_ReceiveData(JobToProcess->ProcessedNode, JobToProcess->ProcessedData);
		break;
	}
	free(JobToProcess);
	return 1;
}
