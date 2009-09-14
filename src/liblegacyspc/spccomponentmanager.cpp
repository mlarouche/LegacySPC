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
#include "spccomponentmanager.h"

// LegacySPC includes
#include "ram.h"
#include "processor.h"
#include "spcrunner.h"

namespace LegacySPC
{

class SpcComponentManager::Private
{
public:
	Private(SpcRunner *runner)
	{
		this->runner = runner;

		processor = new Processor(runner);
		ram = new Ram;
	}
	~Private()
	{
		delete processor;
		delete ram;
	}

	Ram *ram;
	SpcRunner *runner;
	Processor *processor;
};

SpcComponentManager::SpcComponentManager(SpcRunner *runner)
 : d( new Private(runner) )
{
}

SpcComponentManager::~SpcComponentManager()
{
	delete d;
}

SpcRunner* SpcComponentManager::runner() const
{
	return d->runner;
}

Ram* SpcComponentManager::ram() const
{
	return d->ram;
}

Processor* SpcComponentManager::processor() const
{
	return d->processor;
}

}
