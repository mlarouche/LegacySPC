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
#ifndef LEGACYSPC_MEMORYMAP_H
#define LEGACYSPC_MEMORYMAP_H

#include <legacyspc_export.h>
#include <types.h>

// STL includes
#include <vector>

namespace LegacySPC
{

class SpcComponentManager;

/**
 * @brief MemoryMap is the frontend for all the
 * compoments that are mapped into memory map I/O
 */
class LEGACYSPC_EXPORT MemoryMap
{
public:
	/**
	 * @brief Constructor
	 */
	MemoryMap(SpcComponentManager *manager);
	/**
	 * @brief Destructor
	 */
	~MemoryMap();

	/**
	 * @brief Read a byte from memory map
	 * @param address Address to read from.
	 * @return Byte read from a component
	 */
	byte readByte(word address) const;
	/**
	 * @brief Read a word from memory map
	 * @param address Addres to read from.
	 * @return Word read from memory
	 */
	word readWord(word address) const;

	/**
	 * @brief Write a byte to memory
	 * @param address Addres to write to
	 * @param value Byte value to write to memory
	 */
	void writeByte(word address, byte value);

	/**
	 * @brief Write a word to memory
	 * @param address Address to write to
	 * @param value Word value to write to memory
	 */
	void writeWord(word address, word value);

	/**
	 * @brief Write a vector of bytes to memory
	 * @param address Address to write to
	 * @param values Byte vector to write
	 */
	void writeBytes(word address, const std::vector<byte> &bytes);

private:
	class Private;
	Private *d;
};

}

#endif
