/** @file 	TimeBase.c
 *  @brief  Simulator time logic
 *
 *
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

#include "tbase.h"

#include "job.h"



event_t* 	prvTBASE_LIST_HEAD;
uint32_t 	prvTBASE_EVENT_ID_COUNTER;
uint32_t 	prvTBASE_PROCESSING;
double 		prvTBASE_TIME;

void TBASE_Init(){
	prvTBASE_LIST_HEAD = NULL;
	prvTBASE_EVENT_ID_COUNTER = 0;
	prvTBASE_TIME = 0;
	prvTBASE_PROCESSING = 0;
}

event_t* TBASE_CreateEvent(job_t* JobToProcess, double Time,uint16_t Priority){
	event_t* TempEvent = malloc(sizeof(event_t));
	TempEvent->AssignedJob = JobToProcess;
	TempEvent->ID = prvTBASE_EVENT_ID_COUNTER++;
	TempEvent->Priority = Priority;
	TempEvent->Time = Time;
	TempEvent->NextEvent = NULL;
	TempEvent->PreviousEvent = NULL;
	return TempEvent;
}

uint8_t TBASE_AddEvent(event_t* EventToProcess){
	event_t* CurrentListElement = prvTBASE_LIST_HEAD;
	if(prvTBASE_LIST_HEAD == NULL){
		prvTBASE_LIST_HEAD = EventToProcess;
	}
	else{
		while( CurrentListElement!= NULL){
			if(((CurrentListElement->Time == EventToProcess->Time) && (CurrentListElement->Priority <= EventToProcess->Priority))|| (CurrentListElement->Time > EventToProcess->Time)){
				if(CurrentListElement->PreviousEvent == NULL){
					if(prvTBASE_PROCESSING == 1){
						if(CurrentListElement->NextEvent == NULL){
							CurrentListElement->NextEvent = EventToProcess;
						}
						else{
							CurrentListElement->NextEvent->PreviousEvent = EventToProcess;
							EventToProcess->NextEvent = CurrentListElement->NextEvent;
							EventToProcess->PreviousEvent = CurrentListElement;
							CurrentListElement->NextEvent = EventToProcess;
						}
					}
					else{
						CurrentListElement->PreviousEvent = EventToProcess;
						EventToProcess->NextEvent = CurrentListElement;
						prvTBASE_LIST_HEAD = EventToProcess;
					}
				}
				else{
					CurrentListElement->PreviousEvent->NextEvent = EventToProcess;
					EventToProcess->PreviousEvent = CurrentListElement->PreviousEvent;
					EventToProcess->NextEvent = CurrentListElement;
					CurrentListElement->PreviousEvent = EventToProcess;
				}
				break;
			}
			if(CurrentListElement->NextEvent == NULL){
				CurrentListElement->NextEvent= EventToProcess;
				EventToProcess->PreviousEvent= CurrentListElement;
				break;
			}
			CurrentListElement =  CurrentListElement->NextEvent;
		}
	}
	return 1;

}
double TBASE_GetTime(){
	return prvTBASE_TIME;
}
tbase_status_t TBASE_Start(uint32_t SimulationTime){
	event_t* CurrentEvent	=	prvTBASE_LIST_HEAD;
	event_t* TempPointer;
	prvTBASE_PROCESSING = 1;
	while((CurrentEvent	!=	NULL) && (CurrentEvent->Time <= SimulationTime)){
		if(prvTBASE_LIST_HEAD->Time != prvTBASE_TIME){
			prvTBASE_TIME = prvTBASE_LIST_HEAD->Time;
		}
		if(JOB_Process(CurrentEvent->AssignedJob) != JOB_OK){
			puts("TBASE: Error during job processing");
			return TBASE_ERROR;
		}
		TempPointer 	= CurrentEvent;
		CurrentEvent 	= CurrentEvent->NextEvent;
		if(CurrentEvent!=NULL){
			CurrentEvent->PreviousEvent = NULL;
		}
		prvTBASE_LIST_HEAD = CurrentEvent;
		free(TempPointer);
	}
	return TBASE_OK;
}
