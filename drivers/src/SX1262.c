#include <stdint.h>
#include "../inc/SX1262.h"
#include "../inc/spi.h"
#include "../inc/gpio.h"
//If not in STDBY_RC mode, then go to this mode with command SetStandby(...)

//Define the protocol (LoRa or FSK) with command SetPacketType(...)

//Define the RF frequency with command SetRfFrequency(...)

//Define the Power Amplifier configuration with command SetPaConfig(...)

//Define output power and ramping time with command SetTxParams(...)

//Define where the data payload is stored with command SetBufferBaseAddress(...)

//Send the payload to the data buffer with command WriteBuffer(...)

//Define the modulation parameter according to the chosen protocol with command SetModulationParams(...)1

//Define the frame format to be used with command SetPacketParams(...)2

//Configure DIO and IRQ: use command SetDioIrqParams(...) to select TxDone IRQ and map this IRQ to a DIO (DIO1, DIO2 or DIO3)

//Define Sync Word value: use command WriteReg(...) to write the value of the register via direct register access

//Set the circuit in transmitter mode to start transmission with command SetTx(). Use the parameter to enable Timeout

//Wait for the IRQ TxDone or Timeout: once the packet has been sent the chip goes automatically to STDBY_RC mode

//Clear the IRQ TxDone flag

void SX126x_Reset(void) 
{
    GPIO_Write(GPIOA, 0, 0);              
    SysTick_delay(1);                     
    GPIO_Write(GPIOA, 0, 1);             
    while (GPIO_Read(GPIOB, 3) == 1);     
}


void SX126x_Init() {

    SX126x_Reset();
    
    //Set Standby
    uint8_t standbyMode = 0x01; // Device running on XTAL 32MHz, set STDBY_XOSC mode
    SX126x_SendCommand(0x80, &standbyMode, 1);

    //Define Protocol: LoRa 
    uint8_t packetType = 0x01;
    SX126x_SendCommand(0x8A, &packetType, 1); //SetPacketType

    //Set Frequency
    uint32_t freq = 0x39300000;
    uint8_t cmd[4];
    cmd[0] = (freq >> 24) & 0xFF;       // MSB
    cmd[1] = (freq >> 16) & 0xFF;
    cmd[2] = (freq >> 8)  & 0xFF;
    cmd[3] = freq & 0xFF;
    SX126x_SendCommand(0x86, cmd, 4); //SetPacketType

    //Define the Power Amplifier
    uint8_t paConfig[4] = {0x04, 0x07, 0x00, 0x01}; // PA config: RFO, 0dBm, 1.8V
    SX126x_SendCommand(0x95 , paConfig, 1);

    //Set TxParams
    //investigate Ramptime more
    uint8_t TxConfig[2] = {0x16, 0x04}; // Tx config: power, ramptime
    SX126x_SendCommand(0x8E , TxConfig, 2);

    //Set bufferAddress
    //need help with values for this one
    uint8_t BufferAdressConfig[2] = {0x00, 0x00}; // Tx config: power, ramptime
    SX126x_SendCommand(0x8E , BufferAdressConfig, 2); 
}

void SX126x_SetTx(uint8_t* data, uint8_t length) {

    // post initialize 

    //Write Buffer Address
    uint8_t payload[1 + length];
    payload[0] = 0x00; // offset, matches TX base address
    for (int i = 0; i < length; i++) payload[1 + i] = data[i]; 
    SX126x_SendCommand(0x0E , payload, 1 + length); 

    //Write Modulation Params
    uint8_t modulationParams[4];
    modulationParams[0] = 0x0C ; // Spreading Factor: SF12
    modulationParams[1] = 0x04 ; // Bandwidth: 125 kHz
    modulationParams[2] = 0x04 ; // Coding Rate: 4/5
    modulationParams[3] = 0x01 ; // Low Data Rate Optimization: enabled
    SX126x_SendCommand(0x8B , modulationParams, 4); 


    //set packet params
    //to be reviewed
    uint8_t packetParams[7];
    packetParams[0] = 0x00 ; // Preamble Length: 8 symbols
    packetParams[1] = 0x08 ; // Preamble Length: 8 symbols
    packetParams[2] = 0x00 ; // Header Type: Explicit
    packetParams[3] = length ; // Payload Length: 4 bytes
    packetParams[4] = 0x01 ; // CRC: Disabled
    packetParams[5] = 0x00 ; // Invert IQ: Disabled
    SX126x_SendCommand(0x8C, packetParams, 6); //SetPacketParams


    //Configure DIO and IRQ
    uint8_t dioIrqParams[8] = {
        0x00, 0x01,  // IrqMask: TxDone bit set
        0x00, 0x01,  // DIO1Mask: route TxDone to DIO1
        0x00, 0x00,  // DIO2Mask: unused
        0x00, 0x00   // DIO3Mask: unused
    };
    SX126x_SendCommand(0x08, dioIrqParams, 8);

    //write register 
    uint8_t syncWord[4] = {
        0x07, 0x40,   // register address (MSB, LSB) — 0x0740, the sync word MSB register
        0x14, 0x24    // data: private network sync word (matches your reset default)
    };
    SX126x_SendCommand(0x0D, syncWord, 4);


    //SetTx
    uint8_t timeout[3] = {0x00, 0x00, 0x00}; // Timeout: 0 ms
    SX126x_SendCommand(0x83, timeout, 3); //SetTx


    //Check for flag
    while (GPIO_Read(GPIOB, 4) == 0); // wait for DIO1 (TxDone) to go high

    //clear flag
    uint8_t clearIrq[2] = {0x00, 0x01}; // clear TxDone bit — confirm this matches your confirmed bit position
    SX126x_SendCommand(0x02, clearIrq, 2); // confirm 0x02 is really ClearIrqStatus's opcode in your datasheet












}


void SX126x_SendCommand(uint8_t opcode, uint8_t* params, uint8_t param_len) {

    while(GPIO_Read(GPIOB, 3) == 1); // wait BUSY low
    CS_Select(GPIOA, 8);          // NSS low
    uint8_t status;
    SPI_transmit(SPI1, &opcode, 1);   // send opcode, discard the garbage byte back
    
    if(param_len > 0) {
        SPI_transmit(SPI1, params, param_len); // send parameters
    } 

    CS_Deselect(GPIOA, 8); // NSS high         


}