/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright 2007-2011 by Michaël Larouche <larouche@kde.org>
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
#include "processor.h"

// LegacySPC includes
#include "types.h"
#include "cpuopcodes.h"
#include "spcrunner.h"
#include "memorymap.h"
#include "legacyspc_debug.h"

namespace LegacySPC
{

/**
 * @internal
 * @brief MemBitData contains the data
 * for a MemBit operation
 */
struct MemBitData
{
	word address;
	byte bit;
};

class Processor::Private
{
public:
	Private()
	 : runner(0), regs(0), lastAddress(0)
	{
		regs = new ProcessorRegisters;
	}
	~Private()
	{
		delete regs;
	}
	
	// TODO: use a smart pointer
	SpcRunner *runner;
	ProcessorRegisters *regs;
	word lastAddress;
};

Processor::Processor(SpcRunner *runner)
  : d(new Private)
{
	d->runner = runner;
}

Processor::~Processor()
{
	delete d;
}

word Processor::lastAddress() const
{
	return d->lastAddress;
}

void Processor::processOpcode()
{
	byte opcode = readByte();

	lDebug() << "PC:" << registers()->programCounter() << "Opcode:" << opcode;

	switch(opcode)
	{
		case Mov_A_ImmediateData:
		{
			setARegister( readByte() );
			break;
		}
		case Mov_A_IndirectX:
		{
			setARegister( readByte( decodeAddress(IndirectXAddressing) ) );
			break;
		}
		case Mov_A_IndirectXAutoIncrement:
		{
			setARegister( readByte( decodeAddress(IndirectXAddressing) ) );
			registers()->incrementX();
			break;
		}
		case Mov_A_DirectPage:
		{
			setARegister( readByte( decodeAddress(DirectPageAddressing) ) );
			break;
		}
		case Mov_A_DirectPagePlusX:
		{
			setARegister( readByte( decodeAddress(DirectPagePlusXAddressing) ) );
			break;
		}
		case Mov_A_Absolute:
		{
			setARegister( readByte( decodeAddress(AbsoluteAddressing) ) );
			break;
		}
		case Mov_A_AbsolutePlusX:
		{
			setARegister( readByte( decodeAddress(AbsolutePlusXAddressing) ) );
			break;
		}
		case Mov_A_AbsolutePlusY:
		{
			setARegister( readByte( decodeAddress(AbsolutePlusYAddressing) ) );
			break;
		}
		case Mov_A_IndirectDirectPagePlusX:
		{
			setARegister( readByte( decodeAddress(IndirectDirectPagePlusXAddressing) ) );
			break;
		}
		case Mov_A_IndirectDirectPagePlusY:
		{
			setARegister( readByte( decodeAddress(IndirectDirectPagePlusYAddressing) ) );
			break;
		}
		case Mov_X_ImmediateData:
		{
			setXRegister( readByte() );
			break;
		}
		case Mov_X_DirectPage:
		{
			setXRegister( readByte( decodeAddress(DirectPageAddressing) ) );
			break;
		}
		case Mov_X_DirectPagePlusY:
		{
			setXRegister( readByte( decodeAddress(DirectPagePlusYAddressing) ) );
			break;
		}
		case Mov_X_Absolute:
		{
			setXRegister( readWord( decodeAddress(AbsoluteAddressing) ) );
			break;
		}
		case Mov_Y_ImmediateData:
		{
			setYRegister( readByte() );
			break;
		}
		case Mov_Y_DirectPage:
		{
			setYRegister( readByte( decodeAddress(DirectPageAddressing) ) );
			break;
		}
		case Mov_Y_DirectPagePlusX:
		{
			setYRegister( readByte( decodeAddress(DirectPagePlusXAddressing) ) );
			break;
		}
		case Mov_Y_Absolute:
		{
			setYRegister( readWord( decodeAddress(AbsoluteAddressing) ) );
			break;
		}
		case Mov_IndirectX_A:
		{
			writeByte( decodeAddress(IndirectXAddressing), registers()->A() );
			break;
		}
		case Mov_IndirectXAutoIncrement_A:
		{
			writeByte( decodeAddress(IndirectXAddressing), registers()->A() );
			registers()->incrementX();
			break;
		}
		case Mov_DirectPage_A:
		{
			writeByte( decodeAddress(DirectPageAddressing), registers()->A() );
			break;
		}
		case Mov_DirectPagePlusX_A:
		{
			writeByte( decodeAddress(DirectPagePlusXAddressing), registers()->A() );
			break;
		}
		case Mov_Absolute_A:
		{
			writeByte( decodeAddress(AbsoluteAddressing), registers()->A() );
			break;
		}
		case Mov_AbsolutePlusX_A:
		{
			writeByte( decodeAddress(AbsolutePlusXAddressing), registers()->A() );
			break;
		}
		case Mov_AbsolutePlusY_A:
		{
			writeByte( decodeAddress(AbsolutePlusYAddressing), registers()->A() );
			break;
		}
		case Mov_IndirectDirectPagePlusX_A:
		{
			writeByte( decodeAddress(IndirectDirectPagePlusXAddressing), registers()->A() );
			break;
		}
		case Mov_IndirectDirectPagePlusY_A:
		{
			writeByte( decodeAddress(IndirectDirectPagePlusYAddressing), registers()->A() );
			break;
		}
		case Mov_DirectPage_X:
		{
			writeByte( decodeAddress(DirectPageAddressing), registers()->X() );
			break;
		}
		case Mov_DirectPagePlusY_X:
		{
			writeByte( decodeAddress(DirectPagePlusYAddressing), registers()->X() );
			break;
		}
		case Mov_Absolute_X:
		{
			writeByte( decodeAddress(AbsoluteAddressing), registers()->X() );
			break;
		}
		case Mov_DirectPage_Y:
		{
			writeByte( decodeAddress(DirectPageAddressing), registers()->Y() );
			break;
		}
		case Mov_DirectPagePlusX_Y:
		{
			writeByte( decodeAddress(DirectPagePlusXAddressing), registers()->Y() );
			break;
		}
		case Mov_Absolute_Y:
		{
			writeByte( decodeAddress(AbsoluteAddressing), registers()->Y() );
			break;
		}
		case Mov_A_X:
		{
			setARegister( registers()->X() );
			break;
		}
		case Mov_A_Y:
		{
			setARegister( registers()->Y() );
			break;
		}
		case Mov_Y_A:
		{
			setYRegister( registers()->A() );
			break;
		}
		case Mov_X_A:
		{
			setXRegister( registers()->A() );
			break;
		}
		case Mov_X_SP:
		{
			setXRegister( registers()->stackPointer() );
			break;
		}
		case Mov_SP_X:
		{
			registers()->setStackPointer( registers()->X() );
			break;
		}
		case Mov_DirectPage_DirectPage:
		{
			word tempAddress = decodeAddress(DirectPageAddressing);
			writeByte( tempAddress, readByte( decodeAddress(DirectPageAddressing) ) );
			break;
		}
		case Mov_DirectPage_ImmediateData:
		{
			word tempAddress = decodeAddress(DirectPageAddressing);
			writeByte( tempAddress, readByte() );
			break;
		}
		case Adc_ImmediateData:
		{
			setARegister( addWithCarry( registers()->A(), readByte() ) );
			break;
		}
		case Adc_IndirectX:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			setARegister( addWithCarry( registers()->A(), xValue ) );
			break;
		}
		case Adc_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_DirectPagePlusX:
		{
			byte value = readByte( decodeAddress(DirectPagePlusXAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			setARegister( addWithCarry( registers()->A(), value) );
			break;
		}
		case Adc_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			setARegister( addWithCarry( registers()->A(), value ) );
			break;
		}
		case Adc_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			writeByte( decodeAddress(IndirectXAddressing), addWithCarry(xValue, yValue) );
			break;
		}
		case Adc_DirectPage_DirectPage:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte destinationDpValue = readByte( destination );
			byte sourceDpValue = readByte( decodeAddress(DirectPageAddressing) );
			writeByte( destination, addWithCarry(destinationDpValue, sourceDpValue) );
			break;
		}
		case Adc_DirectPage_ImmediateData:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte dpValue = readByte( destination );
			writeByte( destination, addWithCarry(dpValue, readByte()) );
			break;
		}
		case Sbc_ImmediateData:
		{
			setARegister( subtractWithCarry( registers()->A(), readByte() ) );
			break;
		}
		case Sbc_IndirectX:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			setARegister( subtractWithCarry( registers()->A(), xValue ) );
			break;
		}
		case Sbc_DirectPage:
		{
			byte dpValue = readByte( decodeAddress(DirectPageAddressing) );
			setARegister( subtractWithCarry( registers()->A(), dpValue ) );
			break;
		}
		case Sbc_DirectPagePlusX:
		{
			byte dpValue = readByte( decodeAddress(DirectPagePlusXAddressing) );
			setARegister( subtractWithCarry( registers()->A(), dpValue ) );
			break;
		}
		case Sbc_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			setARegister( subtractWithCarry( registers()->A(), value ) );
			break;
		}
		case Sbc_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			setARegister( subtractWithCarry( registers()->A(), value ) );
			break;
		}
		case Sbc_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			setARegister( subtractWithCarry( registers()->A(), value ) );
			break;
		}
		case Sbc_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			setARegister( subtractWithCarry( registers()->A(), value ) );
			break;
		}
		case Sbc_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			setARegister( subtractWithCarry( registers()->A(), value ) );
			break;
		}
		case Sbc_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			writeByte( decodeAddress(IndirectXAddressing) , subtractWithCarry(xValue, yValue) );
			break;
		}
		case Sbc_DirectPage_DirectPage:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte destinationDpValue = readByte( destination );
			byte sourceDpValue = readByte( decodeAddress(DirectPageAddressing) );
			writeByte( destination, subtractWithCarry(destinationDpValue, sourceDpValue) );
			break;
		}
		case Sbc_DirectPage_ImmediateData:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte dpValue = readByte( destination );
			writeByte( destination, subtractWithCarry(dpValue, readByte()) );
			break;
		}
		case Cmp_A_ImmediateData:
		{
			byte value = readByte();
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_IndirectX:
		{
			byte value = readByte( decodeAddress(IndirectXAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_DirectPagePlusX:
		{
			byte value = readByte( decodeAddress(DirectPagePlusXAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_A_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			compare(registers()->A(), value);
			break;
		}
		case Cmp_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			compare( xValue, yValue );
			break;
		}
		case Cmp_DirectPage_DirectPage:
		{
			byte firstValue = readByte( decodeAddress(DirectPageAddressing) );
			byte secondValue = readByte( decodeAddress(DirectPageAddressing) );
			compare( firstValue, secondValue );
			break;
		}
		case Cmp_DirectPage_ImmediateData:
		{
			byte dpValue = readByte( decodeAddress(DirectPageAddressing) );
			byte immValue = readByte();
			compare( dpValue, immValue );
			break;
		}
		case Cmp_X_ImmediateData:
		{
			byte value = readByte();
			compare( registers()->X(), value );
			break;
		}
		case Cmp_X_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			compare( registers()->X(), value );
			break;
		}
		case Cmp_X_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			compare( registers()->X(), value );
			break;
		}
		case Cmp_Y_ImmediateData:
		{
			byte value = readByte();
			compare( registers()->Y(), value );
			break;
		}
		case Cmp_Y_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			compare( registers()->Y(), value );
			break;
		}
		case Cmp_Y_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			compare( registers()->Y(), value );
			break;
		}
		case And_ImmediateData:
		{
			byte value = readByte();
			setARegister( doAnd( registers()->A(), value ) );
			break;
		}
		case And_IndirectX:
		{
			byte value = readByte( decodeAddress(IndirectXAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_DirectPagePlusX:
		{
			byte value = readByte( decodeAddress(DirectPagePlusXAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			setARegister( doAnd(registers()->A(), value) );
			break;
		}
		case And_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			writeByte( decodeAddress(IndirectXAddressing) , doAnd(xValue, yValue) );
			break;
		}
		case And_DirectPage_DirectPage:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte destinationDpValue = readByte( destination );
			byte sourceDpValue = readByte( decodeAddress(DirectPageAddressing) );
			writeByte( destination, doAnd(destinationDpValue, sourceDpValue) );
			break;
		}
		case And_DirectPage_ImmediateData:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte dpValue = readByte( destination );
			writeByte( destination, doAnd(dpValue, readByte()) );
			break;
		}
		case Or_ImmediateData:
		{
			byte value = readByte();
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_IndirectX:
		{
			byte value = readByte( decodeAddress(IndirectXAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_DirectPagePlusX:
		{
			byte value = readByte( decodeAddress(DirectPagePlusXAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			setARegister( doOr(registers()->A(), value) );
			break;
		}
		case Or_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			writeByte( decodeAddress(IndirectXAddressing) , doOr(xValue, yValue) );
			break;
		}
		case Or_DirectPage_DirectPage:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte destinationDpValue = readByte( destination );
			byte sourceDpValue = readByte( decodeAddress(DirectPageAddressing) );
			writeByte( destination, doOr(destinationDpValue, sourceDpValue) );
			break;
		}
		case Or_DirectPage_ImmediateData:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte dpValue = readByte( destination );
			writeByte( destination, doOr(dpValue, readByte()) );
			break;
		}
		case Eor_ImmediateData:
		{
			byte value = readByte();
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_IndirectX:
		{
			byte value = readByte( decodeAddress(IndirectXAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_DirectPage:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_DirectPagePlusX:
		{
			byte value = readByte( decodeAddress(DirectPagePlusXAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_Absolute:
		{
			byte value = readByte( decodeAddress(AbsoluteAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_AbsolutePlusX:
		{
			byte value = readByte( decodeAddress(AbsolutePlusXAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_AbsolutePlusY:
		{
			byte value = readByte( decodeAddress(AbsolutePlusYAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_IndirectDirectPagePlusX:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusXAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_IndirectDirectPagePlusY:
		{
			byte value = readByte( decodeAddress(IndirectDirectPagePlusYAddressing) );
			setARegister( doEor(registers()->A(), value) );
			break;
		}
		case Eor_IndirectXY:
		{
			byte xValue = readByte( decodeAddress(IndirectXAddressing) );
			byte yValue = readByte( decodeAddress(IndirectYAddressing) );
			writeByte( decodeAddress(IndirectXAddressing) , doEor(xValue, yValue) );
			break;
		}
		case Eor_DirectPage_DirectPage:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte destinationDpValue = readByte( destination );
			byte sourceDpValue = readByte( decodeAddress(DirectPageAddressing) );
			writeByte( destination, doEor(destinationDpValue, sourceDpValue) );
			break;
		}
		case Eor_DirectPage_ImmediateData:
		{
			word destination = decodeAddress(DirectPageAddressing);
			byte dpValue = readByte( destination );
			writeByte( destination, doEor(dpValue, readByte()) );
			break;
		}
		case Inc_A:
			setARegister( registers()->A() + 1 );
			break;
		case Inc_X:
			setXRegister( registers()->X() + 1 );
			break;
		case Inc_Y:
			setYRegister( registers()->Y() + 1 );
			break;
		case Inc_DirectPage:
		{
			word tempAddress = decodeAddress(DirectPageAddressing);
			byte incValue = readByte(tempAddress)+1;
			writeByte( tempAddress, incValue );

			updateZeroFlag( incValue );
			updateNegativeFlag( incValue );
			break;
		}
		case Inc_DirectPagePlusX:
		{
			word tempAddress = decodeAddress(DirectPagePlusXAddressing);
			byte incValue = readByte(tempAddress)+1;
			writeByte( tempAddress, incValue );

			updateZeroFlag( incValue );
			updateNegativeFlag( incValue );
			break;
		}
		case Inc_Absolute:
		{
			word tempAddress = decodeAddress(AbsoluteAddressing);
			byte incValue = readByte(tempAddress)+1;
			writeByte( tempAddress, incValue );

			updateZeroFlag( incValue );
			updateNegativeFlag( incValue );
			break;
		}
		case Dec_A:
			setARegister( registers()->A() - 1 );
			break;
		case Dec_X:
			setXRegister( registers()->X() - 1 );
			break;
		case Dec_Y:
			setYRegister( registers()->Y() - 1 );
			break;
		case Dec_DirectPage:
		{
			word tempAddress = decodeAddress(DirectPageAddressing);
			byte decValue = readByte(tempAddress)-1;
			writeByte( tempAddress, decValue );

			updateZeroFlag( decValue );
			updateNegativeFlag( decValue );
			break;
		}
		case Dec_DirectPagePlusX:
		{
			word tempAddress = decodeAddress(DirectPagePlusXAddressing);
			byte decValue = readByte(tempAddress)-1;
			writeByte( tempAddress, decValue );

			updateZeroFlag( decValue );
			updateNegativeFlag( decValue );
			break;
		}
		case Dec_Absolute:
		{
			word tempAddress = decodeAddress(AbsoluteAddressing);
			byte decValue = readByte(tempAddress)-1;
			writeByte( tempAddress, decValue );

			updateZeroFlag( decValue );
			updateNegativeFlag( decValue );
			break;
		}
		case Asl_A:
		{
			setARegister( doAsl(registers()->A()) );
			break;
		}
		case Asl_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doAsl(tempValue) );
			break;
		}
		case Asl_DirectPagePlusX:
		{
			word dpAddress = decodeAddress(DirectPagePlusXAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doAsl(tempValue) );
			break;
		}
		case Asl_Absolute:
		{
			word address = decodeAddress(AbsoluteAddressing);
			byte tempValue = readByte( address );
			writeByte( address, doAsl(tempValue) );
			break;
		}
		case Lsr_A:
		{
			setARegister( doLsr(registers()->A()) );
			break;
		}
		case Lsr_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doLsr(tempValue) );
			break;
		}
		case Lsr_DirectPageX:
		{
			word dpAddress = decodeAddress(DirectPagePlusXAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doAsl(tempValue) );
			break;
		}
		case Lsr_Absolute:
		{
			word address = decodeAddress(AbsoluteAddressing);
			byte tempValue = readByte( address );
			writeByte( address, doAsl(tempValue) );
			break;
		}
		case Rol_A:
		{
			setARegister( doRol(registers()->A()) );
			break;
		}
		case Rol_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doRol(tempValue) );
			break;
		}
		case Rol_DirectPagePlusX:
		{
			word dpAddress = decodeAddress(DirectPagePlusXAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doRol(tempValue) );
			break;
		}
		case Rol_Absolute:
		{
			word address = decodeAddress(AbsoluteAddressing);
			byte tempValue = readByte( address );
			writeByte( address, doRol(tempValue) );
			break;
		}
		case Ror_A:
		{
			setARegister( doRor(registers()->A()) );
			break;
		}
		case Ror_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doRor(tempValue) );
			break;
		}
		case Ror_DirectPagePlusX:
		{
			word dpAddress = decodeAddress(DirectPagePlusXAddressing);
			byte tempValue = readByte( dpAddress );
			writeByte( dpAddress, doRor(tempValue) );
			break;
		}
		case Ror_Absolute:
		{
			word address = decodeAddress(AbsoluteAddressing);
			byte tempValue = readByte( address );
			writeByte( address, doRor(tempValue) );
			break;
		}
		case Xcn_A:
		{
			byte tempAValue = registers()->A();
			setARegister( (tempAValue >>4) | (tempAValue<<4) );
			break;
		}
		case Movw_YA_DirectPage:
		{
			word value = readWord( decodeAddress(DirectPageAddressing) );
			registers()->setYA( value );

			updateZeroFlag( byte(value) );
			updateNegativeFlag( byte(value) );
			break;
		}
		case Movw_DirectPage_YA:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			writeWord( dpAddress, registers()->YA() );
			break;
		}
		case Incw_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			word value = readWord( decodeAddress(DirectPageAddressing) ) + 1;
			writeWord( dpAddress, value );

			updateZeroFlag( byte(value) );
			updateNegativeFlag( byte(value) );
			break;
		}
		case Decw_DirectPage:
		{
			word dpAddress = decodeAddress(DirectPageAddressing);
			word value = readWord( decodeAddress(DirectPageAddressing) ) - 1;
			writeWord( dpAddress, value );

			updateZeroFlag( byte(value) );
			updateNegativeFlag( byte(value) );
			break;
		}
		case Addw_YA_DirectPage:
		{
			word dpValue = readWord( decodeAddress(DirectPageAddressing) );
			int result = dpValue + registers()->YA();
		
			registers()->setYA( word(result) );

			// If the result is within the range of a byte(0x0-0xFF)
			// Clear the carry, else set the carry
			if( result > 0xFFFF )
			{
				setProgramStatusFlag(CarryFlag);
			}
			else
			{
				removeProgramStatusFlag(CarryFlag);
			}

			removeProgramStatusFlag(HalfCarryFlag);
			if( registers()->YA() ^ dpValue ^ static_cast<word>(result) & 0x1000 )
			{
				setProgramStatusFlag(HalfCarryFlag);
			}

			updateZeroFlag( byte(result) );
			updateNegativeFlag( byte(result) );
			updateOverflowFlag( result );

			break;
		}
		case Subw_YA_DirectPage:
		{
			word dpValue = readWord( decodeAddress(DirectPageAddressing) );
			int result = registers()->YA() - dpValue;
		
			registers()->setYA( result );

			if( result > 0xFFFF )
			{
				removeProgramStatusFlag(CarryFlag);
			}
			else
			{
				setProgramStatusFlag(CarryFlag);
			}

			setProgramStatusFlag(HalfCarryFlag);
			if( registers()->YA() ^ dpValue ^ static_cast<word>(result) & 0x1000 )
			{
				removeProgramStatusFlag(HalfCarryFlag);
			}

			updateZeroFlag( byte(result) );
			updateNegativeFlag( byte(result) );
			updateOverflowFlag( result );

			break;
		}
		case Cmpw_YA_DirectPage:
		{
			word dpValue = readWord( decodeAddress(DirectPageAddressing) );
			short result = static_cast<short>(registers()->YA()) - static_cast<short>(dpValue);
			if( result >= 0 )
			{
				setProgramStatusFlag(CarryFlag);
			}
			else
			{
				removeProgramStatusFlag(CarryFlag);
			}

			updateZeroFlag( byte(result) );
			updateNegativeFlag( byte(result) );
			break;
		}
		case Mul:
		{
			//YA <- Y*A
			word result = registers()->Y() * registers()->A();
			registers()->setYA( result );
			
			updateZeroFlag( byte(result) );
			updateNegativeFlag( byte(result) );
			break;
		}
		case Div:
		{
			//  Y <- YA % X and A <- YA / X
			word tempYA = registers()->YA();
			registers()->setY( tempYA % registers()->X() );
			registers()->setA( tempYA / registers()->X() );

			if( (registers()->X() & 0x0f) <= (registers()->Y() & 0x0f) )
			{
				setProgramStatusFlag(HalfCarryFlag);
			}
			else
			{
				removeProgramStatusFlag(HalfCarryFlag);
			}
			if( registers()->YA() & 0x100 )
			{
				setProgramStatusFlag(OverflowFlag);
			}
			else
			{
				removeProgramStatusFlag(OverflowFlag);
			}

			updateZeroFlag( registers()->A() );
			updateNegativeFlag( registers()->A() );
			break;
		}
		case Daa:
		{
			if( registers()->A() > 0x99 || isProgramStatusFlagSet(CarryFlag) )
			{
				setARegister( registers()->A() + 0x60 );
				setProgramStatusFlag(CarryFlag);
			}
			else
			{
				removeProgramStatusFlag(CarryFlag);
			}

			if( (registers()->A() & 0x0f) > 9 || isProgramStatusFlagSet(HalfCarryFlag) )
			{
				setARegister( registers()->A() + 6 );
			}
			else
			{
				removeProgramStatusFlag(HalfCarryFlag);
			}
			break;
		}
		case Das:
		{
			if( registers()->A() > 0x99 || !isProgramStatusFlagSet(CarryFlag) )
			{
				setARegister( registers()->A() - 0x60 );
				removeProgramStatusFlag(CarryFlag);
			}
			else
			{
				setProgramStatusFlag(CarryFlag);
			}
			
			if( (registers()->A() & 0x0f) > 9 || !isProgramStatusFlagSet(HalfCarryFlag) )
			{
				setARegister( registers()->A() - 6 );
			}
			break;
		}
		case Bra_BranchAlways:
		{
			registers()->setProgramCounter( decodeAddress(RelativeAddressing) );
			break;
		}
		case Beq_BranchZ1:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( isProgramStatusFlagSet(ZeroFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bne_BranchZ0:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( !isProgramStatusFlagSet(ZeroFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bcs_BranchC1:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( isProgramStatusFlagSet(CarryFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bcc_BranchC0:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( !isProgramStatusFlagSet(CarryFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bvs_BranchV1:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( isProgramStatusFlagSet(OverflowFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bvc_BranchV0:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( !isProgramStatusFlagSet(OverflowFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bmi_BranchN1:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( isProgramStatusFlagSet(NegativeFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bpl_BranchN0:
		{
			word newPc = decodeAddress(RelativeAddressing);
			if( !isProgramStatusFlagSet(NegativeFlag) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit0:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(0, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit1:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(1, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit2:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(2, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit3:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(3, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit4:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(4, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit5:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(5, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit6:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(6, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbc_BranchBit7:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( !isBitSet(7, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit0:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(0, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit1:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(1, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit2:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(2, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit3:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(3, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit4:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(4, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit5:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(5, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit6:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(6, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Bbs_BranchBit7:
		{
			byte value = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( isBitSet(7, value) )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Cbne_DirectPage:
		{
			byte dpValue = readByte( decodeAddress(DirectPageAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( registers()->A() != dpValue )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Cbne_DirectPagePlusX:
		{
			byte dpValue = readByte( decodeAddress(DirectPagePlusXAddressing) );
			word newPc = decodeAddress(RelativeAddressing);
			if( registers()->A() != dpValue )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Dbnz_DirectPage:
		{
			byte dpAddress = decodeAddress(DirectPageAddressing);
			
			byte dpValue = readByte(dpAddress);
			writeByte(dpAddress, --dpValue);
			
			word newPc = decodeAddress(RelativeAddressing);
			
			if( dpValue != 0 )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Dbnz_Y:
		{
			word newPc = decodeAddress(RelativeAddressing);
			setYRegister( registers()->Y() - 1 );
			
			if( registers()->Y() != 0 )
			{
				registers()->setProgramCounter( newPc );
			}
			break;
		}
		case Jmp:
		{
			registers()->setProgramCounter( readWord() );
			break;
		}
		case Jmp_X:
		{
			word newPc = readWord( readWord() + registers()->X() );
			registers()->setProgramCounter( newPc );
			break;
		}
		case Call:
		{
			word callAddress = readWord();

			doPushWord( registers()->programCounter() );

			registers()->setProgramCounter( callAddress );
			break;
		}
		case Pcall:
		{
			byte pageCall = readByte();

			doPushWord( registers()->programCounter() );

			word addressToCall;
			addressToCall.setHighByte( 0xff );
			addressToCall.setLowByte( pageCall );

			registers()->setProgramCounter( addressToCall );
			break;
		}
		case Tcall0:
		{
			doTcall(0);
			break;
		}
		case Tcall1:
		{
			doTcall(1);
			break;
		}
		case Tcall2:
		{
			doTcall(2);
			break;
		}
		case Tcall3:
		{
			doTcall(3);
			break;
		}
		case Tcall4:
		{
			doTcall(4);
			break;
		}
		case Tcall5:
		{
			doTcall(5);
			break;
		}
		case Tcall6:
		{
			doTcall(6);
			break;
		}
		case Tcall7:
		{
			doTcall(7);
			break;
		}
		case Tcall8:
		{
			doTcall(8);
			break;
		}
		case Tcall9:
		{
			doTcall(9);
			break;
		}
		case TcallA:
		{
			doTcall( 0xA );
			break;
		}
		case TcallB:
		{
			doTcall( 0xB );
			break;
		}
		case TcallC:
		{
			doTcall( 0xC );
			break;
		}
		case TcallD:
		{
			doTcall( 0xD );
			break;
		}
		case TcallE:
		{
			doTcall( 0xE );
			break;
		}
		case TcallF:
		{
			doTcall( 0xF );
			break;
		}
		case Brk:
		{
			doPushWord( registers()->programCounter() );
			doPush( registers()->programStatus() );

			setProgramStatusFlag( BreakFlag );
			removeProgramStatusFlag( InterruptFlag );

			word breakAddress;
			breakAddress.setHighByte( readByte(0xffdf) );
			breakAddress.setLowByte( readByte(0xffde) );

			registers()->setProgramCounter( breakAddress );
			break;
		}
		case Ret:
		{
			word returnAddress = doPopWord();

			registers()->setProgramCounter(returnAddress);
			break;
		}
		case RetI:
		{
			registers()->setProgramStatus( doPop() );
			registers()->setProgramCounter( doPopWord() );
			break;
		}
		case Push_A:
		{
			doPush( registers()->A() );
			break;
		}
		case Push_X:
		{
			doPush( registers()->X() );
			break;
		}
		case Push_Y:
		{
			doPush( registers()->Y() );
			break;
		}
		case Push_Psw:
		{
			doPush( registers()->programStatus() );
			break;
		}
		case Pop_A:
		{
			registers()->setA( doPop() );
			break;
		}
		case Pop_X:
		{
			registers()->setX( doPop() );
			break;
		}
		case Pop_Y:
		{
			registers()->setY( doPop() );
			break;
		}
		case Pop_Psw:
		{
			registers()->setProgramStatus( doPop() );
			break;
		}
		case Set0:
		{
			doSetBit(0);
			break;
		}
		case Set1:
		{
			doSetBit(1);
			break;
		}
		case Set2:
		{
			doSetBit(2);
			break;
		}
		case Set3:
		{
			doSetBit(3);
			break;
		}
		case Set4:
		{
			doSetBit(4);
			break;
		}
		case Set5:
		{
			doSetBit(5);
			break;
		}
		case Set6:
		{
			doSetBit(6);
			break;
		}
		case Set7:
		{
			doSetBit(7);
			break;
		}
		case Clr0:
		{
			doClearBit(0);
			break;
		}
		case Clr1:
		{
			doClearBit(1);
			break;
		}
		case Clr2:
		{
			doClearBit(2);
			break;
		}
		case Clr3:
		{
			doClearBit(3);
			break;
		}
		case Clr4:
		{
			doClearBit(4);
			break;
		}
		case Clr5:
		{
			doClearBit(5);
			break;
		}
		case Clr6:
		{
			doClearBit(6);
			break;
		}
		case Clr7:
		{
			doClearBit(7);
			break;
		}
		case Tset1:
		{
			word absAddress = decodeAddress(AbsoluteAddressing);
			byte tempByte = readByte(absAddress);
			
			writeByte( absAddress, tempByte | registers()->A() );
			
			tempByte = registers()->A() - tempByte;
			updateZeroFlag( tempByte );
			updateNegativeFlag( tempByte );
			break;
		}
		case Tclr1:
		{
			word absAddress = decodeAddress(AbsoluteAddressing);
			byte tempByte = readByte(absAddress);
			
			writeByte( absAddress, tempByte & ~registers()->A() );
			
			tempByte = registers()->A() - tempByte;
			updateZeroFlag( tempByte );
			updateNegativeFlag( tempByte );
			break;
		}
		case And1:
		{
			MemBitData data = getMemBitData();
			if( isProgramStatusFlagSet(CarryFlag) )
			{
				if( !(readByte(data.address) & (1<<data.bit)) )
				{
					removeProgramStatusFlag(CarryFlag);
				}
			}
			break;
		}
		case And1_Not:
		{
			MemBitData data = getMemBitData();
			if( isProgramStatusFlagSet(CarryFlag) )
			{
				if( readByte(data.address) & (1<<data.bit) )
				{
					removeProgramStatusFlag(CarryFlag);
				}
			}
			break;
		}
		case Or1:
		{
			MemBitData data = getMemBitData();
			if( !isProgramStatusFlagSet(CarryFlag) )
			{
				if( readByte(data.address) & (1<<data.bit) )
				{
					setProgramStatusFlag(CarryFlag);
				}
			}
			break;
		}
		case Or1_Not:
		{
			MemBitData data = getMemBitData();
			if( !isProgramStatusFlagSet(CarryFlag) )
			{
				if( !(readByte(data.address) & (1<<data.bit)) )
				{
					setProgramStatusFlag(CarryFlag);
				}
			}
			break;
		}
		case Eor1:
		{
			MemBitData data = getMemBitData();
			if( readByte(data.address) & (1<<data.bit) )
			{
				if( isProgramStatusFlagSet(CarryFlag) )
				{
					removeProgramStatusFlag(CarryFlag);
				}
				else
				{
					setProgramStatusFlag(CarryFlag);
				}
			}
			break;
		}
		case Not1:
		{
			MemBitData data = getMemBitData();

			byte complement = readByte(data.address) ^ (1 << data.bit);

			writeByte(data.address, complement);
			break;
		}
		case Mov1_Store:
		{
			MemBitData data = getMemBitData();
			if( readByte(data.address) & (1<<data.bit) )
			{
				setProgramStatusFlag(CarryFlag);
			}
			else
			{
				removeProgramStatusFlag(CarryFlag);
			}
			break;
		}
		case Mov1_Read:
		{
			MemBitData data = getMemBitData();
			byte tempByte = readByte(data.address);

			if( isProgramStatusFlagSet(CarryFlag) )
			{
				tempByte |= (1 << data.bit);
				
			}
			else
			{
				tempByte &= ~(1 << data.bit);
			}

			writeByte( data.address, tempByte );
			break;
		}
		case Clrc:
		{
			removeProgramStatusFlag(CarryFlag);
			break;
		}
		case Setc:
		{
			setProgramStatusFlag(CarryFlag);
			break;
		}
		case Notc:
		{
			isProgramStatusFlagSet(CarryFlag) ? removeProgramStatusFlag(CarryFlag) : setProgramStatusFlag(CarryFlag);
			break;
		}
		case Clrv:
		{
			removeProgramStatusFlag(OverflowFlag);
			removeProgramStatusFlag(HalfCarryFlag);
			break;
		}
		case Clrp:
		{
			removeProgramStatusFlag(DirectPageFlag);
			break;
		}
		case Setp:
		{
			setProgramStatusFlag(DirectPageFlag);
			break;
		}
		case Ei:
		{
			setProgramStatusFlag(InterruptFlag);
			break;
		}
		case Di:
		{
			removeProgramStatusFlag(InterruptFlag);
			break;
		}
		case Nop:
			break;
		case Sleep:
			// TODO:
			break;
		case Stop:
			// TODO:
			break;
		default:
			lLog() << "Unknow opcode" << opcode << "caught at" << registers()->programCounter()-1;
			break;
	}
}

SpcRunner *Processor::runner() const
{
	return d->runner;
}

ProcessorRegisters *Processor::registers() const
{
	return d->regs;
}

byte Processor::readByte()
{
	byte readByte = runner()->memory()->readByte( registers()->programCounter() );
	registers()->incrementProgramCounter();
	d->lastAddress = registers()->programCounter();
	return readByte;
}

byte Processor::readByte(word address) const
{
	d->lastAddress = address;
	
	return runner()->memory()->readByte( address );
}

word Processor::readWord()
{
	word readWord = runner()->memory()->readWord( registers()->programCounter() );
	// Increment 2 time the program counter
	// because a word is 2 bytes (aka 16bit)
	registers()->setProgramCounter( registers()->programCounter() + 2);
	
	d->lastAddress = registers()->programCounter();
	return readWord;
}

word Processor::readWord(word address) const
{
	d->lastAddress = address;
	
	return runner()->memory()->readWord( address );
}

void Processor::writeByte(word address, byte value)
{
	d->lastAddress = address;
	
	runner()->memory()->writeByte(address, value);
}

void Processor::writeWord(word address, word value)
{
	d->lastAddress = address;
	
	runner()->memory()->writeWord(address, value);
}

word Processor::directPageAddress(byte dpIndex) const
{
	word dpAddress = 0;
	// If the direct page flag is set
	// read from direct page page 1 (0x100-0x1ff)
	// else read from direct page 0 (0x0-0xff)
	if( isProgramStatusFlagSet(DirectPageFlag) )
	{
		dpAddress.setHighByte(0x01);
		dpAddress.setLowByte( dpIndex );
	}
	else
	{
		dpAddress = dpIndex;
	}
	
	return dpAddress;
}

byte Processor::addWithCarry(byte v1, byte v2)
{
	int result = v1 + v2;
	if( isProgramStatusFlagSet(CarryFlag) )
	{
		result++;
	}

	// If the result is within the range of a byte(0x0-0xFF)
	// Clear the carry, else set the carry
	if( result > 0xFF )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	updateZeroFlag( byte(result) );
	updateNegativeFlag( byte(result) );
	updateOverflowFlag( result );

	removeProgramStatusFlag(HalfCarryFlag);
	if( v1 ^ v2 ^ static_cast<uint8>(result) & 0x10 )
	{
		setProgramStatusFlag(HalfCarryFlag);
	}

	return byte(result);
}

byte Processor::subtractWithCarry(byte v1, byte v2)
{
	int result = v1 - v2;
	result = result + (int)!isProgramStatusFlagSet(CarryFlag);

	if( static_cast<word>(result).highByte() )
	{
		removeProgramStatusFlag(CarryFlag);
	}
	else
	{
		setProgramStatusFlag(CarryFlag);
	}

	setProgramStatusFlag(HalfCarryFlag);
	if( v1 ^ v2 ^ static_cast<uint8>(result) & 0x10 )
	{
		removeProgramStatusFlag(HalfCarryFlag);
	}

	updateZeroFlag( byte(result) );
	updateNegativeFlag( byte(result) );
	updateOverflowFlag( result );

	return byte(result);
}

void Processor::compare(byte v1, byte v2)
{
	short result = static_cast<short>(v1) - static_cast<short>(v2);
	if( result >= 0 )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	updateZeroFlag( byte(result) );
	updateNegativeFlag( byte(result) );
	updateOverflowFlag( result );
}

byte Processor::doAnd(byte v1, byte v2)
{
	byte result = v1 & v2;

	updateNegativeFlag( result );
	updateZeroFlag( result );

	return result;
}

byte Processor::doOr(byte v1, byte v2)
{
	byte result = v1 | v2;

	updateNegativeFlag( result );
	updateZeroFlag( result );

	return result;
}

byte Processor::doEor(byte v1, byte v2)
{
	byte result = v1 ^ v2;

	updateNegativeFlag( result );
	updateZeroFlag( result );

	return result;
}

void Processor::setProgramStatusFlag(ProgramStatusFlags value)
{
	registers()->setProgramStatus( registers()->programStatus() | value );
}

void Processor::removeProgramStatusFlag(ProgramStatusFlags value)
{
	registers()->setProgramStatus( registers()->programStatus() & ~value );
}

bool Processor::isProgramStatusFlagSet(ProgramStatusFlags value) const
{
	return registers()->programStatus() & value;
}

void Processor::setARegister(byte value)
{
	registers()->setA( value );
	updateZeroFlag( value );
	updateNegativeFlag( value );
}

void Processor::setXRegister(byte value)
{
	registers()->setX( value );
	updateZeroFlag( value );
	updateNegativeFlag( value );
}

void Processor::setYRegister(byte value)
{
	registers()->setY( value );
	updateZeroFlag( value );
	updateNegativeFlag( value );
}

void Processor::updateZeroFlag(byte value)
{
	if( value == 0x0 )
	{
		setProgramStatusFlag( ZeroFlag );
	}
	else
	{
		removeProgramStatusFlag( ZeroFlag );
	}
}

void Processor::updateNegativeFlag(byte value)
{
	// 0x80 is 1000 0000, to check if the number is negative
	// A negative number has bit 7 set
	if( value & 0x80 )
	{
		setProgramStatusFlag( NegativeFlag );
	}
	else
	{
		removeProgramStatusFlag( NegativeFlag );
	}
}

void Processor::updateOverflowFlag(int value)
{
	// Overflow flag simule carry for signed numbers
	// 0x7f is 127
	if( value < -127 || value > 127 )
	{
		setProgramStatusFlag( OverflowFlag );
	}
	else
	{
		removeProgramStatusFlag( OverflowFlag );
	}
}

word Processor::decodeAddress(Processor::AddressingMode mode)
{
	word address = 0;

	switch(mode)
	{
		case IndirectXAddressing:
			address = directPageAddress( registers()->X() );
			break;
		case IndirectYAddressing:
			address = directPageAddress( registers()->Y() );
			break;
		case DirectPageAddressing:
			address = directPageAddress( readByte() );
			break;
		case DirectPagePlusXAddressing:
			address = directPageAddress( readByte() ) + registers()->X();
			break;
		case DirectPagePlusYAddressing:
			address = directPageAddress( readByte() ) + registers()->Y();
			break;
		case AbsoluteAddressing:
			address = readWord();
			break;
		case AbsolutePlusXAddressing:
			address = readWord() + registers()->X();
			break;
		case AbsolutePlusYAddressing:
			address = readWord() + registers()->Y();
			break;
		case IndirectDirectPagePlusXAddressing:
		{
			address = readWord( directPageAddress(readByte()) + registers()->X() );
			break;
		}
		case IndirectDirectPagePlusYAddressing:
			address = readWord( directPageAddress(readByte()) ) + registers()->Y();
			break;
		case RelativeAddressing:
		{
			offset relativeAddress = static_cast<offset>( readByte() );
			address = static_cast<word>(registers()->programCounter() + relativeAddress);
			break;
		}
		default:
			lWarning() << "Invalid addressing mode.";
			break;
	}

	lDebug() << "Result address:" << address;

	return address;
}

bool Processor::isBitSet(int bit, byte value) const
{
	int bitToTest = 1 << bit;

	return value & bitToTest;
}

byte Processor::doAsl(byte value)
{
	if( (value & 0x80) != 0 )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	value <<= 1;

	updateZeroFlag(value);
	updateNegativeFlag(value);

	return value;
}

byte Processor::doLsr(byte value)
{
	if( value & 1 )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	value >>= 1;

	updateZeroFlag(value);
	updateNegativeFlag(value);

	return value;
}

byte Processor::doRol(byte value)
{
	word temp = (value << 1) | static_cast<int>( isProgramStatusFlagSet(CarryFlag) );

	if( temp >= 0x100 )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	value = static_cast<byte>(temp);

	updateZeroFlag(value);
	updateNegativeFlag(value);

	return value;
}

byte Processor::doRor(byte value)
{
	word temp = value | (static_cast<uint16>(isProgramStatusFlagSet(CarryFlag)) << 8);
	
	if( temp & 1 )
	{
		setProgramStatusFlag(CarryFlag);
	}
	else
	{
		removeProgramStatusFlag(CarryFlag);
	}

	value = static_cast<byte>(temp);
	
	updateZeroFlag(value);
	updateNegativeFlag(value);

	return value;
}

void Processor::doPush(byte value)
{
	word stackAddress;
	stackAddress.setHighByte( 0x01 );
	stackAddress.setLowByte( registers()->stackPointer() );

	writeByte( stackAddress, value );

	registers()->decrementStackPointer();
}

void Processor::doPushWord(word value)
{
	doPush( value.lowByte() );
	doPush( value.highByte() );
}

byte Processor::doPop()
{
	registers()->incrementStackPointer();

	word stackAddress;
	stackAddress.setHighByte( 0x01 );
	stackAddress.setLowByte( registers()->stackPointer() );

	byte popedValue = readByte( stackAddress );

	return popedValue;
}

word Processor::doPopWord()
{
	word poppedValue;

	poppedValue.setHighByte( doPop() );
	poppedValue.setLowByte( doPop() );

	return poppedValue;
}

void Processor::doTcall(byte tableIndex)
{
	doPushWord( registers()->programCounter() );

	word tableAddress;
	tableAddress.setLowByte( 0xffc0 + ((15 - tableIndex) << 1) );
	tableAddress.setHighByte( 0xffc1 + ((15 - tableIndex) << 1) );

	registers()->setProgramCounter( tableAddress );
}

void Processor::doSetBit(int bit)
{
	word dpAddress = decodeAddress(DirectPageAddressing);
	byte tempByte = readByte( dpAddress );

	tempByte |= 1<<bit;

	writeByte( dpAddress, tempByte );
}

void Processor::doClearBit(int bit)
{
	word dpAddress = decodeAddress(DirectPageAddressing);
	byte tempByte = readByte( dpAddress );

	tempByte = tempByte & ~(1<<bit);

	writeByte( dpAddress, tempByte );
}

MemBitData Processor::getMemBitData()
{
	MemBitData data;

	data.address = readWord();
	data.bit = static_cast<byte>( data.address >> 13 );
	data.address = data.address & 0x1fff;

	return data;
}

}
