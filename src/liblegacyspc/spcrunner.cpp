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
#include "spcrunner.h"
 
// LegacySPC includes
#include "memorymap.h"
#include "spccomponentmanager.h"
#include "spcfilememoryloader.h"

namespace LegacySPC
{

class SpcRunner::Private
{
public:
	Private(SpcRunner *parent)
	 : memory(0), componentManager(0)
	{
		componentManager = new SpcComponentManager(parent);

		memory = new MemoryMap(componentManager);
	}

	~Private()
	{
		delete componentManager;
		delete memory;
	}
	
	MemoryMap *memory;
	SpcComponentManager *componentManager;
};

SpcRunner::SpcRunner()
 : d(new Private(this))
{
}

SpcRunner::~SpcRunner()
{
	delete d;
}

MemoryMap* SpcRunner::memory() const
{
	return d->memory;
}

bool SpcRunner::loadSpcFile(const std::string &filename)
{
	SpcFileMemoryLoader loader(d->componentManager);

	return loader.loadSpcFile(filename);
}

bool SpcRunner::run()
{
	// TODO
	return false;
}

SpcComponentManager *SpcRunner::componentManager() const
{
	return d->componentManager;
}

}
