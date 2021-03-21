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


#include "Data.h"
#include "Links.h"




#define	NODE_FILENAME_SIZE		30


typedef enum{
	Producer,
	Consumer
}NodeType;


typedef struct {
	uint32_t 		ID;								/*!< Unique Node value */
	double 			ProcessTime;					/*!< SData processing speed */
	Boolean 		Processing; 					/*!< Node busy indicator */
	NodeType 		Type; 							/*!< Node type */
	Data* 			ProcessingData; 				/*!< Pointer to Data which currently processing on Node */
	Data** 			WaitData; 						/*!< List of Data which wait to be processed */
	uint32_t 		NoOfWaitData;					/*!< Number of data which wait to be processed */
	uint32_t 		MTUProcessOverhead; 			/*!< Time require to process one MTU packet */
	uint32_t 		DataBufferSize; 				/*!< Size of data which will be return in Response messages */
	uint32_t 		SizeOfWaitData; 				/*!< Quantity of data which wait to be processed  */
	uint32_t 		ProcessedDataBytesCount; 		/*!< Quantity of data which are processed on node */
	uint32_t 		ProcessedOverheadBytesCount; 	/*!< Quantity of data overhead which are processed on node */
	double			Consumption;					/*!< Consumption of node while processing in mA */
	double 			FullConsumption;				/*!< Full dissipation on node */
	Connection** 	AdjacentNodes; 					/*!< List of all adjacent connection */
	uint32_t 		AggregationLevel; 				/*!< Aggregation Level */
	char			LogFilename[NODE_FILENAME_SIZE];
	FILE 			*NodeLogFile;  					/*!< Pointer to node file */
}Node;


/**
* @brief Initialize all Node structures and variables
*/
void 	Init_Node();
/**
* @brief Create DataPath for Data
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
Node*	Create_Node(uint32_t NodeId, double ProcessTime, uint32_t AgregationLevel, uint32_t ReturnSize,uint32_t MTUProcessOverhead, double Consumption);
Node*   Get_Node(uint32_t NodeId);
uint8_t	Link_Node(uint32_t Node1Id, uint32_t Node2Id,uint32_t LinkID);

void    Make_Producer_Node(uint32_t NodeId, uint32_t Rate, Boolean Periodic,uint32_t Size, uint32_t* PathLine,uint32_t DestinationId,Boolean RelativeFlag,uint32_t ProtocolID);
void 	Receive_DataOn_Node(Node* NodePtr, Data* DataPtr);
void 	Process_DataOn_Node(Node* NodePtr);
void 	Send_DataFrom_Node(Node* NodePtr);

Connection*   Get_NextLink_Node(Node* NodePtr);


#endif /* NODES_H_ */
