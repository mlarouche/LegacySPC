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
#ifndef DISASSEMBLERMODEL_H
#define DISASSEMBLERMODEL_H

// Qt includes
#include <QtCore/QAbstractListModel>

// LegacySPC includes
#include <types.h>

namespace LegacySPC
{
	class Processor;
}

class EmulatorController;

struct DisassemblerModelItem
{
	DisassemblerModelItem()
	 : ramIndex(0), opcodeSize(0)
	{
	};

	int ramIndex;
	int opcodeSize;
	QString disassemblerOutput;
};


/**
 * @brief A Model to display disassembler output
 *
 * @author Michaël
 */
class DisassemblerModel : public QAbstractListModel
{
	Q_OBJECT
public:
	DisassemblerModel(EmulatorController *controller, QObject *parent = 0);
	~DisassemblerModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

public Q_SLOTS:
	void processorUpdated(LegacySPC::Processor *processor);

private:
	DisassemblerModelItem decodeInstruction(LegacySPC::word *currentPC);

private:
	class Private;
	Private *d;
};

#endif
