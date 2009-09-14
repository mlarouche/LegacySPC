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
#include "testlogicoperations.h"

using namespace std;
using namespace LegacySPC;

TestLogicOperations::TestLogicOperations()
 : CommandTestBase()
{
}

void TestLogicOperations::testAnd()
{
	const int opcodeSize = 39;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x7f
		Mov_A_ImmediateData, 0x7f,
		// AND A, #0x2f
		And_ImmediateData, 0x2f,
		// MOV X, #0x20
		Mov_X_ImmediateData, 0x20,
		// AND A, (X)
		And_IndirectX,
		// AND A, D$32 (DP ON)
		And_DirectPage, 0x32,
		// AND A, D$32+X
		And_DirectPagePlusX, 0x32,
		// AND A, $010F
		And_Absolute, 0x0F, 0x01,
		// MOV A, #$AD
		Mov_A_ImmediateData, 0xAD,
		// AND A, $0100+X
		And_AbsolutePlusX, 0x00, 0x01,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x5,
		// AND A, $0100+Y
		And_AbsolutePlusY, 0x00, 0x01,
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// AND A, [D$64+X]
		And_IndirectDirectPagePlusX, 0x64,
		// AND A, [D$65]+Y
		And_IndirectDirectPagePlusY, 0x65,
		// AND (X), (Y)
		And_IndirectXY,
		// AND D$00, D$10 (DP ON)
		And_DirectPage_DirectPage, 0x00, 0x10,
		// AND D$00, #0x2
		And_DirectPage_ImmediateData, 0x00, 0x2
	};

	// Load RAM
	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	// Process MOV A, #$7f
	processOpcode();

	// AND A, Immediate data(0x2f)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x2f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV X, #0x20
	processOpcode();

	// AND A, (X)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x2e );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(true);

	// AND A, D$32
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x0a );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// AND A, D$32+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x2 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// AND A, $010F
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV A, #$AD
	processOpcode();

	// AND A, $0100+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa1 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV Y, #0x5
	processOpcode();

	// AND A, $0100+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x80 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV X, #0x1
	processOpcode();

	// Process MOV Y, #0x0
	processOpcode();

	// AND A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// AND A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(false);

	// AND (X), (Y) (DP OFF)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0x42 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(true);

	// AND D$00, D$10 (DP ON)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x40 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// AND D$00, #0x2
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
}

void TestLogicOperations::testOr()
{
	const int opcodeSize = 39;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x7f
		Mov_A_ImmediateData, 0x7f,
		// OR A, #0x2f
		Or_ImmediateData, 0x2f,
		// MOV X, #0x20
		Mov_X_ImmediateData, 0x20,
		// OR A, (X)
		Or_IndirectX,
		// MOV A, #$11
		Mov_A_ImmediateData, 0x11,
		// OR A, D$32 (DP ON)
		Or_DirectPage, 0x32,
		// OR A, D$32+X
		Or_DirectPagePlusX, 0x32,
		// OR A, $010F
		Or_Absolute, 0x0F, 0x01,
		// OR A, $0100+X
		Or_AbsolutePlusX, 0x00, 0x01,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x5,
		// OR A, $0100+Y
		Or_AbsolutePlusY, 0x00, 0x01,
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// OR A, [D$64+X]
		Or_IndirectDirectPagePlusX, 0x64,
		// OR A, [D$65]+Y
		Or_IndirectDirectPagePlusY, 0x65,
		// OR (X), (Y)
		Or_IndirectXY,
		// OR D$00, D$10 (DP ON)
		Or_DirectPage_DirectPage, 0x00, 0x10,
		// OR D$00, #0x2
		Or_DirectPage_ImmediateData, 0x00, 0x2
	};

	// Load RAM
	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	// Process MOV A, #$7f
	processOpcode();

	// OR A, Immediate data(0x2f)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV X, #0x20
	processOpcode();

	// OR A, (X)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xff );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	setDirectPage(true);
	
	// Process MOV A, #$11
	processOpcode();

	// OR A, D$32
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x1b );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// OR A, D$32+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7b );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// OR A, $010F
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7b );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// OR A, $0100+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xfb );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV Y, #0x5
	processOpcode();

	// OR A, $0100+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xff );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV X, #0x1
	processOpcode();

	// Process MOV Y, #0x0
	processOpcode();

	// OR A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xff );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// OR A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xff );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	setDirectPage(false);

	// OR (X), (Y) (DP OFF)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0x6f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(true);

	// OR D$00, D$10 (DP ON)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0xdd );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// OR D$00, #0x2
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0xdf );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
}

void TestLogicOperations::testEor()
{
	const int opcodeSize = 37;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x7f
		Mov_A_ImmediateData, 0x7f,
		// EOR A, #0x2f
		Eor_ImmediateData, 0x2f,
		// MOV X, #0x20
		Mov_X_ImmediateData, 0x20,
		// EOR A, (X)
		Eor_IndirectX,
		// EOR A, D$32 (DP ON)
		Eor_DirectPage, 0x32,
		// EOR A, D$32+X
		Eor_DirectPagePlusX, 0x32,
		// EOR A, $010F
		Eor_Absolute, 0x0F, 0x01,
		// EOR A, $0100+X
		Eor_AbsolutePlusX, 0x00, 0x01,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x5,
		// EOR A, $0100+Y
		Eor_AbsolutePlusY, 0x00, 0x01,
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// EOR A, [D$64+X]
		Eor_IndirectDirectPagePlusX, 0x64,
		// EOR A, [D$65]+Y
		Eor_IndirectDirectPagePlusY, 0x65,
		// EOR (X), (Y)
		Eor_IndirectXY,
		// EOR D$00, D$10 (DP ON)
		Eor_DirectPage_DirectPage, 0x00, 0x10,
		// EOR D$00, #0x2
		Eor_DirectPage_ImmediateData, 0x00, 0x2
	};

	// Load RAM
	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	// Process MOV A, #$7f
	processOpcode();

	// EOR A, Immediate data(0x2f)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x50 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV X, #0x20
	processOpcode();

	setDirectPage(false);

	// EOR A, (X) (DP OFF)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xae );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	setDirectPage(true);

	// EOR A, D$32 (DP ON)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa4 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// EOR A, D$32+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xd6 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// EOR A, $010F
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xdf );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// EOR A, $0100+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x3c );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV Y, #0x5
	processOpcode();

	// EOR A, $0100+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV X, #0x1
	processOpcode();

	// Process MOV Y, #0x0
	processOpcode();

	// EOR A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xb2 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// EOR A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	setDirectPage(false);

	// EOR (X), (Y) (DP OFF)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0x2d );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(true);

	// EOR D$00, D$10 (DP ON)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x9d );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// EOR D$00, #0x2
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x9f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
}

QTEST_APPLESS_MAIN ( TestLogicOperations )

#include "testlogicoperations.moc"
