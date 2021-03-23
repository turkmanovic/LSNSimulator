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
typedef struct {
	uint32_t    ID;					/*!< Unique connection value */
	uint32_t 	DestinationNodeId;	/*!< ID value of destination Node */
	uint32_t 	SourceNodeId;		/*!< ID value of current node */
	link_t*		AssignedLink;		/*!< Link info */
}connection_t;

uint8_t 			LINK_Init();
connection_t*    	LINK_CreateConnection(uint32_t DestinationNodeId, uint32_t SourceNodeId, uint32_t LinkId);
link_t*   			LINK_GetByID(uint32_t LinkID);
double          	LINK_GetSpeed(connection_t* ConnectionPtr);



#endif /* LINK_H_ */
