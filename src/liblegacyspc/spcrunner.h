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
#ifndef LEGACYSPC_SPCRUNNER_H
#define LEGACYSPC_SPCRUNNER_H

#include <legacyspc_export.h>

// STL includes
#include <string>

namespace LegacySPC
{

class MemoryMap;
class SpcComponentManager;

 /**
  * @brief Main emulation loop manager
  *
  * This class conntain the main emulation loop
  * and manage all the component required to output
  * a sound out of a SPC buffer, either coming from
  * from memory or from a file.
  *
  * A debugger friendly version is available as DebuggerSpcRunner
  * @see DebuggerSpcRunner
  * @author Michaël Larouche <larouche@kde.org>
  */
class LEGACYSPC_EXPORT SpcRunner
{
public:
	/**
	 * @brief Constructor
	 */
	SpcRunner();
	/**
	 * @brief Destructor
	 */
	virtual ~SpcRunner();
 
 	/**
 	 * @brief Get access to memory map object
 	 * 
 	 * Use this object to read/write memory.
 	 * @return MemoryMap instance
 	 */
 	MemoryMap *memory() const;

	/**
	 * @brief Load SPC file to be runned by SpcRunner
	 * @param filename Path to SPC file
	 * @return false if the loading has failed
	 */
	bool loadSpcFile(const std::string &filename);

 	/**
 	 * @brief Execute the emulation loop
 	 *
 	 * Load an SPC buffer into memory before
 	 * using this method.
 	 *
 	 * @return false if execution failed.
 	 */
 	bool run();

protected:
	/**
	 * @internal
	 * @brief Get SpcComponentManager instance
	 *
	 * This method is used by DebuggerSpcRunner
	 * to expose more details to the outside world.
	 * @return SpcComponentManager instance
	 */
	SpcComponentManager *componentManager() const;

private:
	class Private;
	Private *d;
};

}

#endif
