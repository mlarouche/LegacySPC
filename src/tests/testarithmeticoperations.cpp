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
#include "testarithmeticoperations.h"

using namespace LegacySPC;
using namespace std;

TestArithmeticOperations::TestArithmeticOperations()
 : CommandTestBase()
{}


void TestArithmeticOperations::testAddWithCarry()
{
	const int opcodeSize = 37;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x7f
		Mov_A_ImmediateData, 0x7f,
		// ADC A, #0x1
		Adc_ImmediateData, 0x1,
		// MOV X, #0x20
		Mov_X_ImmediateData, 0x20,
		// ADC A, (X)
		Adc_IndirectX,
		// ADC A, D$32 (DP ON)
		Adc_DirectPage, 0x32,
		// ADC A, D$32+X
		Adc_DirectPagePlusX, 0x32,
		// ADC A, $010F
		Adc_Absolute, 0x0F, 0x01,
		// ADC A, $0100+X
		Adc_AbsolutePlusX, 0x00, 0x01,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x5,
		// ADC A, $0100+Y
		Adc_AbsolutePlusY, 0x00, 0x01,
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// ADC A, [D$64+X]
		Adc_IndirectDirectPagePlusX, 0x64,
		// ADC A, [D$65]+Y
		Adc_IndirectDirectPagePlusY, 0x65,
		// ADC (X), (Y)
		Adc_IndirectXY,
		// ADC D$00, D$10 (DP ON)
		Adc_DirectPage_DirectPage, 0x00, 0x10,
		// ADC D$00, #0x2
		Adc_DirectPage_ImmediateData, 0x00, 0x2
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

	// ADC A, Immediate data(0x1)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x80 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// Process MOV X, #0x20
	processOpcode();

	// ADC A, (X)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7e );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	setDirectPage(true);

	// ADC A, D$32
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x89 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false);
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// ADC A, D$32+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xfb );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// ADC A, $010F
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x04 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// ADC A, $0100+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xe8 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// Process MOV Y, #0x5
	processOpcode();

	// ADC A, $0100+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x84 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// Process MOV X, #0x1
	processOpcode();

	// Process MOV Y, #0x0
	processOpcode();

	// ADC A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x97 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// ADC A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa9 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	setDirectPage(false);

	// ADC (X), (Y) (DP OFF)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0xb1 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	setDirectPage(true);

	// ADC D$00, D$10
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x1d );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// ADC D$00, #0x2
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x20 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );
}

void TestArithmeticOperations::testSubtractWithCarry()
{
	const int opcodeSize = 37;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x7f
		Mov_A_ImmediateData, 0x7f,
		// SBC A, #0x1
		Sbc_ImmediateData, 0x1,
		// MOV X, #0x20
		Mov_X_ImmediateData, 0x20,
		// SBC A, (X)
		Sbc_IndirectX,
		// SBC A, D$32 (DP ON)
		Sbc_DirectPage, 0x32,
		// SBC A, D$32+X
		Sbc_DirectPagePlusX, 0x32,
		// SBC A, $010F
		Sbc_Absolute, 0x0F, 0x01,
		// SBC A, $0100+X
		Sbc_AbsolutePlusX, 0x00, 0x01,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x5,
		// SBC A, $0100+Y
		Sbc_AbsolutePlusY, 0x00, 0x01,
		// MOV X, #$01
		Mov_X_ImmediateData, 0x1,
		// MOV Y, #$00
		Mov_Y_ImmediateData, 0x0,
		// SBC A, [D$64+X]
		Sbc_IndirectDirectPagePlusX, 0x64,
		// SBC A, [D$65]+Y
		Sbc_IndirectDirectPagePlusY, 0x65,
		// SBC (X), (Y)
		Sbc_IndirectXY,
		// SBC D$00, D$10 (DP ON)
		Sbc_DirectPage_DirectPage, 0x00, 0x10,
		// SBC D$00, #0x2
		Sbc_DirectPage_ImmediateData, 0x00, 0x2
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

	// SBC A, Immediate data(0x1)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7f );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// Process MOV X, #0x20
	processOpcode();

	setDirectPage(false);

	// SBC A, (X) (DP OFF)
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x81 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	setDirectPage(true);

	// SBC A, D$32
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x78 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// SBC A, D$32+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x6 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// SBC A, $010F
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xfd );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// SBC A, $0100+X
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x1b );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// Process MOV Y, #0x5
	processOpcode();

	// SBC A, $0100+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x7f );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// Process MOV X, #0x1
	processOpcode();

	// Process MOV Y, #0x0
	processOpcode();

	// SBC A, [D$64+X]
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x6e );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	// SBC A, [D$65]+Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0x5c );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	setDirectPage(false);

	// SBC (X), (Y) (DP OFF)
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( processor()->registers()->X() ), 0xe5 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );

	setDirectPage(true);

	// SBC D$00, D$10
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x76 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), true );

	// SBC D$00, #0x2
	processOpcode();
	QCOMPARE( (int)runner()->memory()->readByte( 0x100 ), 0x75 );
	QCOMPARE( processor()->isProgramStatusFlagSet(CarryFlag), true );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(OverflowFlag), false );
}

void TestArithmeticOperations::testCompare()
{
}

void TestArithmeticOperations::testIncDec()
{
	const int opcodeSize = 26;

	byte opcodeData[opcodeSize] =
	{
		// MOV A, #0x0a
		Mov_A_ImmediateData, 0x0a,
		// MOV X, #0x3
		Mov_X_ImmediateData, 0x03,
		// MOV Y, #0x5
		Mov_Y_ImmediateData, 0x05,
		// INC X
		Inc_X,
		// INC Y
		Inc_Y,
		// INC A
		Inc_A,
		// INC D$20 (DP OFF)
		Inc_DirectPage, 0x20,
		// INC D$20+X
		Inc_DirectPagePlusX, 0x20,
		// INC $0040
		Inc_Absolute, 0x40, 0x00,
		// DEC D$20+X
		Dec_DirectPagePlusX, 0x20,
		// DEC D$20
		Dec_DirectPage, 0x20,
		// DEC $0040
		Dec_Absolute, 0x40, 0x00,
		// DEC A
		Dec_A,
		// DEC X
		Dec_X,
		// DEC Y
		Dec_Y
	};

	// Load RAM
	vector<byte> ramData = getTestRamData();

	for(int i=0; i<opcodeSize; i++)
	{
		ramData.push_back( opcodeData[i] );
	}

	writeData( ramData );

	processor()->registers()->setProgramCounter( ramData.size()-opcodeSize );

	setDirectPage(false);

	// Process MOV A, #0x0a
	processOpcode();

	// Process MOV X, #0x3
	processOpcode();
		
	// Process MOV Y, #0x5
	processOpcode();

	// INC X
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0x4 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// INC Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x6 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// INC A
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xb );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// INC D$20 (DP OFF)
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x20)), 0xff );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// INC D$20+X
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x24)), 0xc9 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// INC $0040
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x0040)), 0xfa );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// DEC D$20+X
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x24)), 0xc8 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// DEC D$20
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x20)), 0xfe );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// DEC $0040
	processOpcode();
	QCOMPARE( int(runner()->memory()->readByte(0x0040)), 0xf9 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), true );

	// DEC A
	processOpcode();
	QCOMPARE( int(processor()->registers()->A()), 0xa );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// DEC X
	processOpcode();
	QCOMPARE( int(processor()->registers()->X()), 0x3 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );

	// DEC Y
	processOpcode();
	QCOMPARE( int(processor()->registers()->Y()), 0x5 );
	QCOMPARE( processor()->isProgramStatusFlagSet(ZeroFlag), false );
	QCOMPARE( processor()->isProgramStatusFlagSet(NegativeFlag), false );
}

QTEST_APPLESS_MAIN ( TestArithmeticOperations )

#include "testarithmeticoperations.moc"
