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
#ifndef LEGACYSPC_SPCFILELOADER_H
#define LEGACYSPC_SPCFILELOADER_H

// STL includes
#include <string>

#include <legacyspc_export.h>

namespace LegacySPC
{

class SpcFile;

/**
 * @brief Load a SPC file from disk
 * @code
LegacySPC::SpcFileLoader spcLoader("spcfile.spc");
if( !spcLoader )
{
	std::cerr << "Error while reading spcfile.spc" << std::endl;
}

SpcFile file = spcLoader.spcFile();
 * @endcode
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class LEGACYSPC_EXPORT SpcFileLoader
{
public:
	/**
	 * @brief Create a new instance of SpcFileLoader.
	 *
	 * To open the SPC file, use open().
	 */
	SpcFileLoader();
	/**
	 * @brief Create SpcFileLoader and open the SPC File
	 *
	 * With this constructor, you don't need to call open().
	 * @param filename SPC file to open
	 */
	SpcFileLoader(const std::string &filename);

	/**
	 * @brief Destructor
	 */
	~SpcFileLoader();

	/**
	 * @brief Open and read the SPC file content
	 * @param file SPC File to open
	 */
	void open(const std::string &filename);

	/**
	 * @brief Check if the file loading failed
	 * @return true if loading failed
	 */
	bool operator!();

	/**
	 * @brief Get the SpcFile with the data loaded from the file
	 * @return SpcFile instance
	 */
	SpcFile spcFile() const;

private:
	/**
	 * @internal
	 * @brief Call to set failed status
	 */
	void setFailed();

private:
	class Private;
	Private *d;
};

}

#endif
