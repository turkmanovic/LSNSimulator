/** @file 	TimeBase.h
 *  @brief  Simulator time logic
 *
 *
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

#ifndef TBASE_H_
#define TBASE_H_

#include <stdio.h>
#include <stdint.h>
#include "Job.h"		//haris

typedef enum{
	TBASE_OK,
	TBASE_ERROR
}tbase_status_t;

/**
* This structure represent TimeBase logic list element.
*
* In this structure are located all parameters necessary to event occur in desire moment of time.
*
*/
typedef struct Event {
	uint32_t ID;	/*!< Generic and unique value assigned by TimeBase logic	*/
	double Time;	/*!< The time moment in which the event occurs	*/
	uint16_t Priority;	/*!< Event priority	*/
	struct Job* AssignedJob; /*!< Assigned Job	*/
	struct Event* NextEvent; /*!< Next time Event. Field value is set by TimeBase Logic	*/
	struct Event* PreviousEvent; /*!< Previous time Event. Field value is set By TimeBase Logic*/
}event_t;
/**
* @brief Initialize Time base system part
* @return void
*/
void 			TBASE_Init();
/**
* @brief 	Create Event which will be added to TimeBase
* @param 	JobToProcess 	- 	Job which will be adder to Event
* @param 	Time		 	- 	Define moment when Event will occur
* @param 	Priority		-	Event priority. Higher value represents higher priority
* @return 	Event			-	Pointer to created Event
*/
event_t* 		TBASE_CreateEvent(struct Job* JobToProcess, double Time,uint16_t Priority);
/**
* @brief Add Event to time base
* @param 	EventToProcess 	- 	Event which will be added to time base
* @return 	1				-	Adding Event to list is done successfully
* @return 	0				-	Adding Event to list is failed
*/
uint8_t 		TBASE_AddEvent(event_t* EventToProcess);
/**	
* @brief Remove Event using Node, job Type and Data ID 
* @param	Node			-	Processed Node of job assigned to Event
* @param	Type			-	Job Type of job assinged to Event
* @param	DataID			-	ID of data processed in job assigned to Event
* @return	1				-	Removing Event from list is doned successfully
* @return	0				-	Removing Event from list is failed
*/
uint8_t			TBASE_RemoveEvent(node_t* Node, job_type_t Type, uint32_t DataID);
/**
 * @brief Get current system time
 * @return System Time
 */
double 	    	TBASE_GetTime();
/**
 * @brief Start Simulation
 * @param SimulationTime - Simulation time duration
 */
tbase_status_t 	TBASE_Start(uint32_t SimulationTime);

#endif /* TBASE_H_ */
