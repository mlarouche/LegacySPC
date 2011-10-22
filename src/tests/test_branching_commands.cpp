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
#include <gtest/gtest.h>

#include "commandtestbase.h"

TEST_F(CommandTestBase, ShouldDecreaseYinDBNZCommand)
{
	const int dataSize = 2;
	byte data[dataSize] =
	{
		// DBNZ Y, $0xf1
		Dbnz_Y, 0xf1
	};

	loadRawData(data, dataSize);
	
	processor()->registers()->setY(3);
	
	processOpcode();
	
	EXPECT_EQ(2, processor()->registers()->Y());
}
