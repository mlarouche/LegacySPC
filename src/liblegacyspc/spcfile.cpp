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
#include "spcfile.h"

// STL Includes

// Local includes
#include "processorregisters.h"
#include "legacyspc_debug.h"

namespace LegacySPC
{

class SpcFile::Private
{
public:
	Private()
	 : refCounting(1)
	{}

	void copy(Private *other)
	{
		tag = other->tag;
		regs = other->regs;
		ramData = other->ramData;
		dspRegisters = other->dspRegisters;
	}

	int refCounting;
	ID666Tag tag;
	ProcessorRegisters regs;
	std::vector<byte> ramData;
	std::vector<byte> dspRegisters;
};

SpcFile::SpcFile()
 : d(new Private)
{}

SpcFile::~SpcFile()
{
	if(!--d->refCounting)
	{
		lDebug() << "Deleting SpcFile data";
		delete d;
	}
}

SpcFile::SpcFile(const SpcFile &copy)
 : d(copy.d)
{
	d->refCounting++;
}

const SpcFile &SpcFile::operator=(const SpcFile &other)
{
	if( this == &other )
		return *this;

	this->d = other.d;

	return *this;
}

ID666Tag &SpcFile::id666Tag() const
{
	return d->tag;
}

void SpcFile::setID666Tag(const ID666Tag &tag)
{
	detach();

	d->tag = tag;
}

ProcessorRegisters &SpcFile::processorRegisters() const
{
	return d->regs;
}

void SpcFile::setProcessorRegisters(const ProcessorRegisters &registers)
{
	detach();

	d->regs = registers;
}

std::vector<byte> &SpcFile::ramData() const
{
	return d->ramData;
}

void SpcFile::setRamData(const std::vector<byte> &ramData)
{
	detach();

	d->ramData = ramData;
}

std::vector<byte> &SpcFile::dspRegisters() const
{
	return d->dspRegisters;
}

void SpcFile::setDspRegisters(const std::vector<byte> &dspRegisters)
{
	detach();

	d->dspRegisters = dspRegisters;
}

void SpcFile::detach()
{
	//lDebug();

	Private *olderPrivate = d;

	d = new Private;
	d->copy(olderPrivate);

	if( !--olderPrivate->refCounting )
	{
		//lDebug() << "Deleting olderPrivate";
		delete olderPrivate;
	}
}

}
