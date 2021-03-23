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
	DATA_STATE_UNITIALIZED,	/*!< Data is created but not initialized */
	DATA_STATE_CREATED,		/*!< Data is created and initialize */
	DATA_STATE_WAIT,			/*!< Data wait to be processed on node */
	DATA_STATE_RECEIVED,		/*!< Data is received on node */
	DATA_STATE_PROCESSED,		/*!< The data is being processed on node */
	DATA_STATE_SENT,			/*!< Data is sent from node */
	DATA_STATE_CONSUMED		/*!< Data arrived on destination */
}data_state_t;

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
* Define data path to the destination Node.
*
* This structure contains all the information necessary to reach the destination.
*/
typedef struct {
	uint32_t 	DestinationID; 	/*!< Destination Node ID	*/
	uint32_t 	CurrentID; 		/*!< ID value of current node on which the data is processed*/
	uint32_t* 	Line;			/*!< This is array which store Node's ID on data path*/
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

/**
* Define information.
*
* This structure contains all the information necessary to process data over the network.
*/
typedef struct {
	uint32_t 	ID;						/*!< Generic data ID	*/
	uint32_t 	Size;					/*!< Data size in Bytes	*/
	uint32_t 	Overhead;			    /*!< Data Overhead	*/
	uint32_t 	BytesToProcess;			/*!< The number of bytes that will be processed on the node */
	double 	ElapsedRequestTime; 	/*!< The elapsed time from the moment the data was created until the data arrived at the destination node.
	 	 	 	 	 	 	 	 	 	 This value is never zero */
	double 	ElapsedResponseTime;	/*!< The elapsed time from the moment the data was created until the response arrived at the source node.
	 	 	 	 	 	 	 	 	 	 This value may be zero if Data type is request */
	double    	CreatedTime;
	double		EnergyToProcessData;    /*!< Energy required to process data on last node (not real energy-unit is mA per time unit)*/
	double		EnergyToTransmitData;	/*!< Energy required to transmit data on last node (not real energy-unit is mA per time unit) */
	double		EnergyToReceiveData;	/*!< Energy required to receive data on next node (not real energy-unit is mA per time unit) */
	data_path_t* 	Path;					/*!< Store information about the data path  */
	data_state_t 	State;					/*!< Current data state */
	data_type_t  	Type;					/*!< Current data type */
	protocol_t* 	AssignedProtocol;		/*!< Protocol assigned to data */
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
data_path_t* 	DATA_Create_Path(uint32_t DestinationId, uint32_t* PathLine);
/**
* @brief Create Data structure and initialize it for use
* @param Size		 	- Data size in bytes
* @param DataPath 		- Previously created DataPath
* @param ProtocolId 	- Protocol ID value assigned to data
* @return Data* 		- Return pointer to data if creation was successful
* @return NULL 			- If creation wasn't successful
*/
data_t* 		DATA_Create(uint32_t Size,data_path_t* Path,uint32_t ProtocolID);
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
uint8_t			DATA_CreateResponse(data_t* DataPtr,uint32_t ResponseSize);

#endif /* DATA_H_ */
