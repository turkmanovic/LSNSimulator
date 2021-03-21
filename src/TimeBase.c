/** @file 	TimeBase.c
 *  @brief  Simulator time logic
 *
 *
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

#include "TimeBase.h"
#include "Job.h"



Event* ListHead;
uint32_t EventIdCounter;
double CurrentTime;
uint32_t Processing;

void Init_TimeBase(){
	ListHead = NULL;
	EventIdCounter = 0;
	CurrentTime = 0;
	Processing = 0;
}

Event* Create_Event(Job* JobToProcess, double Time,uint16_t Priority){
	Event* TempEvent = malloc(sizeof(Event));
	TempEvent->AssignedJob = JobToProcess;
	TempEvent->ID = EventIdCounter++;
	TempEvent->Priority = Priority;
	TempEvent->Time = Time;
	TempEvent->NextEvent = NULL;
	TempEvent->PreviousEvent = NULL;
	return TempEvent;
}

uint8_t Add_Event(Event* EventToProcess){
	Event* CurrentListElement = ListHead;
	if(ListHead == NULL){
		ListHead = EventToProcess;
	}
	else{
		while( CurrentListElement!= NULL){
			if(((CurrentListElement->Time == EventToProcess->Time) && (CurrentListElement->Priority <= EventToProcess->Priority))|| (CurrentListElement->Time > EventToProcess->Time)){
				if(CurrentListElement->PreviousEvent == NULL){
					if(Processing == 1){
						CurrentListElement->NextEvent->PreviousEvent = EventToProcess;
						EventToProcess->NextEvent = CurrentListElement->NextEvent;
						EventToProcess->PreviousEvent = CurrentListElement;
						CurrentListElement->NextEvent = EventToProcess;
					}
					else{
						CurrentListElement->PreviousEvent = EventToProcess;
						EventToProcess->NextEvent = CurrentListElement;
						ListHead = EventToProcess;
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
double Get_CurrentTime(){
	return CurrentTime;
}
void StartTime(uint32_t SimulationTime){
	Event* CurrentEvent=ListHead;
	Event* TempPointer;
	Processing = 1;
	while(CurrentEvent!=NULL && CurrentEvent->Time <= SimulationTime){
		if(ListHead->Time!= CurrentTime){CurrentTime = ListHead->Time;}
		Process_Job(CurrentEvent->AssignedJob);
		TempPointer = CurrentEvent;
		CurrentEvent = CurrentEvent->NextEvent;
		if(CurrentEvent!=NULL){
			CurrentEvent->PreviousEvent = NULL;
		}
		ListHead = CurrentEvent;
		free(TempPointer);
	}

}
