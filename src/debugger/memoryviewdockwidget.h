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