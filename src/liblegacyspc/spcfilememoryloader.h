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
#ifndef LEGACYSPC_SPCFILEMEMORYLOADER_H
#define LEGACYSPC_SPCFILEMEMORYLOADER_H

#include <string>

namespace LegacySPC
{

class SpcComponentManager;

/**
 * @brief Load a SpcFile into memory
 *
 * It put all the SpcFile data into the memory.
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class SpcFileMemoryLoader
{
public:
	/**
	 * @brief Create a new instance of SpcFileMemoryLoader
	 */
	SpcFileMemoryLoader(SpcComponentManager *manager);
	/**
	 * @brief Destructor
	 */
	~SpcFileMemoryLoader();

	/**
	 * @brief Load SPC file in memory
	 * @param filename Path to SPC file
	 */
	bool loadSpcFile(const std::string &filename);

private:
	/**
	 * @internal
	 * @brief Get SpcComponentManager instance
	 * @return Current SpcComponentManager instance
	 */
	SpcComponentManager* component() const;

private:
	class Private;
	Private *d;
};

}

#endif
