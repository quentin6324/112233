
#include "iot_adc.h"

#define SMOKE_ADC_CHANNEL ADC_CHANNEL_0

uint16_t MQ2_ReadData() {
    return IoTAdcRead(SMOKE_ADC_CHANNEL);
}
