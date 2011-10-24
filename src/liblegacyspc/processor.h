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
#ifndef LEGACYSPC_PROCESSOR_H
#define LEGACYSPC_PROCESSOR_H

#include <legacyspc_export.h>
#include <types.h>
#include <processorregisters.h>

namespace LegacySPC
{

class SpcRunner;
struct MemBitData;

/**
 * @brief Sony SPC700 CPU Emulator
 */
class LEGACYSPC_EXPORT Processor
{
public:
	/**
	 * @brief Create a new instance of Processor
	 */
	Processor(SpcRunner *runner);
	/**
	 * @brief Delete the current instance of Processor
	 */
	~Processor();

	/**
	 * @brief Process the next opcode from memory
	 */
	void processOpcode();
	
	/**
	 * @brief Get the CPU registers.
	 * @return ProcessorRegisters struct
	 */
	ProcessorRegisters *registers() const;
	
	/**
	 * @brief Check if the given status flag is set in program status.
	 * @param value Flag to check
	 * @return true if the flag is set
	 */
	bool isProgramStatusFlagSet(ProgramStatusFlags value) const;

	/**
	 * @brief Get last read or written address by the processor
	 *
	 * Used by the memory viewer to view memory data used by the processor
	 *
	 * @return last address
	 */
	word lastAddress() const;
	
private:
	enum AddressingMode
	{
		IndirectXAddressing,
		IndirectYAddressing,
		DirectPageAddressing,
		DirectPagePlusXAddressing,
		DirectPagePlusYAddressing,
		AbsoluteAddressing,
		AbsolutePlusXAddressing,
		AbsolutePlusYAddressing,
		IndirectDirectPagePlusXAddressing,
		IndirectDirectPagePlusYAddressing,
		RelativeAddressing
	};

private:
	/**
	 * @internal
	 * @brief Get the instance of SpcRunner
	 * @return instance of SpcRunner
	 */
	SpcRunner *runner() const;

	/**
	 * @internal
	 * @brief Read a byte from memory and increment
	 * the program counter.
	 * @return byte read from memory.
	 */
	byte readByte();

	/**
	 * @internal
	 * @brief Read a byte from memory from the given
	 * address.
	 *
	 * Unlike the previous method, this method does not
	 * increment the program counter.
	 * @param address Address to read from
	 * @return read byte from memory
	 */
	byte readByte(word address) const;

	/**
	 * @internal
	 * @brief Read a word from memory and incremnet
	 * the program counter.
	 * @return word read from memory.
	 */
	word readWord();

	/**
	 * @internal
	 * @brief Read a word from memory from the given
	 * address.
	 *
	 * Unlike the previous method, this method does not
	 * increment the program counter.
	 *
	 * @param address Address to read from
	 * @return read word from memory
	 */
	word readWord(word address) const;

	/**
	 * @internal
	 * @brief Calcuate the address for a direct page access
	 * depending of the DirectPageFlag in Program Status Ward.
	 *
	 * @param dpIndex Direct page section
	 * @return Absolute address to acces direct page.
	 */
	word directPageAddress(byte dpIndex) const;
	
	/**
	 * @internal
	 * @brief Write the given byte to the address
	 * @param address Absolute address in memory
	 * @param value Byte to write
	 */
	void writeByte(word address, byte value);
	/**
	 * @internal
	 * @brief Write the given word to the address
	 * @param address Absolute address in memory
	 * @param value Word to write
	 */
	void writeWord(word address, word value);
	
	/**
 	 * @internal
	 * @brief Do the addition with carry and modify the affected flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 * @return Result as a byte
	 */
	byte addWithCarry(byte v1, byte v2);

	/**
	 * @internal
	 * @brief Do the subtraction with carry and modify the affected flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 * @return Result as a byte
	 */
	byte subtractWithCarry(byte v1, byte v2);

	/**
	 * @internal
	 * @brief Do the math for compare methods and modify the flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 */
	void compare(byte v1, byte v2);

	/**
	 * @internal
	 * @brief Do the AND operation and modify the flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 * @return Result as a byte
	 */
	byte doAnd(byte v1, byte v2);

	/**
	 * @internal
	 * @brief Do the OR operation and modifiy the flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 * @return Result as a byte
	 */
	byte doOr(byte v1, byte v2);

	/**
	 * @internal
	 * @brief Do the EOR operation and modify the flags
	 * @param v1 First operand
	 * @param v2 Second operand
	 * @return Result as a byte
	 */
	byte doEor(byte v1, byte v3);

	/**
	 * @internal
	 * @brief Add a new flag to the program status
	 * @param value Flag to add to the program status
	 */
	void setProgramStatusFlag(ProgramStatusFlags value);

	/**
	 * @internal
	 * @brief Remove the given flag from the program status
	 * @param value Flag to remove
	 */
	void removeProgramStatusFlag(ProgramStatusFlags value);

	/**
	 * @internal
	 * @brief Update A register and affected flags
	 * @param value New value of A register
	 */
	void setARegister(byte value);

	/**
	 * @internal
	 * @brief Update X register and affected flags
	 * @param value New value of X register
	 */
	void setXRegister(byte value);

	/**
	 * @internal
	 * @brief Update Y register and affected flags
	 */
	void setYRegister(byte value);

	/**
	 * @internal
	 * @brief Update zero flag based on value
	 * @param value Value to verify
	 */
	void updateZeroFlag(byte value);

	/**
	 * @internal
	 * @brief Update Negative flag based on value
	 * @param value Value to verify
	 */
	void updateNegativeFlag(byte value);

	/**
	 * @internal
	 * @brief Update Overflow flag based on value
	 *
	 * If you don't pass an int, this method will not work
	 * as expected.
	 *
	 * @param value Value to verify
	 */
	void updateOverflowFlag(int value);

	/**
	 * @internal
	 * @brief Decode address based on addressing mode
	 * @param mode Addressing mode value in AddressingMode enum
	 * @return Absolute address in memory
	 */
	word decodeAddress(AddressingMode mode);

	/**
	 * @internal
	 * @brief Test if the value has the given bit on
	 * @param bit Bit to test
	 * @param value Value to test
	 * @return true if the bit is set
	 */
	bool isBitSet(int bit, byte value) const;

	/**
	 * @internal
	 * @brief Do the ASL operation
	 * @param value Value
	 * @return Modified value
	 */
	byte doAsl(byte value);
	
	/**
	 * @internal
	 * @brief Do the LSR operation
	 * @param value Value
	 * @return Modified value
	 */
	byte doLsr(byte value);

	/**
	 * @internal
	 * @brief Do the ROL operation
	 * @param value Value
	 * @return Modified value
	 */
	byte doRol(byte value);
	
	/**
	 * @internal
	 * @brief Do the ROR operation
	 * @param value Value
	 * @return Modified value
	 */
	byte doRor(byte value);

	/**
	 * @internal
	 * @brief Push byte to the SPC700 stack
	 * @param value Value to push on the stack
	 */
	void doPush(byte value);

	/**
	 * @internal
	 * @brief Push word to the SPC700 stack
	 * @param value Value to push on the stack
	 */
	void doPushWord(word value);

	/**
	 * @internal
	 * @brief Pop a byte from the stack
	 * @return byte popped from stack
	 */
	byte doPop();

	/**
	 * @internal
	 * @brief Pop a word from the stack
	 * @return word popped from stack
	 */
	word doPopWord();

	/**
	 * @internal
	 * @brief Do a table call using table index
	 * @param tableIndex Table Index
	 */
	void doTcall(byte tableIndex);

	/**
	 * @internal
	 * @brief Set the bit on byte in Direct Page
	 * @param bit Bit to set
	 */
	void doSetBit(int bit);
	
	/**
	 * @internal
	 * @brief Clear the bit on byte in Direct Page
	 * @param bit Bit to clear
	 */
	void doClearBit(int bit);

	/**
	 * @internal
	 * @brief Get the membit data from read word
	 * @return MemBitData struct
	 */
	MemBitData getMemBitData();

private:
	class Private;
	Private *d;
};

}

#endif
