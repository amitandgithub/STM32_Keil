#ifndef __DS18B20_H__
#define __DS18B20_H__

//#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "one_wire.h"

// Structure in which temperature is stored
typedef struct {
	int8_t integer;
	uint16_t fractional;
	bool is_valid;
} simple_float;

// Structure for returning list of devices on one wire
typedef struct {
	uint8_t size;
	one_wire_device *devices;
} ds18b20_devices;

void ds18b20_init(GPIO_TypeDef *gpio, uint16_t port, TIM_TypeDef *timer);
void ds18b20_set_precision(uint8_t precision);
ds18b20_devices ds18b20_get_devices(bool scan);

void ds18b20_convert_temperature_simple(void);
simple_float ds18b20_read_temperature_simple(void);

void ds18b20_wait_for_conversion(void);
simple_float ds18b20_decode_temperature(void);

simple_float ds18b20_get_temperature_simple(void);

#endif // __DS18B20_H__
