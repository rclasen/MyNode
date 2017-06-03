#include "MyNode.h"

/************************************************************
 * MyNode
 */

MyNode::MyNode( uint8_t itemc, const MyNodeItem *itemv )
{
	_itemc = itemc;
	_itemv = itemv;

	_childc = 0;
	for( uint8_t i = 0; i < _itemc; ++i ){
		uint8_t id = _itemv[i].getChildMax();

		if( id > _childc )
			_childc = id;
	}

	_childv = new uint8_t[_childc];
	for( uint8_t i = 0; i < _itemc; ++i ){
		uint8_t num = _itemv[i].getChildCount();

		for( uint8_t c = 0; c < num; ++c ){
			uint8_t id = _itemv[i].getChildId( num );
			_childv[id] = i;
		}
	}
}

MyNode::~MyNode()
{
	delete _childv;
}


void MyNode::before()
{
	for( uint8_t i = 0; i < _itemc; ++i )
		_itemv[i].before();
}

void MyNode::presentation( PGM_P name, PGM_P version )
{
	{
		char buf[MAX_PAYLOAD];

		strncpy_P(buf, name, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
		sendSketchInfo( buf, NULL );

		strncpy_P(buf, version, MAX_PAYLOAD );
		buf[MAX_PAYLOAD-1] = 0;
		sendSketchInfo( NULL, buf );
	}

	for( uint8_t i = 0; i < _itemc; ++i )
		_itemv[i].presentation();
}

void MyNode::setup()
{
	// TODO
}

void MyNode::loop()
{
	for( uint8_t i = 0; i < _itemc; ++i )
		_itemv[i].loop();
}

void MyNode::receive(const MyMessage & msg)
{
	// TODO
}

void MyNode::receiveTime(unsigned long ts)
{
	// TODO
}


