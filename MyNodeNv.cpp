#include "MyNodeNv.h"

// hwWrite... only update changed bytes

void MyNodeNvSet( uint8_t *pos, uint8_t val )
{
	hwWriteConfig( pos, val );
}

uint8_t MyNodeNvGet( uint8_t *pos )
{
	return hwReadConfig( pos );
}

void MyNodeNvSetBlock( uint8_t *pos, const uint8_t *buf, size_t len )
{
	hwWriteConfigBlock( buf, pos, len );
}

void MyNodeNvGetBlock( uint8_t *pos, uint8_t *buf, uint8_t len )
{
	hwReadConfigBlock( buf, pos, len );
}


void MyNodeNvSetPgm( uint8_t *pos, const __FlashStringHelper *buf, size_t len )
{
	for( size_t i = 0; i < len; ++i ){
		MyNodeNvSet( pos+i, pgm_read_byte_near( (uint8_t*)buf + i) );
	}
}

void MyNodeNvSet16( uint8_t *pos, uint16_t val )
{
	MyNodeNvSetBlock( pos, (uint8_t*)&val, 2 );
}

uint16_t MyNodeNvGet16( uint8_t *pos )
{
	uint16_t val = 0;

	MyNodeNvGetBlock( pos, (uint8_t*)&val, 2 );
	return val;
}


