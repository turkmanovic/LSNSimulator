/*
 * Links.h
 *
 *  Created on: Aug 5, 2019
 *      Author: Puma
 */

#ifndef LINKS_H_
#define LINKS_H_

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
}Link;
typedef struct {
	uint32_t    ID;					/*!< Unique connection value */
	uint32_t 	DestinationNodeId;	/*!< ID value of destination Node */
	uint32_t 	SourceNodeId;		/*!< ID value of current node */
	Link*		AssignedLink;		/*!< Link info */
}Connection;

uint8_t 		Init_Links();
Connection*    	Create_Connection(uint32_t DestinationNodeId, uint32_t SourceNodeId, uint32_t LinkId);
Link*   		Get_Link(uint32_t LinkID);
double          Get_LinkSpeed(Connection* ConnectionPtr);



#endif /* LINKS_H_ */
