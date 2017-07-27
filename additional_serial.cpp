#include <Arduino.h>
#include <SPI.h>
#include <wiring_private.h>
#include "additional_serial.h"


// Instantiate the extra Serial classes
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);
Uart Serial3(&sercom4, PIN_SERIAL3_RX, PIN_SERIAL2_TX, PAD_SERIAL3_RX, PAD_SERIAL3_TX);

void serial_setup()
{
  Serial.begin(57600);
  pinPeripheral(0, PIO_SERCOM);   // Assign pins 0 & 1 SERCOM functionality
  pinPeripheral(1, PIO_SERCOM);
  Serial2.begin(500000);           // Begin Serial2
  pinPeripheral(4, PIO_SERCOM_ALT);   // Assign pins 4 & 5 SERCOM functionality
  pinPeripheral(5, PIO_SERCOM_ALT);
  Serial3.begin(500000); // Begin Serial3
}

void serial_motor_controls(int motor_tag,
                           float DIR_PACK,
                           int serial_val)
{
  serial_val = abs(serial_val);
//  Serial.println(serial_val);
  Serial2.write((byte)0xAA); // recognize baud rate 
  Serial2.write((byte)motor_tag); // identify which motor to send packet to 
  Serial2.write((byte)DIR_PACK); // forwards or backwards 
  Serial2.write(serial_val & 0x1F); // speed packet 1
  Serial2.write(serial_val >> 5); // speed packet 2 
}


void SERCOM3_Handler()    // Interrupt handler for SERCOM3
{
  Serial2.IrqHandler();
}

void SERCOM4_Handler()    // Interrupt handler for SERCOM4
{
  Serial3.IrqHandler();
}

