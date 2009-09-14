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
#ifndef EMULATORCONTROLLER_H
#define EMULATORCONTROLLER_H

#include <QtCore/QObject>

namespace LegacySPC
{
	class MemoryMap;
	class Processor;
}

/**
 * @brief Controll all operation related to emulation
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class EmulatorController : public QObject
{
	Q_OBJECT
public:
	EmulatorController(QObject *parent = 0);
	~EmulatorController();

	LegacySPC::Processor *processor() const;

	LegacySPC::MemoryMap *memory() const;

public Q_SLOTS:
	void load(const QString &filename);
	void reset();
	void run();
	void step();
	void stop();

Q_SIGNALS:
	void error();
	void processorUpdated(LegacySPC::Processor *processor);

private:
	void emitProcessorUpdated();

private:
	class Private;
	Private *d;
};

#endif
