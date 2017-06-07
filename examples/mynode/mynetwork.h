/*
 * shared settings for my local mysensors network
 *
 * changes to this file likely require rebuild/update of all sensors
 *
 */

#ifndef MYNETWORK_H
#define MYNETWORK_H

/*******************************
 * Network wide settings:
 */

// radio
#define MY_RADIO_NRF24

// network - for now use defaults to match OTA bootloader
//#define MY_RF24_CHANNE6 76
//#define MY_RF24_BASE_RADIO_ID 0x00,0xFC,0xE1,0xA8,0xA9

// signing (+600 bytes RAM with atsha)
#define MY_SIGNING_REQUEST_SIGNATURES
#define MY_SIGNING_GW_REQUEST_SIGNATURES_FROM_ALL

// ... use sw backend (+100 bytes RAM)
// move this to the sketch if you need to pick the backend by sketch
#define MY_SIGNING_SOFT
//#define MY_SIGNING_SOFT_RANDOMSEED_PIN (7)

// no encryption as it fails with OTA bootloader
//#undef MY_RF24_ENABLE_ENCRYPTION

#endif
