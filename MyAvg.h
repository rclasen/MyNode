#ifndef MyAvg_h
#define MyAvg_h

#include "MyNodePanic.h"

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

template <class T>
class MyAvg {
private:
	T *_list;
	uint8_t _size, _have, _next;

public:
	MyAvg( uint8_t size = 5 )
	{
		_list = new T[size];
		if( ! _list ){
			MyNodePanic();
			return;
		}
		_size = size;
		_have = 0;
		_next = 0;
	};

	~MyAvg()
	{
		delete _list;
	};

	void add( T val )
	{
		_list[_next] = val;

		++_next %= _size;
		if( _have < _size )
			++_have;
	};

	T calc( void )
	{
		if( ! _have )
			return 0;

		T sum = 0;
		for( uint8_t i = _have; i > 0; --i ){
			uint8_t x = (_size + _next - i) % _size;

			// TODO: may overflow T... but can't divide by
			// _have, now as this messes up averaging of
			// small integers
			sum += _list[x];
		}

		return sum / _have;
	};
};

#endif
