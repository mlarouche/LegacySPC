/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright (c) 2007-2011 by Michaël Larouche <larouche@kde.org>
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
#ifndef LEGACYSPC_COMMANDTESTBASE_H
#define LEGACYSPC_COMMANDTESTBASE_H

// gtest includes
#include <gtest/gtest.h>

// LegacySPC includes
#include <cpuopcodes.h>
#include <processor.h>
#include <ram.h>
#include <spcrunner.h>
#include <types.h>
#include <memorymap.h>

// STL includes
#include <vector>

using namespace LegacySPC;

class CommandTestBase : public ::testing::Test
{
public:
	CommandTestBase();
	virtual ~CommandTestBase();
	
	LegacySPC::Processor *processor();
	LegacySPC::SpcRunner *runner();

	void loadRawData(byte *data, int size);
	std::vector<byte> getTestRamData();
	void writeData(const std::vector<byte> &data);

	void setDirectPage(bool value);

	void processOpcode();

private:
	LegacySPC::SpcRunner *m_runner;
	LegacySPC::Processor *m_proc;
};

#endif
