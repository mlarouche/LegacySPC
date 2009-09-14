/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright (c) 2007 by Michaël Larouche <larouche@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "memorymap.h"

// LegacySPC includes
#include "spccomponentmanager.h"
#include "ram.h"
#include "legacyspc_debug.h"

namespace LegacySPC
{

class MemoryMap::Private
{
public:
	Private()
	 : componentManager(0)
	{}
	
	SpcComponentManager *componentManager;
};

MemoryMap::MemoryMap(SpcComponentManager *manager)
 : d(new Private)
{
	d->componentManager = manager;
}

MemoryMap::~MemoryMap()
{
	delete d;
}

byte MemoryMap::readByte(word address) const
{
	// FIXME: Finish
	if( address <= 0xFFFF )
	{
		return d->componentManager->ram()->readByte(address);
	}

	return 0;
}

word MemoryMap::readWord(word address) const
{
	word result;

	result.setLowByte( readByte(address) );
	result.setHighByte( readByte(address+1) );

	return result;
}

void MemoryMap::writeByte(word address, byte value)
{
	// TODO: Finish
	d->componentManager->ram()->writeByte( address, value);
}

void MemoryMap::writeWord(word address, word value)
{
	writeByte(address, value.lowByte());
	writeByte(address, value.highByte());
}

void MemoryMap::writeBytes(word address, const std::vector<byte> &bytes)
{
	for(int i=0; i<bytes.size(); i++)
	{
		writeByte(address+i, bytes[i]);
	}
}

}
