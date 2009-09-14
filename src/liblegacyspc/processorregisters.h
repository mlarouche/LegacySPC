/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright 2007 by Michaël Larouche <larouche@kde.org>
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
#ifndef LEGACYSPC_PROCESSORREGISTERS_H
#define LEGACYSPC_PROCESSORREGISTERS_H

#include <types.h>

namespace LegacySPC
{

/**
 * @brief Status flags for the program status ward CPU registers
 */
enum ProgramStatusFlags
{
	CarryFlag = 0x1, ///< Carry flag (C)
	ZeroFlag = 0x2, ///< Zero flag (Z)
	InterruptFlag = 0x4 , ///< Interrupt Enable Flag, not used (I)
	HalfCarryFlag = 0x8, ///< Half carry flag (H)
	BreakFlag = 0x10, ///< Break flag (B)
	DirectPageFlag = 0x20, ///< Direct page flag (P)
	OverflowFlag = 0x40, ///< Overflow flag (V)
	NegativeFlag = 0x80 ///< Negative flag (N)
};

/**
 * @brief Manage CPU registers
 */
class ProcessorRegisters
{
public:
	ProcessorRegisters()
	{
		m_X = m_YA = m_programStatus = m_stackPointer = 0;
		m_programCounter = 0;
	}

	void loadRegisters(const ProcessorRegisters &loadRegs)
	{
		m_X = loadRegs.m_X;
		m_YA = loadRegs.m_YA;
		m_programStatus = loadRegs.m_programStatus;
		m_stackPointer = loadRegs.m_stackPointer;
		m_programCounter = loadRegs.m_programCounter;
	}

	byte X() const
	{
		return m_X;
	}

	byte Y() const
	{
		return m_YA.highByte();
	}

	byte A() const
	{
		return m_YA.lowByte();
	}

	word YA() const
	{
		return m_YA;
	}

	byte stackPointer() const
	{
		return m_stackPointer;
	}

	word programCounter() const
	{
		return m_programCounter;
	}

	byte programStatus() const
	{
		return m_programStatus;
	}

	void setX(byte value)
	{
		m_X = value;
	}

	void setY(byte value)
	{
		m_YA.setHighByte( value );
	}
	
	void setA(byte value)
	{
		m_YA.setLowByte( value );
	}

	void setYA(word value)
	{
		m_YA = value;
	}

	void setStackPointer(byte value)
	{
		m_stackPointer = value;
	}

	void setProgramCounter(word value)
	{
		m_programCounter = value;
	}

	void setProgramStatus(byte value)
	{
		m_programStatus = value;
	}

	void incrementA()
	{
		byte tempA = A();
		setA( ++tempA );
	}
	
	void incrementY()
	{
		byte tempY = Y();
		setY( ++tempY );
	}
	
	void incrementX()
	{
		++m_X;
	}

	void incrementProgramCounter()
	{
		++m_programCounter;
	}

	

	void incrementStackPointer()
	{
		++m_stackPointer;
	}

	void decrementA()
	{
		byte tempA = A();
		setA( --tempA );
	}
	
	void decrementX()
	{
		--m_X;
	}

	void decrementY()
	{
		byte tempY = Y();
		setY( --tempY );
	}

	void decrementProgramCounter()
	{
		--m_programCounter;
	}

	void decrementStackPointer()
	{
		--m_stackPointer;
	}

private:
	// Program status ward
	byte m_programStatus;

	// Paired YA registers, use A() and Y() to get the component
	word m_YA;

	// X register
	byte m_X;

	// Stack pointer index
	byte m_stackPointer;

	// Program counter
	word m_programCounter;
};

}

#endif
