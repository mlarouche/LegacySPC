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
#include "spcfileloader.h"

// STL includes
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cstring>

// Local includes
#include "processorregisters.h"
#include "spcfile.h"
#include "types.h"
#include "legacyspc_debug.h"

using namespace std;

namespace LegacySPC
{

static const char SpcMagicHeader[] = "SNES-SPC700 Sound File Data v0.30";

class SpcFileLoader::Private
{
public:
	Private()
	 : failed(false)
	{}

	ifstream fileLoader;
	bool failed;
	SpcFile spcFile;

	bool checkMagicHeader();
	void readRegisters();
	void readRamData();
	void readID666Tag();
	void readDspRegisters();
};

// TODO: Maybe add a type to check error types
SpcFileLoader::SpcFileLoader()
 : d(new Private)
{
}

SpcFileLoader::SpcFileLoader(const std::string &filename)
 : d(new Private)
{
	open(filename);
}

SpcFileLoader::~SpcFileLoader()
{
	delete d;
}

void SpcFileLoader::open(const std::string &filename)
{
	lDebug() << "Loading file" << filename;

	d->fileLoader.open(filename.c_str(), ios::in | ios::binary);

	// Does the fileLoader failed to open
	if( !d->fileLoader )
	{
		setFailed();
		return;
	}

	// Do we really deal with a SPC file
	if( !d->checkMagicHeader() )
	{
		setFailed();
		return;
	}

	// Read SPC700 registers
	d->readRegisters();

	// Read ID666 Tag
	d->readID666Tag();

	// Read RAM data
	d->readRamData();

	// Read DSP registers
	d->readDspRegisters();

	// Close file
	d->fileLoader.close();
}

bool SpcFileLoader::operator!()
{
	return d->failed;
}

void SpcFileLoader::setFailed()
{
	d->failed = true;
}

SpcFile SpcFileLoader::spcFile() const
{
	return d->spcFile;
}

bool SpcFileLoader::Private::checkMagicHeader()
{
	char header[34];

	fileLoader.get(header, 34);

	lDebug() << "Read magic header:" << string(header);

	if( memcmp(header,SpcMagicHeader,34) == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SpcFileLoader::Private::readRegisters()
{
	fileLoader.seekg(0x25);
	
	ProcessorRegisters readRegisters;

	word PC;
	byte A, X, Y, PSW, SP;

	// Read PC
	fileLoader.read( reinterpret_cast<char*>(&PC), sizeof(unsigned short) );
	// Read A
	fileLoader >> A;
	// Read X
	fileLoader >> X;
	// Read Y
	fileLoader >> Y;
	// Read PSW
	fileLoader >> PSW;
	// Read SP
	fileLoader >> SP;

	readRegisters.setA( A );
	readRegisters.setX( X );
	readRegisters.setY( Y );
	readRegisters.setProgramStatus( PSW );
	readRegisters.setProgramCounter( PC );
	readRegisters.setStackPointer( SP );

	// Set registers to the SpcFile
	spcFile.setProcessorRegisters( readRegisters );
}

void SpcFileLoader::Private::readRamData()
{
	fileLoader.seekg(0x100);

	static const int ramSize = 0xFFFF;

	// Reserver the vector for the whole RAM
	std::vector<byte> readRamData;
	readRamData.reserve(ramSize);

	byte *rawReadData = new byte[ramSize];
	fileLoader.read((char*)rawReadData, ramSize);

	// Fill the vector with the raw data
	for(int i=0; i<ramSize; i++)
	{
		readRamData.push_back(rawReadData[i]);
	}
	// Doesn't compile
	//readRamData.insert( rawReadData, rawReadData + sizeof(rawReadData) / sizeof(byte) );

	// Set the ram data into SpcFile
	spcFile.setRamData( readRamData );

	delete[] rawReadData;
}

void SpcFileLoader::Private::readID666Tag()
{
	fileLoader.seekg(0x2E);

	char stringBuffer[33];

	ID666Tag readTag;

	// Read song title
	fileLoader.read(stringBuffer, 32);
	readTag.songTitle = string(stringBuffer, 32);

	// Read game title
	fileLoader.read(stringBuffer, 32);
	readTag.gameTitle = string(stringBuffer, 32);

	// Read name of dumper
	fileLoader.read(stringBuffer, 16);
	readTag.dumperName = string(stringBuffer, 16);

	// Read comments
	fileLoader.read(stringBuffer, 32);
	readTag.comment = string(stringBuffer, 32);

	// Read date
	fileLoader.read(stringBuffer, 11);
	readTag.dateDumped = string(stringBuffer, 11);

	// Read song length
	string tempLength;
	fileLoader.read(stringBuffer, 3);
	
	// Here we test if the first char is a digit or 0
	// else, we are in binary mode
	if( isdigit(stringBuffer[0]) || stringBuffer[0] == 0x00 )
	{
		lDebug() << "Text format";
		tempLength = string(stringBuffer, 3);
		lDebug() << "Song length:" << tempLength;
		readTag.songLength = atoi(tempLength.c_str());
	
		// Read fadeout length
		fileLoader.read(stringBuffer, 5);
		tempLength = string(stringBuffer, 5);
		lDebug() << "Fadeout length:" << tempLength;
		readTag.fadeoutLength = atoi(tempLength.c_str());
	}
	else
	{
		lDebug() << "Binary format";
		// Binary format
		readTag.isBinary = true;
		// Read date in binary
		fileLoader.seekg(0x9E, ios_base::beg);
		char date, month;
		word year;
		
		fileLoader.get(date);
		fileLoader.get(month);
		fileLoader.read(reinterpret_cast<char*>(&year), sizeof(year));

		lDebug() << "Date:" << (int)date;
		lDebug() << "Month:" << (int)month;
		lDebug() << "Year:" << year;
		stringstream dateStringStream;
		dateStringStream << year << "/" << (int)month << "/" << (int)date;
		readTag.dateDumped = dateStringStream.str();

		// Read song length in binary
		fileLoader.seekg(0xA9);
		fileLoader.read(reinterpret_cast<char*>(&readTag.songLength), sizeof(word));

		// Pass padding byte of the song length
		fileLoader.seekg(1, ios_base::cur);

		// Read fadeout length in binary
		fileLoader.read( reinterpret_cast<char *>(&readTag.fadeoutLength), 4 );
	}

	// Read artist name
	fileLoader.read(stringBuffer, 32);
	readTag.artistName = string(stringBuffer, 32);

	// Read isDefaultChannelDisabled
	char tempChannel;
	fileLoader >> tempChannel;
	readTag.isDefaultChannelDisabled = static_cast<bool>(tempChannel);

	// Read Emulator identifier
	char tempEmulatorIdent;
	fileLoader >> tempEmulatorIdent;

	readTag.emulatorIndex = atoi(&tempEmulatorIdent);

	spcFile.setID666Tag( readTag );
}

void SpcFileLoader::Private::readDspRegisters()
{
	fileLoader.seekg(0x10100);

	static const int dspRegistersSize = 128;

	// Reserver the vector for the whole RAM
	std::vector<byte> readDspRegisters;
	readDspRegisters.reserve(dspRegistersSize);

	byte *rawReadData = new byte[dspRegistersSize];
	fileLoader.read((char*)rawReadData, dspRegistersSize);

	// Fill the vector with the raw data
	for(int i=0; i<dspRegistersSize; i++)
	{
		readDspRegisters.push_back( rawReadData[i] );
	}
	// Doesn't compile
	//readRamData.insert( rawReadData, rawReadData + sizeof(rawReadData) / sizeof(byte) );

	// Set the ram data into SpcFile
	spcFile.setDspRegisters( readDspRegisters );

	delete[] rawReadData;
}

}
