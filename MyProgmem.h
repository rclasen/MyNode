#ifndef MyProgmem_h
#define MyProgmem_h

// TODO: include header that defines __FlashStringHelper

#define PGMT( pgm_ptr ) \
	(reinterpret_cast< const __FlashStringHelper * >( pgm_ptr ))

#endif


