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

#include "Nodes.h"


typedef enum JobType{
	Create,
	Process,
	Send,
	Receive
}JobType;

typedef struct Job{
	uint32_t ID;/*!<Unique ID assigned at Job creation */
	Boolean Periodic;/*!< Indicate if job is periodic */
	uint32_t Period;/*!< If Job is periodic, this value represent Job rate, otherwise this
	 	 	 	 	 	 parameter is not important*/
    Data*  ProcessedData;/*!< Data assigned to Job */
    Node*  ProcessedNode;/*!< Pointer to Node over which action must be taken */
    JobType Type;/*!< Type of Job */
}Job;

void 	Init_Job();
Job* 	Create_Job(Node* AssignedNodePtr, Data* AssignedDataPtr, Boolean Periodic, uint32_t Rate,JobType Type);
uint8_t Process_Job(Job* JobToProcess);

#endif /* JOB_H_ */
