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
#include "spcfilememoryloader.h"

// LegacySPC includes
#include "spcfile.h"
#include "spcfileloader.h"
#include "spccomponentmanager.h"
#include "processor.h"
#include "memorymap.h"
#include "spcrunner.h"

namespace LegacySPC
{

class SpcFileMemoryLoader::Private
{
public:
	Private()
	 : componentManager(0)
	{}

	SpcComponentManager *componentManager;
};

SpcFileMemoryLoader::SpcFileMemoryLoader(SpcComponentManager *manager)
 : d(new Private)
{
	d->componentManager = manager;
}

SpcFileMemoryLoader::~SpcFileMemoryLoader()
{
	delete d;
}

SpcComponentManager* SpcFileMemoryLoader::component() const
{
	return d->componentManager;
}

bool SpcFileMemoryLoader::loadSpcFile(const std::string &filename)
{
	SpcFileLoader loader(filename);
	if( !loader )
	{
		return false;
	}

	SpcFile fileToLoad = loader.spcFile();

	// Load CPU registers
	component()->processor()->registers()->loadRegisters( fileToLoad.processorRegisters() );

	// Load RAM data FIXME: Use MemoryMap or Ram directly ?
	component()->runner()->memory()->writeBytes( 0, fileToLoad.ramData() );

	// TODO: Load DSP registers

	return true;
}

}
