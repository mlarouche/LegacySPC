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
#ifndef LEGACYSPC_SPCCOMPONENTMANAGER_H
#define LEGACYSPC_SPCCOMPONENTMANAGER_H

namespace LegacySPC
{

class Ram;
class Processor;
class SpcRunner;

/**
 * @brief Manage SPC components such as RAM, DSP, Timers
 *
 * This class manage the creation and deletion of many components
 * required to full emulation. It also give access to component for
 * other classes. This is an implementation of Facade pattern.
 *
 * This is part of private API and should not be exported.
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class SpcComponentManager
{
public:
	SpcComponentManager(SpcRunner *runner);
	~SpcComponentManager();

	/**
	 * @brief Return SpcRunner used by the SpcComponentManager
	 * @return Current SpcRunner instance
	 */
	SpcRunner *runner() const;

	Ram* ram() const;

	Processor* processor() const;

private:
	class Private;
	Private *d;
};

}

#endif
