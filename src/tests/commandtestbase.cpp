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
#include "commandtestbase.h"

// STL includes
#include <fstream>

// LegacySPC includes
#include <legacyspc_debug.h>

using namespace LegacySPC;

CommandTestBase::CommandTestBase()
 : QObject(0)
{
	m_runner = new LegacySPC::SpcRunner();
	m_proc = new LegacySPC::Processor(m_runner);
}

CommandTestBase::~CommandTestBase()
{
	lDebug() << "Cleaning up test resources";

	delete m_proc;
	delete m_runner;
}

LegacySPC::Processor *CommandTestBase::processor()
{
	return m_proc;
}

LegacySPC::SpcRunner *CommandTestBase::runner()
{
	return m_runner;
}

void CommandTestBase::loadRawData(byte *data, int size)
{
	vector<byte> ramData;
	for(int i=0; i<size; i++)
	{
		ramData.push_back( data[i] );
	}

	writeData(ramData);
}

std::vector<byte> CommandTestBase::getTestRamData()
{
	std::vector<byte> result;

	ifstream ramDataFile(LEGACYSPC_TESTDATA"ramData.bin", ifstream::binary);
	if( !ramDataFile )
	{
		lWarning() << "Could not load ramData.bin";
	}
	else
	{
		while( !ramDataFile.eof() )
		{
			char data;
			ramDataFile.get(data);

			result.push_back( static_cast<byte>(data) );
		}
	}
	ramDataFile.close();

	return result;
}

void CommandTestBase::writeData(const std::vector<byte> &data)
{
	runner()->memory()->writeBytes(0, data );
}

void CommandTestBase::setDirectPage(bool value)
{
	if( value )
	{
		processor()->registers()->setProgramStatus( processor()->registers()->programStatus() | DirectPageFlag );
	}
	else
	{
		processor()->registers()->setProgramStatus( processor()->registers()->programStatus() & ~DirectPageFlag );
	}
}

void CommandTestBase::processOpcode()
{
	processor()->processOpcode();
}

#include "commandtestbase.moc"
