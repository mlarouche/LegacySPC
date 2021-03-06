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
#ifndef DEBUGGERMAINWINDOW_H
#define DEBUGGERMAINWINDOW_H

#include <QtGui/QMainWindow>

/**
 * @brief Main window for the debugger
 *
 * @author Michaël Larouche <larouche@kde.org>
*/
class DebuggerMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	DebuggerMainWindow(QWidget *parent = 0);
	~DebuggerMainWindow();

private Q_SLOTS:
	void fileLoad();

private:
	void init();
	void initMenu();
	void initDockWidgets();

private:
	class Private;
	Private *d;
};

#endif
