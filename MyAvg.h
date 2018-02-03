#ifndef MyAvg_h
#define MyAvg_h

#include "MyAssert.h"
#include "MyProgmem.h"

static const char ALOC[] PROGMEM = "MyAvg.h";
#define ASSERT(e) myassert(PGMT(ALOC), e );

// no class as this allows allocating custom sized lists
// without need for remembering list address and size.
// i.e. they're just needed during the lifetime of the function calls

template <typename T>
void MyAvgAdd( T *list, T val, const uint8_t size, uint8_t *have, uint8_t *next )
{
	list[*next] = val;

	++(*next) %= size;
	if( *have < size )
		++(*have);
}

template <typename T>
T MyAvgCalc( T *list, const uint8_t size, const uint8_t have, const uint8_t next )
{
	if( ! have )
		return 0;

	T sum = 0;
	for( uint8_t i = have; i > 0; --i ){
		uint8_t x = (size + next - i) % size;

		// TODO: may overflow T... but can't divide by
		// _have, now as this messes up averaging of
		// small integers
		sum += list[x];
	}

	return sum / have;
}

// and as class:

// TODO: consider define for static list allocation

template <class T, class S>
class MyAvg {
private:
	T *_list;
	uint8_t _size, _have, _next;

public:
	MyAvg( uint8_t size = 5 )
	{
		_list = new T[size];
		ASSERT(_list);

		_size = size;
		_have = 0;
		_next = 0;
	};

	~MyAvg()
	{
		delete _list;
	};

	uint8_t size( void ) { return _size; }
	uint8_t have( void ) { return _have; }

	void add( T val )
	{
		_list[_next] = val;

		++_next %= _size;
		if( _have < _size )
			++_have;
	};

	T calc( uint8_t want = UINT8_MAX )
	{
		// TODO: handle want=0 myself

		if( ! _have )
			return 0;

		if( want > _have )
			want = _have;

		S sum = 0;
		for( uint8_t i = want; i > 0; --i ){
			uint8_t x = (_size + _next - i) % _size;

			// TODO: may overflow T... but can't divide by
			// want, now as this messes up averaging of
			// small integers
			sum += _list[x];
		}

		return sum / want;
	};
};

#endif
