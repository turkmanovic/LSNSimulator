/** @file 	TimeBase.h
 *  @brief  Simulator time logic
 *
 *
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

#ifndef TIMEBASE_H_
#define TIMEBASE_H_

#include <stdio.h>
#include <stdint.h>

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
}Event;
/**
* @brief Initialize Time base system part
* @return void
*/
void 		Init_TimeBase();
/**
* @brief 	Create Event which will be added to TimeBase
* @param 	JobToProcess 	- 	Job which will be adder to Event
* @param 	Time		 	- 	Define moment when Event will occur
* @param 	Priority		-	Event priority. Higher value represents higher priority
* @return 	Event			-	Pointer to created Event
*/
Event* 		Create_Event(struct Job* JobToProcess, double Time,uint16_t Priority);
/**
* @brief Add Event to time base
* @param 	EventToProcess 	- 	Event which will be added to time base
* @return 	1				-	Adding Event to list is done successfully
* @return 	0				-	Adding Event to list is failed
*/
uint8_t 	Add_Event(Event* EventToProcess);
/**
 * @brief Get current system time
 * @return System Time
 */
double 	    Get_CurrentTime();
/**
 * @brief Start Simulation
 * @param SimulationTime - Simulation time duration
 */
void 		StartTime(uint32_t SimulationTime);

#endif /* TIMEBASE_H_ */
