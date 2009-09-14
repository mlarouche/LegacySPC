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
#include "ram.h"

#include <legacyspc_debug.h>

namespace LegacySPC
{

class Ram::Private
{
public:
	// TODO: Remplace with array with something better
	// like a custom ByteArray class
	std::vector<byte> ramData;
};

Ram::Ram()
 : d(new Private)
{
	d->ramData.reserve(0xFFFF);
}

Ram::~Ram()
{
	delete d;
}

void Ram::loadRam(const std::vector<byte> &data)
{
	d->ramData = data;
}

byte Ram::readByte(word address)
{
	if( address <= 0xFFFF )
	{
		return d->ramData[static_cast<uint16>(address)];
	}
	else
	{
		// TODO: Error
		return 0;
	}
}

void Ram::writeByte(word address, byte value)
{
	d->ramData[static_cast<uint16>(address)] = value;
}

}
