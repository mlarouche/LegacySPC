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
#pragma once

#include <QtGui/QDockWidget>

namespace LegacySPC
{
	class MemoryMap;
	class Processor;
}

class MemoryViewDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	MemoryViewDockWidget(QWidget *parent);
	~MemoryViewDockWidget();

	void setMemoryReference(LegacySPC::MemoryMap *memory);
	
public Q_SLOTS:
	void processorUpdated(LegacySPC::Processor *processor);

private:
	class Private;
	Private *d;
};