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
#ifndef LEGACYSPC_SPCFILE_H
#define LEGACYSPC_SPCFILE_H

// STL includes
#include <string>
#include <vector>

// Local includes
#include <types.h>
#include <legacyspc_export.h>

namespace LegacySPC
{

class ProcessorRegisters;

/**
 * @brief Basic ID666 Tag structure
 *
 * This structure is independant of the way the data
 * is stored, either its text or binary.
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
struct ID666Tag
{
	ID666Tag()
	 : songLength(0), fadeoutLength(0), isDefaultChannelDisabled(false),
	   emulatorIndex(0), isBinary(false)
	{}

	/**
	 * @brief Song Title
	 */
	std::string songTitle;
	/**
	 * @brief Game Title
	 */
	std::string gameTitle;
	/**
	 * @brief Name of the dumper
	 */
	std::string dumperName;
	/**
	 * @brief Comments
	 */
	std::string comment;
	/**
	 * @brief Date the SPC was dumped
	 * TODO: Use a DateTime type class instead of std::string
	 */
	std::string dateDumped;
	/**
	 * @brief Number of seconds to play the song before fadeout
	 */
	int songLength;
	/**
	 * @brief Length of the fadeout in milliseconds
	 */
	int fadeoutLength;
	/**
	 * @brief Original artist name
	 */
	std::string artistName;
	/**
	 * @brief Are the default channel disabled ?
	 */
	bool isDefaultChannelDisabled;
	/**
	 * @brief Emulator used to dump the SPC
	 * TODO: Maybe use a enum
	 */
	int emulatorIndex;

	/**
	 * @brief Define whenever the tag is in binary format
	 * By default it's false
	 */
	bool isBinary;
};

// TODO: Get/set Extra RAM
// TODO: Extended ID666 Tag based on chunk
/**
 * @brief Contain all data of a SPC file
 *
 * This class does not load an SPC file, it is data class for
 * all the data contained into a SPC file.
 *
 * To load a SPC file, use SpcFileLoader.
 *
 * SpcFile is implicit shared, but not thread-safe because the
 * ref-counting is not thread aware.
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class LEGACYSPC_EXPORT SpcFile
{
public:
	/**
	 * @brief Create a new instance of SpcFile
	 */
	SpcFile();
	/**
	 * @brief Release SpcFile from memory when the ref counting expires.
	 */
	~SpcFile();

	/**
	 * @brief Create a implicit shared copy of SpcFile
	 * @param copy SpcFile to share
	 */
	SpcFile(const SpcFile &copy);

	/**
	 * @brief Share the instace with the other SpcFile
	 * @param other Other SpcFile
	 * @return current SpcFile instance reference
	 */
	const SpcFile &operator=(const SpcFile &other);

	/**
	 * @brief Get the ID666 Tag
	 * @return ID666 Tag
	 */
	ID666Tag &id666Tag() const;

	/**
	 * @brief Set the ID666 Tag
	 * @param tag ID666Tag instance
	 */
	void setID666Tag(const ID666Tag &tag);

	/**
	 * @brief Get the Processor registers
	 * @return ProcessorRegisters instance
	 */
	ProcessorRegisters &processorRegisters() const;

	/**
	 * @brief Set the registers values for the CPU
	 * @param registers ProcessorRegisters instance
	 */
	void setProcessorRegisters(const ProcessorRegisters &registers);

	/**
	 * @brief Get the ram data
	 * @return a vector of bytes containing the RAM data
	 */
	std::vector<byte> &ramData() const;

	/**
	 * @brief Set the RAM data read from the SPC file
	 * @param ramData vector of bytes containing the RAM data
	 */
	void setRamData(const std::vector<byte> &ramData);

	/**
	 * @brief Get the DSP registers
	 * @return byte vector containing DSP registers
	 */
	std::vector<byte> &dspRegisters() const;
	
	/**
	 * @brief Set the DSP registers
	 * @param dspRegisters byte vector containing DSP registers
	 */
	void setDspRegisters(const std::vector<byte> &dspRegisters);

private:
	/**
	 * @brief Detach the instance from the shared one when modified
	 */
	void detach();

private:
	class Private;
	Private *d;
};

}

#endif
