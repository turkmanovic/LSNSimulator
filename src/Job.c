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

job_status_t JOB_Process(job_t* JobToProcess){
	switch(JobToProcess->Type){
		case JOB_TYPE_CREATE:
			if(NODE_ReceiveData(JobToProcess->ProcessedNode, JobToProcess->ProcessedData) != NODE_OK){
				puts("JOB: Error during data receiving");
				return JOB_ERROR;
			}
			if(JobToProcess->Periodic == True){
				NODE_MakeProducerNode(JobToProcess->ProcessedNode->ID,	JobToProcess->Period, True, JobToProcess->ProcessedData->Size, JobToProcess->ProcessedData->Path->line, JobToProcess->ProcessedData->Path->destinationID,True, JobToProcess->ProcessedData->AssignedProtocol->ID);
			}
			break;
		case JOB_TYPE_PROCESS:
			if(NODE_ProcessData(JobToProcess->ProcessedNode) != NODE_OK){
				puts("JOB: Error during data processing");
				return JOB_ERROR;
			}
			break;
		case JOB_TYPE_PROCESS_MTU:
			if(NODE_ProcessMTUData(JobToProcess->ProcessedNode) != NODE_OK){
				puts("JOB: Error during mtu data processing");
				return JOB_ERROR;
			}
			break;
		case JOB_TYPE_SEND:
			if(NODE_SendData(JobToProcess->ProcessedNode) != NODE_OK){
				puts("JOB: Error during data sending");
				return JOB_ERROR;
			}
			break;
		case JOB_TYPE_RECEIVE:
			if(NODE_ReceiveData(JobToProcess->ProcessedNode, JobToProcess->ProcessedData) != NODE_OK){
				puts("JOB: Error during data receiving");
				return JOB_ERROR;
			}
			break;
		case JOB_TYPE_RECEIVE_AGG:
			if(NODE_ReceiveAggData(JobToProcess->ProcessedNode, JobToProcess->ProcessedData) != NODE_OK){
				puts("JOB: Error during data receiving");
				return JOB_ERROR;
			}
			break;
	}
	free(JobToProcess);
	return JOB_OK;
}
