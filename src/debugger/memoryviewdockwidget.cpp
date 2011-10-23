#include "memoryviewdockwidget.h"

// Qt includes
#include <QtCore/QByteArray>

// LegacySPC includes
#include <memorymap.h>
#include <processor.h>

// Local includes
#include "qhexedit.h"

class MemoryViewDockWidget::Private
{
public:
	Private()
	: hexViewer(0), memory(0)
	{}
	
	QHexEdit *hexViewer;
	LegacySPC::MemoryMap *memory;
};

MemoryViewDockWidget::MemoryViewDockWidget(QWidget *parent)
: QDockWidget(tr("Memory View"), parent), d(new Private)
{
	d->hexViewer = new QHexEdit(this);
	d->hexViewer->setReadOnly(true);
	
	QFont hexFont = d->hexViewer->font();
	hexFont.setPointSize(12);
	d->hexViewer->setFont(hexFont);
	
	setWidget(d->hexViewer);
}

MemoryViewDockWidget::~MemoryViewDockWidget()
{
	delete d;
}

void MemoryViewDockWidget::setMemoryReference(LegacySPC::MemoryMap *memory)
{
	d->memory = memory;
}

void MemoryViewDockWidget::processorUpdated(LegacySPC::Processor *processor)
{
	QByteArray memoryData;
	static const int MemoryFragmentSize = 16 * 8;
	
	LegacySPC::word currentReadAddress = processor->registers()->programCounter();
	
	for(LegacySPC::uint32 i=0; i<MemoryFragmentSize; i++)
	{
		memoryData.append( d->memory->readByte(currentReadAddress) );
		currentReadAddress++;
	}
	
	d->hexViewer->setData( memoryData );
	d->hexViewer->setAddressOffset( processor->registers()->programCounter() );
}

#include "memoryviewdockwidget.moc"