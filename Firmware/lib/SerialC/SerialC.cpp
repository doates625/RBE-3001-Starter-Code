/**
 * @file SerialC.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <SerialC.h>

/**
 * Unions for interpreting multi-byte data types
 */

// 8-bit union
typedef union
{
	uint8_t bytes[1];
	int8_t int8;
	uint8_t uint8;
}
union8_t;

// 16-bit union
typedef union
{
	uint8_t bytes[2];
	int16_t int16;
	uint16_t uint16;
}
union16_t;

// 32-bit union
typedef union
{
	uint8_t bytes[4];
	int32_t int32;
	uint32_t uint32;
	float floating;
}
union32_t;

/**
 * @brief Class constructor.
 * @param serial Mbed serial interface
 */
SerialC::SerialC(Serial& serial)
{
	this->serial = &serial;
}

/**
 * @brief Reads int8 (1 byte) from serial buffer.
 */
int8_t SerialC::read_int8()
{
	union8_t union8;
	read_bytes(union8.bytes, 1);
	return union8.int8;
}

/**
 * @brief Reads int16 (2 bytes) from serial buffer.
 */
int16_t SerialC::read_int16()
{
	union16_t union16;
	read_bytes(union16.bytes, 2);
	return union16.int16;
}

/**
 * @brief Reads int32 (4 bytes) from serial buffer.
 */
int32_t SerialC::read_int32()
{
	union32_t union32;
	read_bytes(union32.bytes, 4);
	return union32.int32;
}

/**
 * @brief Reads uint8 (1 byte) from serial buffer.
 */
uint8_t SerialC::read_uint8()
{
	union8_t union8;
	read_bytes(union8.bytes, 1);
	return union8.uint8;
}

/**
 * @brief Reads uint16 (2 bytes) from serial buffer.
 */
uint16_t SerialC::read_uint16()
{
	union16_t union16;
	read_bytes(union16.bytes, 2);
	return union16.uint16;
}

/**
 * @brief Reads uint32 (4 bytes) from serial buffer.
 */
uint32_t SerialC::read_uint32()
{
	union32_t union32;
	read_bytes(union32.bytes, 4);
	return union32.uint32;
}

/**
 * @brief Reads float (4 bytes) from serial buffer.
 */
float SerialC::read_float()
{
	union32_t union32;
	read_bytes(union32.bytes, 4);
	return union32.floating;
}

/**
 * @brief Writes int8 (1 byte) to serial buffer.
 */
void SerialC::write_int8(int8_t val)
{
	union8_t union8;
	union8.int8 = val;
	write_bytes(union8.bytes, 1);
}

/**
 * @brief Writes int16 (2 bytes) to serial buffer.
 */
void SerialC::write_int16(int16_t val)
{
	union16_t union16;
	union16.int16 = val;
	write_bytes(union16.bytes, 2);
}

/**
 * @brief Writes int32 (4 bytes) to serial buffer.
 */
void SerialC::write_int32(int32_t val)
{
	union32_t union32;
	union32.int32 = val;
	write_bytes(union32.bytes, 4);
}

/**
 * @brief Writes uint8 (1 byte) to serial buffer.
 */
void SerialC::write_uint8(uint8_t val)
{
	union8_t union8;
	union8.uint8 = val;
	write_bytes(union8.bytes, 1);
}

/**
 * @brief Writes uint16 (2 bytes) to serial buffer.
 */
void SerialC::write_uint16(uint16_t val)
{
	union16_t union16;
	union16.uint16 = val;
	write_bytes(union16.bytes, 2);
}

/**
 * @brief Writes uint32 (4 bytes) to serial buffer.
 */
void SerialC::write_uint32(uint32_t val)
{
	union32_t union32;
	union32.uint32 = val;
	write_bytes(union32.bytes, 4);
}

/**
 * @brief Writes float (4 bytes) to serial buffer.
 */
void SerialC::write_float(float val)
{
	union32_t union32;
	union32.floating = val;
	write_bytes(union32.bytes, 4);
}

/**
 * @brief Reads n bytes from serial into buffer b.
 */
void SerialC::read_bytes(uint8_t* bytes, uint8_t n)
{
	for(uint8_t i = 0; i < n; i++)
	{
		while(!serial->readable());
		bytes[i] = serial->getc();
	}
}

/**
 * @brief Writes n bytes from b into serial buffer.
 */
void SerialC::write_bytes(uint8_t* bytes, uint8_t n)
{	
	for(uint8_t i = 0; i < n; i++)
	{
		while(!serial->writable());
		serial->putc(bytes[i]);
	}
}