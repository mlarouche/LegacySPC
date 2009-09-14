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
#include "testdatatransmissionoperations.h"

#include <legacyspc_debug.h>

using namespace std;
using namespace LegacySPC;

TestDataTransmissionOperations::TestDataTransmissionOperations()
 : CommandTestBase()
{}

void TestDataTransmissionOperations::testMovImmediate()
{
	const int numOpcodes = 14;
	byte movOpcodes[numOpcodes] =
	{
		// MOV A, #0xF1
		Mov_A_ImmediateData, 0xf1,
		// MOV X, #0x3F
		Mov_X_ImmediateData, 0x3f,
		// MOV Y, #0x0
		Mov_Y_ImmediateData, 0x0,
		// MOV A, X
		Mov_A_X,
		// MOV Y, A
		Mov_Y_A,
		// MOV Y, #0xb0
		Mov_Y_ImmediateData, 0xb0,
		// MOV A, Y
		Mov_A_Y,
		// MOV X, A
		Mov_X_A,
		// MOV SP, X
		Mov_SP_X,
		// MOV X, SP
		Mov_X_SP
	};

	loadRawData(movOpcodes, numOpcodes);

	// Test MOV A, #data
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xf1 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV X, #data
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0x3f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test MOV Y, #data
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x00 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test MOV A, X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x3f );
	QCOMPARE( int(processor()->registers()->X()), 0x3f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test Y, A
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x3f );
	QCOMPARE( int(processor()->registers()->A()), 0x3f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV Y, #
	processOpcode();

	// Test MOV A, Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xb0 );
	QCOMPARE( int(processor()->registers()->Y()), 0xb0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV X, A
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0xb0 );
	QCOMPARE( int(processor()->registers()->A()), 0xb0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV SP, X
	processOpcode();
	QCOMPARE( int(processor()->registers()->stackPointer()), 0xb0 );
	QCOMPARE( int(processor()->registers()->X()), 0xb0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV X, SP
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0xb0 );
	QCOMPARE( int(processor()->registers()->stackPointer()), 0xb0 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
}

void TestDataTransmissionOperations::testMovIndirectX()
{
	const int dataSize = 12;
	byte ramData[dataSize] =
	{
		// Data
		0x3f, 0x23, 0x24, 0x25, 0x56, 0x02,
		Mov_X_ImmediateData, 0x2,
		Mov_A_IndirectX,
		Mov_A_ImmediateData, 0x45,
		Mov_IndirectX_A
	};

	loadRawData(ramData, dataSize);

	processor()->registers()->setProgramCounter( 6 );

	// Execute MOV X, #0x2
	processOpcode();

	// Test MOV A, (X)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x24 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV A, #$45
	processOpcode();
	
	// Test MOV (X), A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0x45 );
}

void TestDataTransmissionOperations::testMovIndirectXAutoIncrement()
{
	const int dataSize = 19;
	byte ramData[dataSize] =
	{
		// Data
		0x3f, 0x23, 0x24, 0x25, 0x56, 0x02,
		Mov_X_ImmediateData, 0x0,
		Mov_A_IndirectXAutoIncrement,
		Mov_A_IndirectXAutoIncrement,
		Mov_A_IndirectXAutoIncrement,
		Mov_A_IndirectXAutoIncrement,
		Mov_A_IndirectXAutoIncrement,
		Mov_A_IndirectXAutoIncrement,
		Mov_X_ImmediateData, 0x0,
		Mov_A_ImmediateData, 0x56,
		Mov_IndirectXAutoIncrement_A
	};

	loadRawData(ramData, dataSize);

	processor()->registers()->setProgramCounter( 6 );

	// Execute MOV X, #$0x0
	processOpcode();

	for(int i=0; i<6; i++)
	{
		processOpcode();

		QCOMPARE( int(processor()->registers()->A()), (int)ramData[i] );
		QCOMPARE( int(processor()->registers()->X()), i+1 );
		QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
		QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	}

	// Process MOV X, #$00
	processOpcode();
	// Process MOV A, #0x56
	processOpcode();

	// Test MOV (X)+, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0), 0x56 );
	QCOMPARE( int(processor()->registers()->X()), 1 );
}

void TestDataTransmissionOperations::testMovDirectPage()
{
	const int opcodeSize = 48;
	byte opcodeData[opcodeSize] =
	{
		// MOV A, D$09
		Mov_A_DirectPage, 0x9,
		// MOV A, D$09 // With direct page on
		Mov_A_DirectPage, 0x9,
		// MOV X, D$20
		Mov_X_DirectPage, 0x20,
		// MOV X, D$20 // With direct page on
		Mov_X_DirectPage, 0x20,
		// MOV Y, D$40
		Mov_Y_DirectPage, 0x40,
		// MOV Y, D$40 // With direct page on
		Mov_Y_DirectPage, 0x40,
		// MOV X, #$04
		Mov_X_ImmediateData, 0x4,
		// MOV A, D$09+X (DP OFF)
		Mov_A_DirectPagePlusX, 0x9,
		// MOV Y, #$03
		Mov_Y_ImmediateData, 0x3,
		// MOV X, D$20+Y (DP OFF)
		Mov_X_DirectPagePlusY, 0x20,
		// MOV X, #$06
		Mov_X_ImmediateData, 0x6,
		// MOV Y, D$40+X (DP OFF)
		Mov_Y_DirectPagePlusX, 0x40,
		// MOV D$20, #$00 (DP ON)
		Mov_DirectPage_ImmediateData, 0x20, 0x0,
		// MOV D$20, D$21 (DP ON)
		Mov_DirectPage_DirectPage, 0x20, 0x21,
		// MOV A, #$69
		Mov_A_ImmediateData, 0x69,
		// MOV X, #$34
		Mov_X_ImmediateData, 0x34,
		// MOV Y, #$23
		Mov_Y_ImmediateData, 0x23,
		// MOV D$20, A (DP ON)
		Mov_DirectPage_A, 0x20,
		// MOV D$00+X, A
		Mov_DirectPagePlusX_A, 0x00,
		// MOV D$30, X
		Mov_DirectPage_X, 0x30,
		// MOV D$00+Y, X
		Mov_DirectPagePlusY_X, 0x0,
		// MOV D$40, Y
		Mov_DirectPage_Y, 0x40,
		// MOV D$00+X, Y
		Mov_DirectPagePlusX_Y, 0x00
	};

	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	// Test MOV A, D$09
	setDirectPage(false);
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xd4 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV A, D$09 (with direct page on)
	setDirectPage(true);
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xae );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV X, D$20
	setDirectPage(false);
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0xfe );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV X, D$20 (with direct page on)
	setDirectPage(true);
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0xe3 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV Y, D$40
	setDirectPage(false);
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0xf9 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV Y, D$40 (with direct page on)
	setDirectPage(true);
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x79 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	setDirectPage(false);

	// Process MOV X, #$04
	processOpcode();

	// Test MOV A, D$09+X (DP OFF)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x9b );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Process MOV Y, #$03
	processOpcode();

	// Test MOV X, D$20+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0x18 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV X, #$06
	processOpcode();

	// Test MOV Y, D$40+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x6e );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test MOV D$20, #$00 (DP ON)
	setDirectPage(true);
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x120), 0x0 );;

	// Test MOV D$20, D$21 (DP ON)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x120), 0x70 );

	// Process // MOV A, #$69, MOV X, #$34, MOV Y, #$23
	for(int i=0; i<3; i++)
	{
		processOpcode();
	}

	setDirectPage(true);

	// Test MOV D$20, A (DP ON)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0120), 0x69 );
	// Test MOV D$00+X, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0134), 0x69 );
	// Test MOV D$30, X
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0130), 0x34 );
	// Test MOV D$00+Y, X
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0123), 0x34 );
	// Test MOV D$40, Y
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0140), 0x23 );
	// Test MOV D$00+X, Y
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x0134), 0x23 );
}

void TestDataTransmissionOperations::testMovAbsolute()
{
	const int opcodeSize = 34;
	byte opcodeData[opcodeSize] =
	{
		// MOV A, $0068
		Mov_A_Absolute, 0x68, 0x00,
		// MOV X, #$05
		Mov_X_ImmediateData, 0x5,
		// MOV A, $0060+X
		Mov_A_AbsolutePlusX, 0x60, 0x00,
		// MOV Y, #$03
		Mov_Y_ImmediateData, 0x3,
		// MOV A, $0060+Y
		Mov_A_AbsolutePlusY, 0x60, 0x00,
		// MOV X, $0052
		Mov_X_Absolute, 0x52, 0x00,
		// MOV Y, $0034
		Mov_Y_Absolute, 0x34, 0x00,
		// MOV $0012, A
		Mov_Absolute_A, 0x12, 0x00,
		// MOV $0012+X, A
		Mov_AbsolutePlusX_A, 0x12, 0x00,
		// MOV $0012+Y, A
		Mov_AbsolutePlusY_A, 0x12, 0x00,
		// MOV $0120, X
		Mov_Absolute_X, 0x20, 0x01,
		// MOV $0120, Y
		Mov_Absolute_Y, 0x20, 0x01
	};

	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	// Test MOV A, $0068
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x2f );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV X, #$05
	processOpcode();

	// Test MOV A, $0060+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x54 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV Y, #$03
	processOpcode();

	// Test MOV A, $0060+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x5d );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test MOV X, $0052
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0xf9 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV Y, $0034
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0xac );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// Test MOV $0012, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x0012 ), 0x5d );
	// MOV $0012+X, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x0012+0xf9 ), 0x5d );
	// MOV $0012+Y, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x0012+0xac ), 0x5d );
	// MOV $0120, X
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x0120 ), 0xf9 );
	// MOV $0120, Y
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x0120 ), 0xac );
}

void TestDataTransmissionOperations::testMovIndirectDirectPage()
{
	const int opcodeSize = 16;
	byte opcodeData[opcodeSize] =
	{
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// MOV A, [D$64+X]
		Mov_A_IndirectDirectPagePlusX, 0x64,
		// MOV A, [D$65]+Y
		Mov_A_IndirectDirectPagePlusY, 0x65,
		// MOV A, #$69
		Mov_A_ImmediateData, 0x69,
		// MOV [D$64+X], A
		Mov_IndirectDirectPagePlusX_A, 0x64,
		// MOV A, #$45
		Mov_A_ImmediateData, 0x45,
		// MOV [D$65]+Y, A
		Mov_IndirectDirectPagePlusY_A, 0x65
	};

	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );
	setDirectPage(true);

	// Process MOV X, #$01
	processOpcode();

	// Process MOV Y, #$00
	processOpcode();

	// Test MOV A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x12 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Test MOV A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x12 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV A, #$69
	processOpcode();

	// Test MOV [D$64+X], A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x51), 0x69 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// Process MOV A, #$45
	processOpcode();

	// Test MOV [D$65]+Y, A
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte(0x51), 0x45 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
}

QTEST_APPLESS_MAIN ( TestDataTransmissionOperations )
#include "testdatatransmissionoperations.moc"
