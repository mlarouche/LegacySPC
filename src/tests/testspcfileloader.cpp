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
#include "testspcfileloader.h"

// Qt includes
#include <QtTest>

// STL includes
#include <fstream>

// LegacySPC includes
#include "spcfileloader.h"
#include "spcfile.h"
#include "processor.h"

using namespace LegacySPC;
using namespace std;

void TestSpcFileLoader::testMmxFile()
{
	// The goal of this test is to test a text-based ID666 tag but not complete
	SpcFileLoader mmxFileLoader(LEGACYSPC_TESTDATA"mmx1_prologue.spc");
	
	QCOMPARE( !mmxFileLoader, false );

	SpcFile mmxFile = mmxFileLoader.spcFile();

	// Test Processor registers
	QCOMPARE( (int)mmxFile.processorRegisters().programCounter(), 0x03C2 );
	QCOMPARE( (int)mmxFile.processorRegisters().A(), 0xF9 );
	QCOMPARE( (int)mmxFile.processorRegisters().X(), 0xFF );
	QCOMPARE( (int)mmxFile.processorRegisters().Y(), 0x02 );
	QCOMPARE( (int)mmxFile.processorRegisters().programStatus(), 0x80 );
	QCOMPARE( (int)mmxFile.processorRegisters().stackPointer(), 0xCD );

	// Test ID666 Tag
	QCOMPARE( mmxFile.id666Tag().songTitle.c_str(), "Prologue stage" );
	QCOMPARE( mmxFile.id666Tag().gameTitle.c_str(), "MEGAMAN X" );
	QCOMPARE( mmxFile.id666Tag().dumperName.c_str(), "" );
	QCOMPARE( mmxFile.id666Tag().comment.c_str(), "" );
	QCOMPARE( mmxFile.id666Tag().dateDumped.c_str(), "20.9.2000" );
	QCOMPARE( mmxFile.id666Tag().songLength, 46 );
	QCOMPARE( mmxFile.id666Tag().fadeoutLength, 5714 );
	QCOMPARE( mmxFile.id666Tag().artistName.c_str(), "" );
	QCOMPARE( mmxFile.id666Tag().isDefaultChannelDisabled, false );
	QCOMPARE( (int)mmxFile.id666Tag().emulatorIndex, 1 );
	QCOMPARE( mmxFile.id666Tag().isBinary, false );
}

void TestSpcFileLoader::testDck2File()
{
	// The goal of this test is to test loading of a SPC with an empty ID666 Tag
	SpcFileLoader dckFileLoader(LEGACYSPC_TESTDATA"dkc2_roller_coaster.spc");
	
	QCOMPARE( !dckFileLoader, false );

	SpcFile dckFile = dckFileLoader.spcFile();

	// Test Processor registers
	QCOMPARE( (int)dckFile.processorRegisters().programCounter(), 0x0A3C );
	QCOMPARE( (int)dckFile.processorRegisters().A(), 0x00 );
	QCOMPARE( (int)dckFile.processorRegisters().X(), 0x04 );
	QCOMPARE( (int)dckFile.processorRegisters().Y(), 0x19 );
	QCOMPARE( (int)dckFile.processorRegisters().programStatus(), 0x02 );
	QCOMPARE( (int)dckFile.processorRegisters().stackPointer(), 0xfd );

	// Test ID666 Tag
	QCOMPARE( dckFile.id666Tag().songTitle.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().gameTitle.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().dumperName.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().comment.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().dateDumped.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().songLength, 0 );
	QCOMPARE( dckFile.id666Tag().fadeoutLength, 0 );
	QCOMPARE( dckFile.id666Tag().artistName.c_str(), "" );
	QCOMPARE( dckFile.id666Tag().isDefaultChannelDisabled, false );
	QCOMPARE( (int)dckFile.id666Tag().emulatorIndex, 0 );
	QCOMPARE( dckFile.id666Tag().isBinary, false );
}

void TestSpcFileLoader::testBinaryTag()
{
	// The goal of this test is tot test loading of a binary ID666 Tag
	SpcFileLoader binaryFileLoader(LEGACYSPC_TESTDATA"rs3_binarytag.spc");
	
	QCOMPARE( !binaryFileLoader, false );

	SpcFile binaryFile = binaryFileLoader.spcFile();

	// Test Processor registers
	QCOMPARE( (int)binaryFile.processorRegisters().programCounter(), 0x02F4 );
	QCOMPARE( (int)binaryFile.processorRegisters().A(), 0x1E );
	QCOMPARE( (int)binaryFile.processorRegisters().X(), 0x22 );
	QCOMPARE( (int)binaryFile.processorRegisters().Y(), 0x01 );
	QCOMPARE( (int)binaryFile.processorRegisters().programStatus(), 0x00 );
	QCOMPARE( (int)binaryFile.processorRegisters().stackPointer(), 0xff );

	// Test ID666 Tag
	QCOMPARE( binaryFile.id666Tag().songTitle.c_str(), "Battle Theme" );
	QCOMPARE( binaryFile.id666Tag().gameTitle.c_str(), "Romancing Saga 3" );
	QCOMPARE( binaryFile.id666Tag().dumperName.c_str(), "Zophar Domain" );
	QCOMPARE( binaryFile.id666Tag().comment.c_str(), "best music from best author" );
	QCOMPARE( binaryFile.id666Tag().dateDumped.c_str(), "2007/12/9" );
	QCOMPARE( binaryFile.id666Tag().songLength, 200 );
	QCOMPARE( binaryFile.id666Tag().fadeoutLength, 10000 );
	QCOMPARE( binaryFile.id666Tag().artistName.c_str(), "Kenji Ito" );
	QCOMPARE( binaryFile.id666Tag().isDefaultChannelDisabled, false );
	QCOMPARE( (int)binaryFile.id666Tag().emulatorIndex, 0 );
	QCOMPARE( binaryFile.id666Tag().isBinary, true );
}

void TestSpcFileLoader::testFailure()
{
	SpcFileLoader failureLoader("roeotetewer");

	ifstream testFstream("roeotetewer");
	QCOMPARE( !testFstream, true );
	QCOMPARE( !failureLoader, true );
}

void TestSpcFileLoader::testMagicHeader()
{
	SpcFileLoader magicHeaderLoader(LEGACYSPC_TESTDATA"notaspcfile");

	ifstream testFstream(LEGACYSPC_TESTDATA"notaspcfile");
	QCOMPARE( !testFstream, false );
	QCOMPARE( !magicHeaderLoader, true );
}

QTEST_APPLESS_MAIN ( TestSpcFileLoader )

#include "testspcfileloader.moc"
