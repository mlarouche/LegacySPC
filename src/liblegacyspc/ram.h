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
#ifndef LEGACYSPC_RAM_H
#define LEGACYSPC_RAM_H

#include <legacyspc_export.h>
#include <types.h>

// STD includes
#include <vector>

namespace LegacySPC
{

/**
 * @brief Manage SPC700 RAM
 *
 * Does not manage a traditional memory,
 * only the RAM(Random Access Memory) part of it. To get full
 * access to the memory, use MemoryMap instead.
 *
 * @author Michaël Larouche <larouche@kde.org>
 * @see MemoryMap
 */
class LEGACYSPC_EXPORT Ram
{
public:
	/**
	 * @brief Constructor
	 */
	Ram();
	/**
	 * @brief Destructor
	 */
	~Ram();

	void loadRam(const std::vector<byte> &data);

	/**
	 * @brief Read a byte from RAM
	 * @param address Address to read from.
	 * @return Byte read from RAM
	 */
	byte readByte(word address);

	/**
	 * @brief Write a byte toRAM
	 * @param address Addres to write to
	 * @param value Byte value to write to memory
	 */
	void writeByte(word address, byte value);

private:
	class Private;
	Private *d;
};

}

#endif
