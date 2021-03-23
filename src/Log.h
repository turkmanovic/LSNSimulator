/*
 * Error.h
 *
 *  Created on: Jul 24, 2019
 *      Author: Puma
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <stdint.h>
#include "Global.h"
#include "nodes.h"


//This error define that there is two Nodes with same ID
#define ERROR_NODE_0				0
//This error define that there is two Nodes with same ID
#define ERROR_NODE_1				1
//This error define that there is double linking with same node
#define ERROR_LINKING_0 			100
//This error define that there is no nodes to link with
#define ERROR_LINKING_1 			101
//Error with file opening
#define ERROR_FILE_0 				201
//Error with memory allocation
#define ERROR_ALLOCATING_0 			300
//There is two protocols with same ID
#define ERROR_PROTOCOL_0 			310
//There is no protocol for desire ID value
#define ERROR_PROTOCOL_1 			311
//There is two links with same ID
#define ERROR_LINK_0 				320
//There is no link for desire ID value
#define ERROR_LINK_1 				321

//This code indicate that node is successfully created
#define LOG_CREATED_NODE   			1000
//This code indicate that all nodes are successfully created
#define LOG_CREATED_ALL_NODES   	1001
//This code indicate that two nodes are linked
#define LOG_LINKED_NODE   			1100
//This code indicate that all nodes are linked
#define LOG_LINKED_ALL_NODES   		1101
//This code indicate that protocol is successfully created
#define LOG_PROTOCOL_0   			1200
//This code indicate that all protocol are successfully created
#define LOG_PROTOCOL_1   			1201
//This code indicate that link is successfully created
#define LOG_LINK_0   				1301
//This code indicate that all links are successfully created
#define LOG_LINK_1   				1302

uint8_t LOG_ERROR_Init();
void 	LOG_ERROR_Print(uint16_t ErrorNumber,const char AddInfo[]);

uint8_t Init_ProcessLog();
void 	Print_ProcessLog(uint16_t LogID, const char AddInfo[]);


uint8_t Init_DataLog();
//lpStatusFlag == 0 -> not lp status
//lpStatusFlag == 1 -> lp status
//lpMode	== 1 -> enter to LP mode
//lpMode	== 0 -> exit from LP mode
void 	Print_NodeLog(node_t* NodePtr,data_t* DataPtr,double Time, uint8_t lpStatusFlag, uint8_t lpMode);



#endif /* LOG_H_ */
