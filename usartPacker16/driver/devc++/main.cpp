#include <iostream>
#include "packuart.h"




using namespace std;

// measurement data
float UVindex;
float VIS;
float IR;

PackUART Sensor;

AnalogTypeDef SygnalSensor[SIZE_OF_SYGNALS]; // number of sygnals

quint8 DataTX[BUFF_LENGTH]; // TX buffer of 16 bytes


int main(int argc, char** argv) {
	
		//
	Sensor.packInstruction(Instruction, DataTX);
	Sensor.packConfig(SygnalSensor);
	
	//
	VIS = 12000;
	IR = 15000;
	UVindex = 10; 
	
	cout << VIS << endl;
	cout << IR << endl;
	cout << UVindex << endl;
  
    
    SygnalSensor[LUXMETER_VIS].value = VIS;
    SygnalSensor[LUXMETER_IR].value = IR;
    SygnalSensor[LUXMETER_UV].value = UVindex;
    

    Sensor.pack(SygnalSensor[LUXMETER_UV], DataTX);
    Sensor.pack(SygnalSensor[LUXMETER_VIS], DataTX);
    Sensor.pack(SygnalSensor[LUXMETER_IR], DataTX);
  
	for (uint8_t i = 0; i < BUFF_LENGTH; i++ ){
		cout << i << DataTX[i] << endl;
		// usart send
	}
	
	system("pause");
	return 0;
}

