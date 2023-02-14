/*************************************************** 
  This is a library for the Si1145 UV/IR/Visible Light Sensor

  Designed specifically to work with the Si1145 sensor in the
  adafruit shop
  ----> https://www.adafruit.com/products/1777

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_SI1145.h"

#include "packuart.h"

Adafruit_SI1145 luxmeter = Adafruit_SI1145();
float UVindex;
float VIS;
float IR;

PackUART LX_GY1145 = PackUART();
AnalogTypeDef AS_GY1145[SIZE_GY1145];
quint8 DataTX[UART_BUFF_LENGTH]; // 16 bytes

void setup() {

  Serial.begin(9600);
  Serial.println("Adafruit SI1145 test");
  
  if (!luxmeter.begin())
  {
    Serial.println("Didn't find Si1145");
    while (1);
  }

  Serial.println("OK!");
  LX_GY1145.packConfig(AS_GY1145);
  LX_GY1145.packInstruction(&GY1145_Instruction, DataTX);
  Serial.println("Luxmeter GY1145");
}

void loop() 
{
	VIS = luxmeter.readVisible();
	IR = luxmeter.readIR();
	UVindex = luxmeter.readUV(); 
	UVindex /= 100.0; // the index is multiplied by 100 so to get the 
  
    AS_GY1145[LUXMETER_UV].value = UVindex;
    AS_GY1145[LUXMETER_VIS].value = VIS;
    AS_GY1145[LUXMETER_IR].value = IR;

    LX_GY1145.pack(&AS_GY1145[LUXMETER_UV], DataTX);
    LX_GY1145.pack(&AS_GY1145[LUXMETER_VIS], DataTX);
    LX_GY1145.pack(&AS_GY1145[LUXMETER_IR], DataTX);
  
	for(uint8_t i = 0; i < UART_BUFF_LENGTH; i++ )
	{
		Serial.println(DataTX[i]);
	}
	
	delay(1000);
}
