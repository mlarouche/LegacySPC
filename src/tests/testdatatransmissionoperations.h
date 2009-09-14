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
#ifndef LEGACYSPC_TESTMOVCOMMANDS_H
#define LEGACYSPC_TESTMOVCOMMANDS_H

#include "commandtestbase.h"

class TestDataTransmissionOperations : public CommandTestBase
{
	Q_OBJECT
public:
	TestDataTransmissionOperations();

private Q_SLOTS:
	void testMovImmediate();
	void testMovIndirectX();
	void testMovIndirectXAutoIncrement();
	void testMovDirectPage();
	void testMovAbsolute();
	void testMovIndirectDirectPage();
};
#endif
