/*
 * Nodes.h
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */

#ifndef NODES_H_
#define NODES_H_

#include <stdio.h>
#include <stdlib.h>

#include "data.h"

#include "link.h"




#define	NODE_FILENAME_SIZE		30

typedef enum{
	NODE_OK,
	NODE_ERROR
}node_status_t;


typedef enum{
	NODE_TYPE_PRODUCER,
	NODE_TYPE_CONSUMER
}node_type_t;

typedef enum{
	NODE_OPMODE_FULLOPERATONAL,
	NODE_OPMODE_LOWPOWER,
}node_operational_mode_t;


typedef struct {
	uint32_t 		ID;								/*!< Unique Node value */
	double 			ProcessTime;					/*!< SData processing speed */
	Boolean 		Processing; 					/*!< Node busy indicator */
	node_type_t 	Type; 							/*!< Node type */
	data_t* 		processingData; 				/*!< Pointer to Data which currently processing on Node */

	uint32_t 		MTUProcessOverhead; 			/*!< Time require to process one MTU packet */
	uint32_t 		DataBufferSize; 				/*!< Size of data which will be return in Response messages */
	uint32_t 		SizeOfWaitData; 				/*!< Quantity of data which wait to be processed  */
	uint32_t 		ProcessedDataBytesCount; 		/*!< Quantity of data which are processed on node */
	uint32_t 		ProcessedOverheadBytesCount; 	/*!< Quantity of data overhead which are processed on node */
	uint32_t		processedDataCount;

	data_t**		processedDataBuffer;
	uint32_t		processedDataBufferSize;

	data_t** 		receivedDataBuffer; 			/*!< List of Data which wait to be processed */
	uint32_t 		receivedDataBufferSize;			/*!< Number of data which wait to be processed */


	double			lpConsumption;					/*!< Consumption of node while it is in Low power mode(in mA) */
	double			activeConsumption;				/*!< Consumption of node while it is active mode(in mA)*/
	double 			FullConsumption;				/*!< Full dissipation on node */
	double			lpEnterTime;					/*!< Time stamp when node enter in LP mode*/
	connection_t** 	AdjacentNodes; 					/*!< List of all adjacent connection */
	uint32_t 		compressionLevel; 				/*!< Aggregation Level */
	char			LogFilename[NODE_FILENAME_SIZE];
	FILE 			*NodeLogFile;  					/*!< Pointer to node file */
	node_operational_mode_t	operationalMode;		/*!< Current active node operational mode*/
	uint32_t		agregationLevel;
	double			currentConsumption;
}node_t;


/**
* @brief Initialize all Node structures and variables
*/
void 	NODE_Init();
/**
* @brief Create DataPath for Data
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
node_t*		NODE_Create(uint32_t NodeId, double ProcessTime, uint32_t CompressionLevel, uint32_t ReturnSize,uint32_t MTUProcessOverhead, double lpConsumption, double activeConsumption, uint32_t AggregationLevel);
node_t*   	NODE_GetById(uint32_t NodeId);
uint8_t		NODE_LinkToNode(uint32_t Node1Id, uint32_t Node2Id,uint32_t LinkID);

void    	NODE_MakeProducerNode(uint32_t NodeId, uint32_t Rate, Boolean Periodic,uint32_t Size, uint32_t* PathLine,uint32_t DestinationId,	Boolean RelativeFlag,	uint32_t ProtocolID);


node_status_t 			NODE_ReceiveData(node_t* NodePtr, data_t* DataPtr);
node_status_t 			NODE_StartReceiveData(node_t* NodePtr, data_t* DataPtr);
node_status_t 			NODE_ProcessData(node_t* NodePtr);
node_status_t 			NODE_StartProcessData(node_t* NodePtr);
node_status_t 			NODE_ProcessMTUData(node_t* NodePtr);
node_status_t 			NODE_StartProcessMTUData(node_t* NodePtr);
node_status_t 			NODE_TransmitData(node_t* NodePtr);
node_status_t 			NODE_StartTransmitData(node_t* NodePtr);

node_status_t			NODE_GoToLPMode(node_t* NodePtr, double time);
node_status_t			NODE_WakeFromLPMode(node_t* NodePtr, double time);
node_operational_mode_t	NODE_GetOperationalMode(node_t* NodePtr);




connection_t*   NODE_GetNextLink(node_t* NodePtr);


#endif /* NODES_H_ */
