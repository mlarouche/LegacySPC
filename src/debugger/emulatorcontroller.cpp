/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright 2007-2008 by Michaël Larouche <larouche@kde.org>
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
#include "emulatorcontroller.h"

// LegacySPC includes
#include <debuggerspcrunner.h>
#include <memorymap.h>
#include <processor.h>

class EmulatorController::Private
{
public:
	LegacySPC::DebuggerSpcRunner runner;
};

EmulatorController::EmulatorController(QObject *parent)
 : QObject(parent), d(new Private)
{
}

EmulatorController::~EmulatorController()
{
	delete d;
}

LegacySPC::Processor *EmulatorController::processor() const
{
	return d->runner.processor();
}

LegacySPC::MemoryMap *EmulatorController::memory() const
{
	return d->runner.memory();
}

void EmulatorController::load(const QString &filename)
{
	if( !d->runner.loadSpcFile( filename.toStdString() ) )
	{
		emit error();
	}

	emitProcessorUpdated();
}

void EmulatorController::reset()
{
	// TODO
}

void EmulatorController::run()
{
	// TODO
}

void EmulatorController::step()
{
	d->runner.processor()->processOpcode();
	emitProcessorUpdated();
}

void EmulatorController::stop()
{
	// TODO
}

void EmulatorController::emitProcessorUpdated()
{
	emit processorUpdated(d->runner.processor());
}

#include "emulatorcontroller.moc"
