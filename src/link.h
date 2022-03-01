/*
 * Links.h
 *
 *  Created on: Aug 5, 2019
 *      Author: Puma
 */

#ifndef LINK_H_
#define LINK_H_

#include <stdint.h>



typedef struct {
	uint32_t    ID;					/*!< Unique number value */
	double		Speed;				/*!< Link speed */
	uint32_t    MTUSize;			/*!< MTU packet size */
	uint32_t    NumberOfHops;		/*!< Number of transmission unit on link */
	uint32_t    Deviation;			/*!< Deviation from defined speed  */
	double		ReceiveConsumption;	/*!< Current needed to receive data*/
	double		TransmitConsumption;	/*!< Current needed to send data*/
	char		LinkName[30];		/*!< Link name */
}link_t;


typedef struct node_t node_t;


typedef struct {
	uint32_t    ID;					/*!< Unique connection value */
	node_t* 	node1;				/*!< ID value of destination Node */
	node_t* 	node2;				/*!< ID value of current node */
	link_t*		AssignedLink;		/*!< Link info */
	Boolean		bussy;
	data_t*		currentTransferData;
	uint32_t	countingAccess;		/*!< Just for testing check how many times node try to acess busy link*/
	double		transferStartTime;
	double		transferEndTime;
}connection_t;

uint8_t 			LINK_Init();
link_t*   			LINK_GetByID(uint32_t LinkID);
double          	LINK_GetSpeed(connection_t* ConnectionPtr);


connection_t*    	CONNECTION_Create(node_t* node1, node_t* node2, uint32_t LinkId);
connection_t*		CONNECTION_GetTwoNodesConnection(node_t* node1, node_t* node2);



#endif /* LINK_H_ */
