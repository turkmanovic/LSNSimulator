/** @file 	Data.h
 *  @brief  Data Network's part declarations
 *
 *   This file contains data and function declarations which are used in Data processing over the
 *   network. Data is one of the network parts and it is created to store information which represents
 *   data exchanged between Nodes in network. Data travel between nodes based on Path defined by user in
 *   Topology configuration file. Each Data has assigned protocols which defined properties of communication.
 *   Protocols properties are defined in Protocol configuration file.
 *
 *	@author Haris Turkmanovic
 *	@date   August 2019.
 * */

#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>

typedef enum{
	DATA_OK,
	DATA_ERROR
}data_status_t;
 /**
 * Boolean data type.
 * This enum define Boolean types: True and False. *S
 */
typedef enum {
	True,
	False
}Boolean;

/**
* Data States.
*/
typedef enum {
	DATA_STATE_UNITIALIZED,		/*!< Data is created but not initialized */
	DATA_STATE_CREATED,			/*!< Data is created and initialize */
	DATA_STATE_WAIT,			/*!< Data wait to be processed on node */
	DATA_STATE_RECEIVE_START,	/*!< Data is received on node */
	DATA_STATE_RECEIVE_END,		/*!< Data is received on node */
	DATA_STATE_PROCESS_START,		/*!< The data is being processed on node */
	DATA_STATE_PROCESS_END,		/*!< The data is being processed on node */
	DATA_STATE_TRANSMIT_START,	/*!< Data is sent from node */
	DATA_STATE_TRANSMIT_END,	/*!< Data is sent from node */
	DATA_STATE_TRANSMIT_SUSPENDED,	/*!< Data is sent from node */
	DATA_STATE_TRANSMIT_CONTINUE,	/*!< Data is sent from node */
	DATA_STATE_CONSUMED,		/*!< Data arrived on destination */
	DATA_STATE_DISCARDED		/*!< Data dsicarded on destination */
}data_state_t;

/**
* Data Retransmission States.
*/
typedef enum {
	RETRANSMISSION_STATE_UNITIALIZED,		/*!< Default state, retransmission not used */
	RETRANSMISSION_STATE_SCHEDULED,			/*!< Data is created and initialized */
	RETRANSMISSION_STATE_STARTED,			/*!< Data wait to be processed on node */
	RETRANSMISSION_STATE_CANCELED,			/*!< Data is received on node */
}data_retransmission_state_t;

/**
* Data Type.
*
* This Enum defines two possible types of data. Data type depends on data protocol.
* Once created, the data is initialized as a request. If protocol assigned to data
* create response, data type is changed to Response
*/
typedef enum {
	DATA_TYPE_REQUEST,		/*!< Data content represents protocol request to destination node 	*/
	DATA_TYPE_RESPONSE		/*!< Data content is response from destination node 	*/
}data_type_t;
/**
 * Data Aggregation type.
 *
 * This Enum defines either data present aggregated data or not
 */
 typedef enum {
 	DATA_AGG_FULL,		/*!< Data is aggregated */
 	DATA_AGG_RAW		/*!< Data isn't aggregated*/
 }data_type_aggregation_t;

/**
* Define data path to the destination Node.
*
* This structure contains all the information necessary to reach the destination.
*/
typedef struct {
	uint32_t 	destinationID; 		/*!< Destination Node ID	*/
	uint32_t 	currentID; 			/*!< ID value of current node on which the data is processed*/
	uint32_t	currentIDPossition;	/*!< Position of currentID in line array */
	uint32_t* 	line;				/*!< This is array which store Node's ID on data path*/
	uint32_t	numberOfNodesOnPath;/*!< Number of nodes on data path including node which produce data*/
}data_path_t;

/**
* Define protocol assigned to Data.
*
* This structure contains all the information necessary to established protocol.
*/
typedef struct {
	uint32_t 	ID;					/*!< Protocol ID defined by user in configuration files*/
	uint32_t 	Overhead;			/*!< Protocol Overhead	*/
	Boolean 	Response;       	/*!< This flag indicates whether the protocol generates a response
	 	 	 	 	 	 	 	 	 when the data arrives at its destination or not 	*/
	char 		ProtocolName[30];	/*!< Protocol name*/
}protocol_t;
//
struct data_t;
//
typedef struct{
	uint32_t		numberOfData;		/*!<Number of data in aggregation list*/
	struct data_t**	dataList;			/*!<Pointers to all data which are aggregated*/
}data_aggregation_list_t;
/**
* Define information.
*
* This structure contains all the information necessary to process data over the network.
*/
typedef struct data_t{
	uint32_t 		ID;						/*!< Generic data ID	*/
	uint32_t 		Size;					/*!< Data size in Bytes	*/
	uint32_t 		Overhead;			    /*!< Data Overhead	*/
	uint32_t 		BytesToProcess;			/*!< The number of bytes that will be processed on the node */
	double 			ElapsedRequestTime; 	/*!< The elapsed time from the moment the data was created until the data arrived at the destination node.
	 	 	 	 	 	 	 	 	 	 	 	 This value is never zero */
	double 			ElapsedResponseTime;	/*!< The elapsed time from the moment the data was created until the response arrived at the source node.
	 	 	 	 	 	 	 	 	 	 	 	 This value may be zero if Data type is request */
	double    		CreatedTime;
	double			EnergyToProcessData;    /*!< Energy required to process data on last node (not real energy-unit is mA per time unit)*/
	double			EnergyToTransmitData;	/*!< Energy required to transmit data on last node (not real energy-unit is mA per time unit) */
	double			EnergyToReceiveData;	/*!< Energy required to receive data on next node (not real energy-unit is mA per time unit) */
	data_path_t* 	Path;					/*!< Store information about the data path  */
	data_state_t 	State;					/*!< Current data state */
	data_type_t  	Type;					/*!< Current data type */
	protocol_t* 	AssignedProtocol;		/*!< Protocol assigned to data */
	double		linkTransmitConsumption;/*!< This information is used during data transmit to determine which link is used to send data*/
	double		linkReceiveConsumption; /*!< This information is used during data receie to determine which link is used to send data*/
	data_type_aggregation_t		aggregationFlag;
	data_aggregation_list_t*	aggreationList;
	uint8_t 		overheadProccesFlag; 			/*!< Indicate when data is overhead */
	data_path_t* 	finalPath;					/*!< Used in case of aggregation data */
	uint8_t			dataChangedToRow;

	uint32_t		numOfRetransmits;		/*!< How many times data has been retransmitted */
	data_retransmission_state_t	 RetState;  /*!< State of data to be retransmitted */
}data_t;

/**
* @brief Initialize Data part of system
* @return 0 - Initialization success
* @return 1 - Initialization failed
*/
uint8_t 		DATA_Init();
/**
* @brief Create DataPath for Data
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
data_path_t* 	DATA_PATH_Create(uint32_t DestinationId, uint32_t* PathLine);
/**
* @brief Create DataPath from current position until the end of path
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
data_path_t* 	DATA_PATH_CreateEndPath(data_path_t* path);
/**
* @brief Create DataPath from current position until the end of path
* @param DestinationId 	- Destination Node ID
* @param PathLine 		- Array which stores Nodes id on data path
* @return DataPath 		- DataPath is successfully created
* @return NULL 			- DataPath creation fault
*/
uint8_t 	DATA_PATH_AddNextNode(data_path_t* path, uint32_t nextNodeId);
/**
* @brief Create Raw Data structure and initialize it for use
* @param Size		 	- Data size in bytes
* @param Path 			- Previously created DataPath
* @param ProtocolId 	- Protocol ID value assigned to data
* @return Data* 		- Return pointer to data if creation was successful
* @return NULL 			- If creation wasn't successful
*/
data_t* 		DATA_RAW_Create(uint32_t Size, data_path_t* Path, uint32_t ProtocolID);
/**
* @brief Create tdentical copy of data
* @param DataToCopyPtr 	- Pointer to data to be copied
* @return Data* 		- Return pointer to copy of data
* @return NULL 			- If copying wasn't successful
*/
data_t*			DATA_RAW_CreateCopy(data_t* DataToCopyPtr);
/**
* @brief Create Aggregation Data structure and initialize it for use
* @param Path 			- Previously created DataPath to the next node
* @param ProtocolId 	- Protocol ID value assigned to data
* @param time 			- Timestamp when aggregation data is created
* @param FinalPath		- Path to final node in aggregation data
* @return Data* 		- Return pointer to data if creation was successful
* @return NULL 			- If creation wasn't successful
*/
data_t* 		DATA_AGG_Create(data_path_t* Path, uint32_t ProtocolID, data_path_t* FinalPath,double time);
/**
* @brief Add Data to aggregation list
* @param  rootData		- Pointer to aggregation data structure
* @param  chilldData	- Pointer to data which wants to be aggregate
* @return 0 			- Data is successfully added to aggregation list
* @return 1 			- Data cant'b added to aggregation list because path is not well
* @return 2 			- Data cant'b added to aggregation list because protocol is not well
* @return 3 			- There is a error while adding data to aggregation list
*/
uint8_t 		DATA_AGG_AddData(data_t* rootData, data_t* childData);
/**
* @brief Remove Data from aggregation list
* @param  rootData		- Pointer to aggregation data structure
* @param  chilldData	- Pointer to data which wants to be remove from aggregate list
* @return 0 			- Data is successfully removed from aggregation list
* @return 1 			- Data isn't in aggregation list
* @return 2 			- There is a error while removing data to aggregation list
*/
uint8_t 		DATA_AGG_RemoveData(data_t* rootData, data_t* childData);
/**
* @brief Remove Data from aggregation list
* @param  rootData		- Pointer to aggregation data structure
* @param  chilldData	- Pointer to data which wants to be remove from aggregate list
* @return 0 			- Data is successfully removed from aggregation list
* @return 1 			- Data isn't in aggregation list
* @return 2 			- There is a error while removing data to aggregation list
*/
uint8_t 		DATA_AGG_AddNextNodeOnPath(data_t* data, uint32_t nodeID);
/**
* @brief Remove all data from aggregation list
* @param  rootData		- Pointer to aggregation data structure
* @return 0 			- Data is successfully removed from aggregation list
* @return 1 			- Data isn't in aggregation list
* @return 2 			- There is a error while removing data to aggregation list
*/
uint8_t 		DATA_AGG_RemoveAllData(data_t* rootData);
/**
* @brief Change data to raw.
*
* 		 Before calling this function be sure that all aggregated data are removed
* 		 from the list by using @func DATA_AGG_RemoveAllData function
*
* @param  rootData		- Pointer to aggregation data structure
* @return 0 			- Aggregated data is successfully changed to RAW data
* @return 1 			- There is a error while changing data agg type from full agregated to raw
*/
uint8_t 		DATA_AGG_ChangeToRAW(data_t* rootData);
/**
* @brief Return protocol by ID value
* @param ProtocolID		- Protocol ID
* @return Protocol* 	- Return pointer to Protocol which ID is sent by parameter ProtocolID
* @return NULL 			- If there is no protocol with ID value define by ProtocolID
*/
protocol_t*   	DATA_GetProtocol(uint32_t ProtocolID);
/**
* @brief Check if Data require response and create corresponding response if it is necessary
* @param 	Data*			- Data to check
* @return 	0 				- Data require response and response is created
* @return	1 				- Data does not require response
*/
uint8_t			DATA_CreateResponse(data_t* DataPtr,uint32_t ResponseSize, double time);

data_status_t	DATA_GetNextNodeID(data_t* dataPtr, uint32_t* nextNodeID);

data_status_t	DATA_SetNode(data_t* dataPtr, uint32_t nodeID);

#endif /* DATA_H_ */
