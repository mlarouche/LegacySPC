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
#ifndef LEGACYSPC_DEBUG_H
#define LEGACYSPC_DEBUG_H

// STL includes
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <types.h>

using namespace std;

namespace LegacySPC
{

class LogStream
{
public:
	enum MessageLevel
	{
		Debug,
		Warning,
		Fatal,
		Log
	};

	inline LogStream(MessageLevel level)
	 : d(new Private)
	{
		d->level = level;
	}

	inline ~LogStream()
	{
		if(!--d->ref)
		{
			// Print message to the correct ouput
			ostream *outputStream;
			if( d->level == Debug )
			{
				outputStream = &cerr;
			}
			else if( d->level == Log )
			{
				outputStream = &clog;
			}
			else
			{
				outputStream = &cout;
			}

			// Print the message
			*outputStream << d->loggingStream.str() << endl;

			// Delete instance
			delete d;
		}
	}

	inline LogStream(const LogStream &copy)
	 : d(copy.d)
	{
		++d->ref;
	}

	inline LogStream &space()
	{
		d->space = true;
		d->loggingStream << " ";
		return *this;
	}

	inline LogStream &nospace()
	{
		d->space = false;
		return *this;
	}

	inline LogStream &maybeSpace()
	{
		if( d->space )
		{
			d->loggingStream << " ";
		}
		return *this;
	}

	inline LogStream &operator=(const LogStream &other)
	{
		if( this != &other )
		{
			LogStream copy(other);
			swap(d, copy.d);
		}

		return *this;
	}

	inline LogStream &operator<<(const char *value)
	{
		d->loggingStream << string(value);
		return maybeSpace();
	}

	inline LogStream &operator<<(const std::string &value)
	{
		d->loggingStream << value;
		return maybeSpace();
	}

	inline LogStream &operator<<(int value)
	{
		d->loggingStream << value;
		return maybeSpace();
	}

	inline LogStream &operator<<(size_t value)
	{
		d->loggingStream << value;
		return maybeSpace();
	}

	inline LogStream &operator<<(unsigned char value)
	{
		d->loggingStream << "0x" << hex << (int)value;
		return maybeSpace();
	}

	inline LogStream &operator<<(word value)
	{
		d->loggingStream << "0x" << hex << (unsigned short)value;
		return maybeSpace();
	}

private:
	class Private
	{
	public:
		Private()
		 : ref(1), space(true), level(Log)
		{}

		int ref;
		bool space;
		MessageLevel level;
		stringstream loggingStream;
	};

	Private *d;
};

static inline LogStream logStream(LogStream::MessageLevel level, const char *file = 0, int line = -1, const char *funcinfo = 0)
{
	LogStream stream(level);

	if( funcinfo )
	{
		stream.nospace() << funcinfo << ":";
		return stream.space();
	}

	return stream;
}

static inline LogStream lDebug(const char *file = 0,  int line = -1, const char *funcinfo = 0)
{
	return logStream(LogStream::Debug, file, line, funcinfo);
}

static inline LogStream lWarning()
{
	return logStream(LogStream::Warning) << "WARNING:";
}

static inline LogStream lLog()
{
	return logStream(LogStream::Log);
}

static inline LogStream lFatal()
{
	return logStream(LogStream::Fatal);
}

class LogDebug
{
public:
	explicit inline LogDebug(const char *name, int line, const char *info)
	 : m_name(name), m_line(line), m_funcinfo(info)
	{}

	inline LogStream operator()()
	{
		return lDebug(m_name, m_line, m_funcinfo);
	}

private:
	const char *m_name;
	int m_line;
	const char *m_funcinfo;
};

class ReleaseLogStream
{
public:
	explicit ReleaseLogStream()
	{}

	inline ReleaseLogStream operator()()
	{
		return *this;
	}

	inline ReleaseLogStream &operator<<(const char *)
	{
		return *this;
	}

	inline ReleaseLogStream &operator<<(int)
	{
		return *this;
	}

	inline ReleaseLogStream &operator<<(const std::string &)
	{
		return *this;
	}

	inline ReleaseLogStream &operator<<(size_t)
	{
		return *this;
	}

	inline ReleaseLogStream &operator<<(word)
	{
		return *this;
	}
};

#ifdef __GNUC__
#define LEGACYSPC_FUNC_INFO __PRETTY_FUNCTION__
#else
#define LEGACYSPC_FUNC_INFO
#endif

#ifdef LEGACYSPC_DEBUG
#define lDebug LogDebug(__FILE__, __LINE__,LEGACYSPC_FUNC_INFO)
#else
#define lDebug ReleaseLogStream()
#endif

}

#endif // LEGACYSPC_DEBUG_H
