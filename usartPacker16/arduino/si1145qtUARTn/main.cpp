#include <iostream>
#include "packuart.h"


using namespace std;

AnalogTypeDef AS_GY1145[SIZE_GY1145];
quint8 DataTX[UART_BUFF_LENGTH];
quint8 DataRX[UART_BUFF_LENGTH];
unsigned char BuffRX[UART_BUFF_LENGTH];

float UV = 125;
float VIS = 1245;
float IR = 7453;


int main()
{
    cout << "Luxmeter GY1145" << endl;

    PackUART LX_GY1145;

    LX_GY1145.packConfig(AS_GY1145);
    LX_GY1145.packInstruction(&GY1145_Instruction, DataTX);

    AS_GY1145[LUXMETER_UV].value = UV;
    AS_GY1145[LUXMETER_VIS].value = VIS;
    AS_GY1145[LUXMETER_IR].value = IR;

    LX_GY1145.pack(&AS_GY1145[LUXMETER_UV], DataTX);
    LX_GY1145.pack(&AS_GY1145[LUXMETER_VIS], DataTX);
    LX_GY1145.pack(&AS_GY1145[LUXMETER_IR], DataTX);

    LX_GY1145.unpack(&AS_GY1145[LUXMETER_UV], DataTX);
    LX_GY1145.unpack(&AS_GY1145[LUXMETER_VIS], DataTX);
    LX_GY1145.unpack(&AS_GY1145[LUXMETER_IR], DataTX);

    for(unsigned int i = 0; i < UART_BUFF_LENGTH; i++)
    {
        BuffRX[i] = (char)DataTX[i];
    }

    system("pause");
    return 0;
}
