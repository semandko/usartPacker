#include "packuart.h"

PackUART::PackUART()
{

}

PackUART::~PackUART()
{
    delete p_Config;
    delete buff;
    delete as;
    delete dataBuff;
}

void PackUART::packConfig(void* p_as)
{
    this->as = (AnalogTypeDef*)p_as;

    for(quint32 i = 0; i < sizeof(ConfigPakage)/sizeof(ConfigPakage[0]); i++)
    {
        this->p_Config = &ConfigPakage[i];

        this->as[this->p_Config->index].MSB         = this->p_Config->msb;       // quint32
        this->as[this->p_Config->index].bitlength   = this->p_Config->bitlength;  // quint32
        this->as[this->p_Config->index].offset      = this->p_Config->offset;     // quint8
        this->as[this->p_Config->index].maxCode     = this->p_Config->maxCode;   // quint32
        this->as[this->p_Config->index].Alignment   = this->p_Config->Alignment;  // quint32
        this->as[this->p_Config->index].dividerADC  = this->p_Config->dividerADC; // float
        this->as[this->p_Config->index].coeff       = this->p_Config->coeff;      // float
    }
}

void PackUART::packInstruction(const quint8* p_instruction, void* p_Buff)
{
	quint8 *buff;
	buff = (quint8*)p_Buff;
	
	buff[0] = START_FRAME;
	buff[1] = *p_instruction;
	buff[14] = STOP_FRAME;
	buff[15] = END_FRAME;
}

void PackUART::pack(void* p_as, void* p_Buff)
{
    this->buff = (quint16*)p_Buff;
    this->as = (AnalogTypeDef*)p_as;

    this->value = this->as->value;
    this->value = (this->value)*(pack_power[this->as->bitlength]/(this->as->MSB)); // Binary code transforming by MSB
    this->as->output = (quint16)this->value; // convert to two bytes
    this->as->output &= (pack_mask[this->as->bitlength]); // reduce the accuracy
    this->buff[this->as->offset] &= ~(pack_mask[this->as->bitlength]); // clearing the bits
    this->buff[this->as->offset] |= this->as->output; // put it in the result
}

void PackUART::unpack(void* p_as, void* p_Buff)
{
    this->buff = (quint16*)p_Buff;
    this->as = (AnalogTypeDef*)p_as;

    this->value = buff[this->as->offset];
    this->as->valuemeasured = (this->value) * (((float)this->as->MSB)/(pack_power[this->as->bitlength]));
    this->as->code = (quint32)(((this->as->valuemeasured)*(this->as->maxCode))/(2*(this->as->MSB)));
}

void PackUART::packBuffer(void* p_as, void* p_TXBuff, void* p_dataBuff)
{
    this->dataBuff = (quint16 *)p_dataBuff;
    this->as = (AnalogTypeDef *)p_as;

    this->buff = (quint16*)p_TXBuff; // step = 2 bytes
    this->index = (quint8*)p_TXBuff; // step = 1 byte for service command
    this->as = &this->as[this->index[2]]; // number of sygnal in the table

    for(quint8 i = 0; i < NUMBER_POINTS; i++)
    {
        this->as->value = ((float)(this->dataBuff[i]))*(this->as->coeff); // 1: take the data
        this->value = (this->as->value)*pack_power[this->as->bitlength]/(this->as->MSB); // 2: convert it to binary form by MSB
        this->as->output = (quint16)this->value; // 3: convert float to integer
        this->as->output &= (pack_mask[this->as->bitlength]); // 4: reduce accurracy of the sygnal
        this->buff[i+2] &= ~(pack_mask[this->as->bitlength]); // 5: clear the first 4 bytes for service commands
        this->buff[i+2] |= this->as->output; // 6: place converted data to the buffer
    }
}

void PackUART::unpackBuffer(void* p_as, void* p_RXBuff, void* p_dataBuff)
{
    this->dataBuff = (quint16*)p_dataBuff;
    this->as = (AnalogTypeDef*)p_as;

    this->buff = (quint16*)p_RXBuff; // step = 2 bytes
    this->index = (quint8*)p_RXBuff; // step = 1 byte for service command
    this->as = &this->as[this->index[2]]; // number of sygnal in the table

    for(quint8 i = 0; i < NUMBER_POINTS; i++)
    {
        this->value = this->buff[i+2]; // first 4 bytes are for service commands
        this->as->valuemeasured = (this->value)*(((float)this->as->MSB)/(pack_power[this->as->bitlength]));
        this->dataBuff[i] = (quint16)((this->as->valuemeasured) / (this->as->coeff));
    }
}
