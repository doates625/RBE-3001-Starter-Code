/**
 * @file SerialC.h
 * @brief Class for sending and receiving standard
 * C data types over serial.
 * 
 * Once a read function is called, the function will
 * wait indefinitely until a sufficient number of bytes
 * have been read from the serial buffer. The same goes
 * for write commands and the outgoing serial buffer.
 * 
 * Supported data types:
 * - Signed integers (int8_t, int16_t, int32_t)
 * - Unsigned integers (uint8_t, uint16_t, uint32_t)
 * - Floating-point (float)
 * 
 * @author Dan Oates (WPI Class of 2020)
 */
#include <mbed.h>

/**
 * Class Declaration
 */
class SerialC
{
public:
	SerialC(Serial& serial);
	int8_t read_int8();
	int16_t read_int16();
	int32_t read_int32();
	uint8_t read_uint8();
	uint16_t read_uint16();
	uint32_t read_uint32();
	float read_float();
	void write_int8(int8_t val);
	void write_int16(int16_t val);
	void write_int32(int32_t val);
	void write_uint8(uint8_t val);
	void write_uint16(uint16_t val);
	void write_uint32(uint32_t val);
	void write_float(float val);
private:
	void read_bytes(uint8_t* bytes, uint8_t n);
	void write_bytes(uint8_t* bytes, uint8_t n);
	Serial* serial;
};