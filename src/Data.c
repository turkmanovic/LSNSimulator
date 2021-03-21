/** @file 	Data.c
 *  @brief  Data Network's part definitions
 *
 *   This file contains data and function definitions which are used in Data processing over the
 *   network. Data is one of the network parts and it is created to store information which represents
 *   data exchanged between network's Nodes. Data travel between nodes based on Path defined by user in
 *   Topology configuration file. Each Data has assigned protocols which defined properties of communication.
 *   Protocols properties are defined in Protocol configuration file.
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

/**
 * Standard include Files
 * */
#include "String.h"

/**
 * User Defined include Files
 * */
#include "Data.h"
#include "Global.h"
#include "Log.h"
#include "TimeBase.h"


FILE 			*ProtocolFile;		/*!< Store pointer to protocol file	*/
Protocol** 		DefinedProtocols;	/*!< List of all protocols define in protocol file */
uint32_t 		ProtocolsNo;		/*!< Number of protocol	*/
uint32_t 		DataIdCounter;		/*!< Used to create generic Data IDs	*/




/**
* @brief Check if Data require response and create corresponding response if it is necessary
*
* 	Search protocol defined by ID in list of protocols ((DefinedProtocols))
*
* @param ProtocolID		- Protocol ID
* @return Protocol* 	- Return pointer to Protocol which ID is sent by parameter ProtocolID
* @return NULL 			- If there is no protocol with ID value define by ProtocolID
*/
Protocol*   Get_Protocol_Data(uint32_t ProtocolID){
	uint32_t i=0;
	while(i < ProtocolsNo){
		if(DefinedProtocols[i]->ID == ProtocolID){
			return DefinedProtocols[i];
		}
		i++;
	}
	return NULL;
}
/**
* @brief Read protocol file and create protocols
*
* This function processing Protocol file and create Protocol structure based on data written in Protocol file.
* That file has a defined format by which the data is extracted from it. That format is:
*    ProtocolID  RequireResponse Overhead ProtocolName
*
*    -> ProtocolID 		- Unique value which identify protocol properties
*    -> RequireResponse	- This field has two values: y - Protocol require response
*    												 n - Protocol doesn't require response
*    -> Overhead			- Number of bytes which this protocol requires.
*    -> ProtocolName       - String which represent protocol name
*
* @param ProtocolID		- Protocol ID
* @return Protocol* 	- Return pointer to Protocol which ID is sent by parameter ProtocolID
* @return NULL 			- If there is no protocol with ID value define by ProtocolID
*/
uint8_t Parse_Protocols(){
	char TempChar;					/*!< User as instant variable to store read char 	*/
	uint32_t TempId, TempOverhead;	/*!< ID and Overhead read from file	*/
	char TempResponse;				/*!< RequireResponse value 	*/
	char TempString[50];			/*!< This string is used as string to store protocol name, error name, etc	*/
	ProtocolsNo = 0;				/*!< Init number of protocols	*/
	while(1){
		TempChar = (char)fgetc(ProtocolFile);
		if(TempChar == '-') break;
		fseek(ProtocolFile,-1,SEEK_CUR);
		/*Read falues from file in defined format*/
		fscanf(ProtocolFile,"%d %c %d %s",&TempId, &TempResponse, &TempOverhead,TempString);
		/*Check if there is protocol with same ID*/
		if(Get_Protocol_Data(TempId) != NULL){
			/*If there is a protocol with same id print error*/
			sprintf(TempString,"%d",TempId);
			Print_ErrorLog(ERROR_PROTOCOL_0,TempString);
			return 1;
		}
		/*Create protocl and add it to the list of protocols*/
		ProtocolsNo++;
		DefinedProtocols = realloc(DefinedProtocols,ProtocolsNo*sizeof(Protocol*));
		DefinedProtocols[ProtocolsNo-1]=malloc(sizeof(Protocol));
		DefinedProtocols[ProtocolsNo-1]->ID = TempId;
		DefinedProtocols[ProtocolsNo-1]->Overhead = TempOverhead;
		if(TempResponse == 'y'){
			DefinedProtocols[ProtocolsNo-1]->Response	= True;
		}
		else{
			DefinedProtocols[ProtocolsNo-1]->Response 	= False;
		}
		strcpy(DefinedProtocols[ProtocolsNo-1]->ProtocolName, TempString);
		sprintf(TempString,"ID: %d, Name: %s",DefinedProtocols[ProtocolsNo-1]->ID,DefinedProtocols[ProtocolsNo-1]->ProtocolName);
		/*Print in log that protocol is successfully create*/
		Print_ProcessLog(LOG_PROTOCOL_0,TempString);
		while(TempChar != '\n') TempChar = (char)fgetc(ProtocolFile);
	}
	/*Print in log that all protocols are successfully created*/
	Print_ProcessLog(LOG_PROTOCOL_1,"");
	fclose(ProtocolFile);
	return 0;
}

/**
* @brief Initialize Data part of system
*
* 	Open Protocol file and parse it
*
* @return 0 - Initialization success
* @return 1 - Initialization failed
*/
uint8_t Init_Data(){
	DataIdCounter = 0;  /*!< Initialize numbers of data*/
	ProtocolsNo = 0;    /*!< Initialize numbers of protocols*/
	ProtocolFile = fopen(Protocols_FileName,"r");
	if(ProtocolFile == 0){
		Print_ErrorLog(ERROR_FILE_0,Protocols_FileName);
		return 1;
	}
	DefinedProtocols=malloc(sizeof(Protocol*));
	if(DefinedProtocols==0){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Problem with protocols allocation");
		return 1;
	}
	if(Parse_Protocols()!=0){
		return 1;
	}
	return 0;
}
/**
* @brief Create DataPath for Data
*
*	This function create DataPath structure based on PathLine and destination ID.
*	If there is a errors during allocation, this function return NULL
*
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
DataPath* Create_Path_Data(uint32_t DestinationId, uint32_t* PathLine){
	DataPath* CreatedPath=malloc(sizeof(DataPath));
	if(CreatedPath==0){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Problem with Path allocation");
		return NULL;
	}
	uint32_t Counter=0;
	CreatedPath->CurrentID = 0;
	while(PathLine[Counter]!=DestinationId)Counter++;
	CreatedPath->Line = malloc((Counter+1)*sizeof(uint32_t));
	if(CreatedPath->Line==0){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Problem with Path line allocation");
		return NULL;
	}
	Counter=0;
	do{
		CreatedPath->Line[Counter] = PathLine[Counter];
	}while(PathLine[Counter++]!=DestinationId);
	CreatedPath->DestinationID = DestinationId;
	return CreatedPath;
}
/**
* @brief Create Data structure and initialize it for use
*
* 	This function create Data structure and initialize its fields for use. Size, DataPath and Protocol are
* 	pass to data as arguments. Data state is Unitialized and Type of Data is Request.
*
* @param Size		 	- Data size in bytes
* @param DataPath 		- Previously created DataPath
* @param ProtocolId 	- Protocol ID value assigned to data
* @return Data* 		- Return pointer to data if creation was successful
* @return NULL 			- If creation wasn't successful
*/
Data* Create_Data(uint32_t Size,DataPath* Path,uint32_t ProtocolID){
	char TempString[30];   /*!< USed to store error messages*/
	Data* CreatedData 	= malloc(sizeof(Data));
	if(CreatedData == NULL){
		Print_ErrorLog(ERROR_ALLOCATING_0,"Data allocation");
		return NULL;
	}
	CreatedData->Path 			= Path;
	CreatedData->Size 			= Size;
	CreatedData->BytesToProcess	= Size;
	CreatedData->Overhead	    = 0;
	CreatedData->ElapsedRequestTime 	= 0;
	CreatedData->ElapsedResponseTime 	= 0;
	CreatedData->CreatedTime 			= 0;
	CreatedData->ID 			= DataIdCounter++;
	CreatedData->State			= Unitialized;
	CreatedData->Type			= Request;
	CreatedData->AssignedProtocol = Get_Protocol_Data(ProtocolID);
	if(CreatedData->AssignedProtocol == NULL){
		sprintf(TempString,"%d",ProtocolID);
		Print_ErrorLog(ERROR_PROTOCOL_1,TempString);
		free(CreatedData);
		return NULL;
	}
	return CreatedData;
}
/**
* @brief Check if Data require response and create corresponding response if it is necessary
*
*   This function check if protocol assigned to data require response and create response if it is necessary
*
* @param 	Data*			- Data to check
* @return 	0 				- Data require response and response is created
* @return	1 				- Data does not require response
*/
uint8_t		Create_Response_Data(Data* DataPtr,uint32_t ResponseSize){
	if( (DataPtr->AssignedProtocol->Response == False)) return 1;
	if((DataPtr->Type != Request)) return 1;
	uint32_t Counter_Start = 0;
	uint32_t Counter_End = 0;
	uint32_t Temp;
	while(DataPtr->Path->Line[Counter_End] != DataPtr->Path->DestinationID) Counter_End++;
	DataPtr->Path->DestinationID = DataPtr->Path->Line[0];
	while(Counter_End>Counter_Start){
		Temp = DataPtr->Path->Line[Counter_End];
		DataPtr->Path->Line[Counter_End] = DataPtr->Path->Line[Counter_Start];
		DataPtr->Path->Line[Counter_Start] = Temp;
		Counter_Start++;
		Counter_End--;
	}
	DataPtr->Type = Response;
	DataPtr->BytesToProcess =ResponseSize;
	DataPtr->Size =ResponseSize;
	DataPtr->State = Unitialized;
	return 0;
}


