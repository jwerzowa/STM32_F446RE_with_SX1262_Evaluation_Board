#ifndef SX1262_H
#define SX1262_H


//in future, will use enums to define the different modes, frequencies, and other parameters for the SX1262, rahter than hardcoding them in the functions.


void SX126x_Reset(void);
void SX126x_Init(void);                         
void SX126x_Transmit(uint8_t* data, uint8_t len); 
void SX126x_SendCommand(uint8_t opcode, uint8_t* params, uint8_t param_len);



#endif // SX1262_H