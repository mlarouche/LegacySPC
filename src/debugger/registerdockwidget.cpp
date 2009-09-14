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
#include "registerdockwidget.h"

// Qt includes
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>

// LegacySPC includes
#include <processor.h>
#include <processorregisters.h>

// Local includes
#include "ui_registerdockwidget.h"

class RegisterDockWidget::Private
{
public:
	Ui::RegisterDockWidget ui;
};

RegisterDockWidget::RegisterDockWidget(QWidget *parent)
 : QDockWidget(tr("Register Window"), parent), d(new Private)
{
	QWidget *theWidget = new QWidget(this);
	d->ui.setupUi(theWidget);
	setWidget(theWidget);
}

RegisterDockWidget::~RegisterDockWidget()
{
	delete d;
}

void RegisterDockWidget::processorUpdated(LegacySPC::Processor *processor)
{
	// Update each register value
	d->ui.lineA->setText( QString::number(processor->registers()->A(), 16) );
	d->ui.lineX->setText( QString::number(processor->registers()->X(), 16) );
	d->ui.lineY->setText( QString::number(processor->registers()->Y(), 16) );
	d->ui.lineYA->setText( QString::number((quint16)processor->registers()->YA(), 16) );
	d->ui.linePC->setText( QString::number((quint16)processor->registers()->programCounter(), 16) );
	d->ui.lineSP->setText( QString::number(processor->registers()->stackPointer(), 16) );

	// Update each processor status flag
	int processorStatus = static_cast<int>( processor->registers()->programStatus() );
	
	d->ui.checkCarry->setChecked( processorStatus & LegacySPC::CarryFlag );
	d->ui.checkZero->setChecked( processorStatus & LegacySPC::ZeroFlag );
	d->ui.checkOverflow->setChecked( processorStatus & LegacySPC::OverflowFlag );
	d->ui.checkNegative->setChecked( processorStatus & LegacySPC::NegativeFlag );
	d->ui.checkDirectPage->setChecked( processorStatus & LegacySPC::DirectPageFlag );
	d->ui.checkInterrupt->setChecked( processorStatus & LegacySPC::InterruptFlag );
	d->ui.checkHalfCarry->setChecked( processorStatus & LegacySPC::HalfCarryFlag );
	d->ui.checkBreak->setChecked( processorStatus & LegacySPC::BreakFlag );
}

#include "registerdockwidget.moc"
