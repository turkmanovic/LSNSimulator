/** @file 	Job.h
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

#ifndef JOB_H_
#define JOB_H_

#include <stdint.h>

#include "nodes.h"

typedef enum{
	JOB_OK,
	JOB_ERROR
}job_status_t;

typedef enum{
	JOB_TYPE_CREATE,
	JOB_TYPE_PROCESS_DATA_START,
	JOB_TYPE_PROCESS_DATA_END,
	JOB_TYPE_PROCESS_MTU_START,
	JOB_TYPE_PROCESS_MTU_END,
	JOB_TYPE_TRANSMIT,
	JOB_TYPE_TRANSMIT_START,
	JOB_TYPE_RECEIVE,
	JOB_TYPE_RECEIVE_START,
}job_type_t;

typedef struct Job{
	uint32_t 	ID;/*!<Unique ID assigned at Job creation */
	Boolean 	Periodic;/*!< Indicate if job is periodic */
	double 	Period;/*!< If Job is periodic, this value represent Job rate, otherwise this
	 	 	 	 	 	 parameter is not important*/
    data_t*  	ProcessedData;/*!< Data assigned to Job */
    node_t*  	ProcessedNode;/*!< Pointer to Node over which action must be taken */
    job_type_t 	Type;/*!< Type of Job */
}job_t;

void 			JOB_Init();
job_t* 			JOB_Create(node_t* AssignedNodePtr, data_t* AssignedDataPtr, Boolean Periodic, double Rate,job_type_t Type);
job_status_t 	JOB_Process(job_t* JobToProcess);

#endif /* JOB_H_ */
