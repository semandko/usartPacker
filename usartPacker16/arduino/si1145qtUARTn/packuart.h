/*
Semenenko Myk: semenenko_myk@ukr.net
*/

#ifndef _PACKUART_H_
#define _PACKUART_H_

#define quint8 unsigned char
#define quint16 unsigned short
#define qint32 int
#define quint32 unsigned int

#define NUMBER_POINTS 128 // number of points for data
#define UART_BUFF_LENGTH 16 // length of the UART packet

#define START_FRAME      0x3A
#define STOP_FRAME       0x0D
#define END_FRAME        0x0A
#define START_LUXMETER   14

const quint8 GY1145_Instruction = START_LUXMETER;

typedef enum
{
    Default_GY1145      = 0,
    LUXMETER_UV         = 1,
    LUXMETER_VIS        = 2,
    LUXMETER_IR         = 3,
    SIZE_GY1145
} BaseGY1145_TypeDef;

typedef struct
{
    float       divider;
    float       dividerADC;
    float       value;
    float       valuemeasured;
    float       coeff;

    quint32    code;
    quint32    MSB;
    quint32    bitlength;
    qint32     output;
    quint32    maxCode;
    quint32    Alignment;
    quint8     offset;
    quint8     sign;

} AnalogTypeDef;

typedef struct
{
    quint8    index;
    quint32   msb;
    quint32   bitlength;
    quint8    offset; // 2 bytes length of the word
    quint32   maxCode;
    quint32   Alignment;
    float     dividerADC;
    float     coeff;

} DefineConfigPack;

const DefineConfigPack ConfigPakage[] =
{
    //quint8        //const quint32*    //quint32   //quint8   //quint32    //quint32  //float             //float
    //index              MSB            bitlength   offset      maxCode     Alignment   divider for ADC     coeff
    {LUXMETER_UV,   	 32768,         16,         1,         4095,       4095,        1,      			1},
    {LUXMETER_VIS,       32768,         16,         2,         4095,       4095,        1,      			1},
    {LUXMETER_IR,        32768,         16,         3,         4095,       4095,        1,      			1}
};

//packing for 2 bytes by MSB
//                             0      1      2      3      4      5      6      7      8      9     10     11     12     13     14     15     16     17 */
const quint32 pack_power[] = {0x0000,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000,0x8000};
const quint32 pack_mask[] =  {0x0000,0x0001,0x0003,0x0007,0x000f,0x001f,0x003f,0x007f,0x00ff,0x01ff,0x03ff,0x07ff,0x0fff,0x1fff,0x3fff,0x7fff,0xffff};

class PackUART
{
public:

    explicit PackUART();
    virtual ~PackUART();

    virtual void packConfig(void* p_as);
    virtual void packInstruction(const quint8 *p_instruction, void* p_Buff);
    virtual void pack(void* p_as, void* p_Buff);
    virtual void unpack(void* p_as, void* p_Buff);
    virtual void packBuffer (void* p_as, void* p_TXBuff, void* p_dataBuff);
    virtual void unpackBuffer (void* p_as, void* p_RXBuff, void* p_dataBuff);

    const DefineConfigPack* p_Config;
    float value;
    quint16* buff;
    quint8* index;
    quint16* dataBuff;
    AnalogTypeDef* as;
};

#endif // PACKUART_H
