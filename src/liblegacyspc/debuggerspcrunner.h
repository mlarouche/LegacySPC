/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright 2007-2008 by Michaël Larouche <larouche@kde.org>
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
#ifndef LEGACYSPCDEBUGGERSPCRUNNER_H
#define LEGACYSPCDEBUGGERSPCRUNNER_H

#include <legacyspc_export.h>
#include <spcrunner.h>

namespace LegacySPC
{

class Processor;

/**
 * @brief Special version of SpcRunner for debugger usage
 *
 * @author Michaël Larouche <larouche@kde.org>
*/
class LEGACYSPC_EXPORT DebuggerSpcRunner : public SpcRunner
{
public:
	DebuggerSpcRunner();
	virtual ~DebuggerSpcRunner();

	Processor *processor() const;
};

}

#endif
