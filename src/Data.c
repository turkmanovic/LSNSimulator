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
#include "data.h"

#include "String.h"

/**
 * User Defined include Files
 * */
#include "Global.h"
#include "Log.h"
#include "tbase.h"


FILE 			*ProtocolFile;		/*!< Store pointer to protocol file	*/
protocol_t** 		DefinedProtocols;	/*!< List of all protocols define in protocol file */
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
protocol_t*   DATA_GetProtocol(uint32_t ProtocolID){
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
		if(DATA_GetProtocol(TempId) != NULL){
			/*If there is a protocol with same id print error*/
			sprintf(TempString,"%d",TempId);
			LOG_ERROR_Print(ERROR_PROTOCOL_0,TempString);
			return 1;
		}
		/*Create protocl and add it to the list of protocols*/
		ProtocolsNo++;
		DefinedProtocols = realloc(DefinedProtocols,ProtocolsNo*sizeof(protocol_t*));
		DefinedProtocols[ProtocolsNo-1]=malloc(sizeof(protocol_t));
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
uint8_t DATA_Init(){
	DataIdCounter = 0;  /*!< Initialize numbers of data*/
	ProtocolsNo = 0;    /*!< Initialize numbers of protocols*/
	ProtocolFile = fopen(Protocols_FileName,"r");
	if(ProtocolFile == 0){
		LOG_ERROR_Print(ERROR_FILE_0,Protocols_FileName);
		return 1;
	}
	DefinedProtocols=malloc(sizeof(protocol_t*));
	if(DefinedProtocols==0){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Problem with protocols allocation");
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
data_path_t* DATA_PATH_Create(uint32_t DestinationId, uint32_t* PathLine){
	data_path_t* CreatedPath=malloc(sizeof(data_path_t));
	if(CreatedPath==0){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Problem with Path allocation");
		return NULL;
	}
	uint32_t Counter=0;
	CreatedPath->currentID = PathLine[0];
	while(PathLine[Counter]!=DestinationId)Counter++;
	CreatedPath->line = malloc((Counter+1)*sizeof(uint32_t));
	if(CreatedPath->line==0){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Problem with Path line allocation");
		return NULL;
	}
	Counter=0;
	do{
		CreatedPath->line[Counter] = PathLine[Counter];
	}while(PathLine[Counter++]!=DestinationId);
	CreatedPath->destinationID 			= 	DestinationId;
	CreatedPath->numberOfNodesOnPath	=	Counter;
	CreatedPath->currentIDPossition		=   0;
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
data_t* DATA_RAW_Create(uint32_t Size, data_path_t* Path, uint32_t ProtocolID){
	char TempString[30];   /*!< USed to store error messages*/
	data_t* CreatedData 	= malloc(sizeof(data_t));
	if(CreatedData == NULL){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Data allocation");
		return NULL;
	}
	CreatedData->Path 					= Path;
	CreatedData->Size 					= Size;
	CreatedData->BytesToProcess			= Size;
	CreatedData->Overhead	    		= 0;
	CreatedData->ElapsedRequestTime 	= 0;
	CreatedData->ElapsedResponseTime 	= 0;
	CreatedData->CreatedTime 			= 0;
	CreatedData->ID 					= DataIdCounter++;
	CreatedData->State					= DATA_STATE_UNITIALIZED;
	CreatedData->Type					= DATA_TYPE_REQUEST;
	CreatedData->AssignedProtocol 		= DATA_GetProtocol(ProtocolID);
	if(CreatedData->AssignedProtocol == NULL){
		sprintf(TempString,"%d",ProtocolID);
		LOG_ERROR_Print(ERROR_PROTOCOL_1,TempString);
		free(CreatedData);
		return NULL;
	}
	CreatedData->aggregationFlag		= DATA_AGG_RAW;
	CreatedData->aggreationList			= NULL;
	return CreatedData;
}
/**
* @brief Create Aggregation Data structure and initialize it for use
* @param Size		 	- Data size in bytes
* @param DataPath 		- Previously created DataPath
* @param ProtocolId 	- Protocol ID value assigned to data
* @return Data* 		- Return pointer to data if creation was successful
* @return NULL 			- If creation wasn't successful
*/
data_t* 		DATA_AGG_Create(data_path_t* Path,uint32_t ProtocolID,double time){
	char TempString[30];   /*!< USed to store error messages*/
	data_t* CreatedData 	= malloc(sizeof(data_t));
	if(CreatedData == NULL){
		LOG_ERROR_Print(ERROR_ALLOCATING_0,"Data allocation");
		return NULL;
	}
	CreatedData->Path 					= Path;
	CreatedData->Size 					= 0;
	CreatedData->BytesToProcess			= 0;
	CreatedData->Overhead	    		= 0;
	CreatedData->ElapsedRequestTime 	= 0;
	CreatedData->ElapsedResponseTime 	= 0;
	CreatedData->CreatedTime 			= time;
	CreatedData->ID 					= DataIdCounter++;
	CreatedData->State					= DATA_STATE_CREATED;
	CreatedData->Type					= DATA_TYPE_REQUEST;
	CreatedData->AssignedProtocol 		= DATA_GetProtocol(ProtocolID);
	if(CreatedData->AssignedProtocol == NULL){
		sprintf(TempString,"%d",ProtocolID);
		LOG_ERROR_Print(ERROR_PROTOCOL_1,TempString);
		free(CreatedData);
		return NULL;
	}
	CreatedData->aggregationFlag		= DATA_AGG_FULL;
	CreatedData->aggreationList			= malloc(sizeof(data_aggregation_list_t));
	CreatedData->aggreationList->numberOfData = 0;
	CreatedData->aggreationList->dataList=malloc(sizeof(data_t*));
	if(CreatedData->aggreationList == NULL){
		return NULL;
	}
	return CreatedData;
}
/**
* @brief Add Data to aggregation list
* @param  rootData		- Pointer to aggregation data structure
* @param  chilldData	- Pointer to data which wants to be aggregate
* @return 0 			- Data is successfully added to aggregation list
* @return 1 			- Data cant'b added to aggregation list because path is not well
* @return 2 			- Data cant'b added to aggregation list because protocol is not well
* @return 3 			- There is a error while adding data to aggregation list
*/
uint8_t 		DATA_AGG_AddData(data_t* rootData, data_t* childData){
	if(rootData->aggregationFlag != DATA_AGG_FULL) return 3;
	//check if data are on the same node
	if(childData->Path->currentID != rootData->Path->currentID) return 1;
	//check if next node is same for the both data
	if(childData->Path->line[childData->Path->currentIDPossition + 1] != rootData->Path->line[rootData->Path->currentIDPossition + 1]) return 1;
	rootData->aggreationList->numberOfData +=	1;
	rootData->aggreationList->dataList		= realloc(rootData->aggreationList->dataList,rootData->aggreationList->numberOfData*sizeof(data_t*));
	rootData->aggreationList->dataList[rootData->aggreationList->numberOfData - 1] = childData;
	rootData->Size	+= childData->Size;
	return 0;
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
uint8_t		DATA_CreateResponse(data_t* DataPtr,uint32_t ResponseSize){
	if( (DataPtr->AssignedProtocol->Response == False)) return 1;
	if((DataPtr->Type != DATA_TYPE_REQUEST)) return 1;
	uint32_t Counter_Start = 0;
	uint32_t Counter_End = 0;
	uint32_t Temp;
	while(DataPtr->Path->line[Counter_End] != DataPtr->Path->destinationID) Counter_End++;
	DataPtr->Path->destinationID = DataPtr->Path->line[0];
	while(Counter_End>Counter_Start){
		Temp = DataPtr->Path->line[Counter_End];
		DataPtr->Path->line[Counter_End] = DataPtr->Path->line[Counter_Start];
		DataPtr->Path->line[Counter_Start] = Temp;
		Counter_Start++;
		Counter_End--;
	}
	DataPtr->Type = DATA_TYPE_RESPONSE;
	DataPtr->BytesToProcess =ResponseSize;
	DataPtr->Size =ResponseSize;
	DataPtr->State = DATA_STATE_UNITIALIZED;
	return 0;
}
data_status_t	DATA_GetNextNodeID(data_t* dataPtr, uint32_t* nextNodeID){
	if(dataPtr == NULL) return DATA_ERROR;
	if((dataPtr->Path->currentIDPossition + 1) == dataPtr->Path->numberOfNodesOnPath){
		//there is not next node on the path, return current node
		*nextNodeID = dataPtr->Path->currentID;
	}
	else{
		//there is next node on the path, return its ID
		*nextNodeID = dataPtr->Path->line[dataPtr->Path->currentIDPossition +1];
	}
	return DATA_OK;
}

data_status_t	DATA_SetNode(data_t* dataPtr, uint32_t nodeID){
	if(dataPtr == NULL) return DATA_ERROR;
	//try to find nodeID on data path
	uint32_t counter = 0;
	while(dataPtr->Path->line[counter] != nodeID){
		if((counter+1) ==  dataPtr->Path->numberOfNodesOnPath) return DATA_ERROR;
		counter++;
	}
	dataPtr->Path->currentIDPossition = counter;
	dataPtr->Path->currentID = nodeID;
	return DATA_OK;
}


