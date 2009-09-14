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
#include "debuggercentralwidget.h"

// Local includes
#include "disassemblermodel.h"
#include "emulatorcontroller.h"
#include "ui_debuggercentralwidget.h"

class DebuggerCentralWidget::Private
{
public:
	Ui::DebuggerCentralWidget widget;
	DisassemblerModel *model;
};

DebuggerCentralWidget::DebuggerCentralWidget(QWidget *parent)
 : QWidget(parent), d(new Private)
{
	d->widget.setupUi(this);

	connect(d->widget.buttonStep, SIGNAL(clicked()), this, SIGNAL(stepClicked()));
}

DebuggerCentralWidget::~DebuggerCentralWidget()
{
}

void DebuggerCentralWidget::setupDebuggerOutput(EmulatorController *controller)
{
	d->model = new DisassemblerModel(controller, this);
	connect(controller, SIGNAL(processorUpdated(LegacySPC::Processor *)), d->model, SLOT(processorUpdated(LegacySPC::Processor *)));
	d->widget.listDebuggerOutput->setModel(d->model);
}

#include "debuggercentralwidget.moc"
