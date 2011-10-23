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
#include "debuggermainwindow.h"

// Qt includes
#include <QtCore/QLatin1String>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>

// Local includes
#include "debuggercentralwidget.h"
#include "emulatorcontroller.h"
#include "registerdockwidget.h"
#include "memoryviewdockwidget.h"

class DebuggerMainWindow::Private
{
public:
	Private()
	 : controller(0)
	{}

	EmulatorController *controller;
};

DebuggerMainWindow::DebuggerMainWindow(QWidget *parent)
 : QMainWindow(parent), d(new Private)
{
	d->controller = new EmulatorController(this);

	init();
}

DebuggerMainWindow::~DebuggerMainWindow()
{
	delete d;
}

void DebuggerMainWindow::init()
{
	setWindowTitle( tr("LegacySPC Debugger") );

	DebuggerCentralWidget *centralWidget = new DebuggerCentralWidget(this);
	centralWidget->setupDebuggerOutput(d->controller);
	connect(centralWidget, SIGNAL(stepClicked()), d->controller, SLOT(step()));
	setCentralWidget(centralWidget);

	initMenu();
	initDockWidgets();
}

void DebuggerMainWindow::initMenu()
{
	QMenu *fileMenu = menuBar()->addMenu( tr("&File") );
	
	QAction *fileLoadAction = new QAction( tr("Open..."), this );
	connect(fileLoadAction, SIGNAL(triggered(bool)), this, SLOT(fileLoad()));
	fileMenu->addAction(fileLoadAction);

	QAction *quitAction = new QAction( tr("Quit"), this );
	quitAction->setShortcut( Qt::CTRL + Qt::Key_Q );
	quitAction->setMenuRole( QAction::QuitRole );
	connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
	fileMenu->addAction(quitAction);

	QMenu *aboutMenu = menuBar()->addMenu( tr("&Help") );
	
	QAction *aboutQtAction = new QAction( tr("About Qt..."), this );
	aboutQtAction->setMenuRole( QAction::AboutQtRole );
	connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
	aboutMenu->addAction(aboutQtAction);
}

void DebuggerMainWindow::initDockWidgets()
{
	RegisterDockWidget *registerWidget = new RegisterDockWidget(this);
	addDockWidget( Qt::RightDockWidgetArea, registerWidget );
	
	connect(d->controller, SIGNAL(processorUpdated(LegacySPC::Processor *)), registerWidget, SLOT(processorUpdated(LegacySPC::Processor*)));
	
	MemoryViewDockWidget *memoryViewWidget = new MemoryViewDockWidget(this);
	memoryViewWidget->setMemoryReference( d->controller->memory() );
	
	addDockWidget( Qt::BottomDockWidgetArea, memoryViewWidget );
	
	connect(d->controller, SIGNAL(processorUpdated(LegacySPC::Processor*)), memoryViewWidget, SLOT(processorUpdated(LegacySPC::Processor*)));
}

void DebuggerMainWindow::fileLoad()
{
	QString spcFilename = QFileDialog::getOpenFileName(this, tr("Open SPC file"), QString(), QLatin1String("SPC file (*.spc)"));
	if( !spcFilename.isEmpty() )
	{
		d->controller->load( spcFilename );
	}
}

#include "debuggermainwindow.moc"
