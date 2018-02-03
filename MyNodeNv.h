#ifndef MyNodeNv_h
#define MyNodeNv_h

#include <core/MySensorsCore.h>
#include <core/MyArchitecture.h>

/************************************************************
 *
 * Non-volatile storage (eeprom / nvram)
 *
 ************************************************************/

#define MYNODE_NV_MAGIC		(EEPROM_LOCAL_CONFIG_ADDRESS + 0u)
#define MYNODE_NV_MAGIC_SIZE	(2u)

#define MYNODE_NV_INTREF	(MYNODE_NV_MAGIC + MYNODE_NV_MAGIC_SIZE)
#define MYNODE_NV_INTREF_SIZE	(2u)

#define MYNODE_NV_ITEMS		(MYNODE_NV_INTREF + MYNODE_NV_INTREF_SIZE)


void MyNodeNvSet( uint8_t *pos, uint8_t val );
uint8_t MyNodeNvGet( uint8_t *pos );

void MyNodeNvGetBlock( uint8_t *pos, uint8_t *buf, uint8_t len );
void MyNodeNvSetBlock( uint8_t *pos, const uint8_t *buf, size_t len );

void MyNodeNvSetPgm( uint8_t *pos, const __FlashStringHelper *buf, size_t len );

void MyNodeNvSet16( uint8_t *pos, uint16_t val );
uint16_t MyNodeNvGet16( uint8_t *pos );


#endif
