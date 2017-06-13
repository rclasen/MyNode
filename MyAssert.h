#ifndef MyAssert_h
#define MyAssert_h

#include <Arduino.h>
#include "MyProgmem.h"


/*
 * standard assert.h uses __func__ and __FILE__ which both aren't in
 * progmem with current gcc. This will hog up RAM.
 *
 * This implementation lacks these details but at least won't use RAM.
 *
 * Another problem is the lack of stderr/logging... so, you may want to
 * provide your own __myassert function to handle the assertions.
 */

#  if defined(NDEBUG)
#    define myassert(location,e)	((void)0)
#  else /* !NDEBUG */

#      define myassert(location,e)	((e) ? (void)0 : ( __myassert ? \
                         __myassert(location, __LINE__, F(#e)) : \
			 abort() ) )

#  endif /* NDEBUG */

void __myassert(const __FlashStringHelper *__location,
		int __lineno,
		const __FlashStringHelper *__sexp)
		__attribute__((weak));

// TODO; static assert

#endif
