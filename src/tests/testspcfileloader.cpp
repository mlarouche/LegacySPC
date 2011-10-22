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
// gtest includes
#include <gtest/gtest.h>

// STL includes
#include <fstream>

// LegacySPC includes
#include "spcfileloader.h"
#include "spcfile.h"
#include "processor.h"

using namespace LegacySPC;
using namespace std;

TEST(TestSpcFileLoader, TestMMXFile)
{
	// The goal of this test is to test a text-based ID666 tag but not complete
	SpcFileLoader mmxFileLoader(LEGACYSPC_TESTDATA"mmx1_prologue.spc");
	
	EXPECT_EQ( !mmxFileLoader, false );

	SpcFile mmxFile = mmxFileLoader.spcFile();

	// Test Processor registers
	EXPECT_EQ( (int)mmxFile.processorRegisters().programCounter(), 0x03C2 );
	EXPECT_EQ( (int)mmxFile.processorRegisters().A(), 0xF9 );
	EXPECT_EQ( (int)mmxFile.processorRegisters().X(), 0xFF );
	EXPECT_EQ( (int)mmxFile.processorRegisters().Y(), 0x02 );
	EXPECT_EQ( (int)mmxFile.processorRegisters().programStatus(), 0x80 );
	EXPECT_EQ( (int)mmxFile.processorRegisters().stackPointer(), 0xCD );

	// Test ID666 Tag
	EXPECT_STREQ( mmxFile.id666Tag().songTitle.c_str(), "Prologue stage" );
	EXPECT_STREQ( mmxFile.id666Tag().gameTitle.c_str(), "MEGAMAN X" );
	EXPECT_STREQ( mmxFile.id666Tag().dumperName.c_str(), "" );
	EXPECT_STREQ( mmxFile.id666Tag().comment.c_str(), "" );
	EXPECT_STREQ( mmxFile.id666Tag().dateDumped.c_str(), "20.9.2000" );
	EXPECT_EQ( mmxFile.id666Tag().songLength, 46 );
	EXPECT_EQ( mmxFile.id666Tag().fadeoutLength, 5714 );
	EXPECT_STREQ( mmxFile.id666Tag().artistName.c_str(), "" );
	EXPECT_EQ( mmxFile.id666Tag().isDefaultChannelDisabled, false );
	EXPECT_EQ( (int)mmxFile.id666Tag().emulatorIndex, 1 );
	EXPECT_EQ( mmxFile.id666Tag().isBinary, false );
}

TEST(TestSpcFileLoader, TestDck2File)
{
	// The goal of this test is to test loading of a SPC with an empty ID666 Tag
	SpcFileLoader dckFileLoader(LEGACYSPC_TESTDATA"dkc2_roller_coaster.spc");
	
	EXPECT_EQ( !dckFileLoader, false );

	SpcFile dckFile = dckFileLoader.spcFile();

	// Test Processor registers
	EXPECT_EQ( (int)dckFile.processorRegisters().programCounter(), 0x0A3C );
	EXPECT_EQ( (int)dckFile.processorRegisters().A(), 0x00 );
	EXPECT_EQ( (int)dckFile.processorRegisters().X(), 0x04 );
	EXPECT_EQ( (int)dckFile.processorRegisters().Y(), 0x19 );
	EXPECT_EQ( (int)dckFile.processorRegisters().programStatus(), 0x02 );
	EXPECT_EQ( (int)dckFile.processorRegisters().stackPointer(), 0xfd );

	// Test ID666 Tag
	EXPECT_STREQ( dckFile.id666Tag().songTitle.c_str(), "" );
	EXPECT_STREQ( dckFile.id666Tag().gameTitle.c_str(), "" );
	EXPECT_STREQ( dckFile.id666Tag().dumperName.c_str(), "" );
	EXPECT_STREQ( dckFile.id666Tag().comment.c_str(), "" );
	EXPECT_STREQ( dckFile.id666Tag().dateDumped.c_str(), "" );
	EXPECT_EQ( dckFile.id666Tag().songLength, 0 );
	EXPECT_EQ( dckFile.id666Tag().fadeoutLength, 0 );
	EXPECT_STREQ( dckFile.id666Tag().artistName.c_str(), "" );
	EXPECT_EQ( dckFile.id666Tag().isDefaultChannelDisabled, false );
	EXPECT_EQ( (int)dckFile.id666Tag().emulatorIndex, 0 );
	EXPECT_EQ( dckFile.id666Tag().isBinary, false );
}

TEST(TestSpcFileLoader, TestBinaryTag)
{
	// The goal of this test is tot test loading of a binary ID666 Tag
	SpcFileLoader binaryFileLoader(LEGACYSPC_TESTDATA"rs3_binarytag.spc");
	
	EXPECT_EQ( !binaryFileLoader, false );

	SpcFile binaryFile = binaryFileLoader.spcFile();

	// Test Processor registers
	EXPECT_EQ( (int)binaryFile.processorRegisters().programCounter(), 0x02F4 );
	EXPECT_EQ( (int)binaryFile.processorRegisters().A(), 0x1E );
	EXPECT_EQ( (int)binaryFile.processorRegisters().X(), 0x22 );
	EXPECT_EQ( (int)binaryFile.processorRegisters().Y(), 0x01 );
	EXPECT_EQ( (int)binaryFile.processorRegisters().programStatus(), 0x00 );
	EXPECT_EQ( (int)binaryFile.processorRegisters().stackPointer(), 0xff );

	// Test ID666 Tag
	EXPECT_STREQ( binaryFile.id666Tag().songTitle.c_str(), "Battle Theme" );
	EXPECT_STREQ( binaryFile.id666Tag().gameTitle.c_str(), "Romancing Saga 3" );
	EXPECT_STREQ( binaryFile.id666Tag().dumperName.c_str(), "Zophar Domain" );
	EXPECT_STREQ( binaryFile.id666Tag().comment.c_str(), "best music from best author" );
	EXPECT_STREQ( binaryFile.id666Tag().dateDumped.c_str(), "2007/12/9" );
	EXPECT_EQ( binaryFile.id666Tag().songLength, 200 );
	EXPECT_EQ( binaryFile.id666Tag().fadeoutLength, 10000 );
	EXPECT_STREQ( binaryFile.id666Tag().artistName.c_str(), "Kenji Ito" );
	EXPECT_EQ( binaryFile.id666Tag().isDefaultChannelDisabled, false );
	EXPECT_EQ( (int)binaryFile.id666Tag().emulatorIndex, 0 );
	EXPECT_EQ( binaryFile.id666Tag().isBinary, true );
}

TEST(TestSpcFileLoader, TestFailure)
{
	SpcFileLoader failureLoader("roeotetewer");

	ifstream testFstream("roeotetewer");
	EXPECT_EQ( !testFstream, true );
	EXPECT_EQ( !failureLoader, true );
}

TEST(TestSpcFileLoader, TestMagicHeader)
{
	SpcFileLoader magicHeaderLoader(LEGACYSPC_TESTDATA"notaspcfile");

	ifstream testFstream(LEGACYSPC_TESTDATA"notaspcfile");
	EXPECT_EQ( !testFstream, false );
	EXPECT_EQ( !magicHeaderLoader, true );
}
