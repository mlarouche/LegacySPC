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
#ifndef LEGACYSPCTYPES_H
#define LEGACYSPCTYPES_H

#include <ostream>
#include <legacyspc_export.h>

namespace LegacySPC
{
	typedef unsigned char u8, uint8;
	typedef signed char s8, sint8;
	typedef unsigned short u16, uint16;
	typedef signed short s16, sint16;
	typedef unsigned int u32, uint32;
	typedef signed int s32, sint32;

	typedef u8 byte;
	typedef s8 offset;
	typedef u32 dword;
	typedef s32 sdword;

/**
 * @brief A wrapper around endian aware word(16 bit unsigned integer)
 *
 * The class works the same way as a normal unsigned short except you
 * can get the high byte and the low byte of the integer independent
 * of the endianess of the processor.
 *
 * A word typedef is available.
 *
 * @author Michaël Larouche <larouche@kde.org>
 */
class LEGACYSPC_EXPORT ManagedWord
{
public:
	/**
	 * @brief Create a new instance of ManagedWord
	 */
	ManagedWord()
	{
		m_impl.Word = 0;
	}

	/**
	 * @brief Create a new instance of ManagedWord with initial value
	 * @param value unsigned short value
	 */
	ManagedWord(uint16 value)
	{
		m_impl.Word = value;
	}

	/**
	 * @brief Copy constructor
	 * @param copy Instance to copy
	 */
	ManagedWord(const ManagedWord &copy)
	{
		this->m_impl = copy.m_impl;
	}

	/**
	 * @brief Copy-assignment operator
	 * @param other Instance to assign.
	 */
	ManagedWord &operator=(const ManagedWord &other)
	{
		if( this != &other )
		{
			this->m_impl = other.m_impl;
		}

		return *this;
	}

	/**
	 * @brief Get the lower byte of the word
	 * @return Lower byte
	 */
	byte lowByte() const
	{
		return m_impl.Byte.low;
	}

	/**
	 * @brief Get the high byte of the word
	 * @return High byte
	 */
	byte highByte() const
	{
		return m_impl.Byte.high;
	}

	/**
	 * @brief Set the low byte of the word
	 * @param value Low byte value
	 */
	void setLowByte(byte value)
	{
		m_impl.Byte.low = value;
	}

	/**
	 * @brief Set the high byte of the word
	 * @param value High byte value
	 */
	void setHighByte(byte value)
	{
		m_impl.Byte.high = value;
	}

	/**
	 * @brief Convert word to short
	 * @return short
	 */
	inline operator short()
	{
		return static_cast<short>(m_impl.Word);
	}

	/**
	 * @brief Convert word to unsigned short
	 * @return unsigned short
	 */
	inline operator unsigned short()
	{
		return m_impl.Word;
	}

	// NOTE: There are no operator to unsigned int
	// because its cause too many ambiguity for
	// the compiler

	/**
	 * @brief Convert word to int
	 * @return int
	 */
	inline operator int()
	{
		return static_cast<int>(m_impl.Word);
	}

	/**
	 * @brief Convert word to byte (with loss of information)
	 * @return byte
	 */
	inline operator byte()
	{
		return static_cast<byte>(m_impl.Word);
	}

	/**
	 * @brief Assign value to word
	 * @param value unsigned short value to assign
	 * @return this instance
	 */
	inline ManagedWord &operator=(uint16 value)
	{
		m_impl.Word = value;
		return *this;
	}

	/**
	 * @brief Prefix operator++, increment now the value of the word
	 * @return this instance
	 */
	inline ManagedWord &operator++()
	{
		++m_impl.Word;
		return *this;
	}

	/**
	 * @brief Postfix operator++, increment the value after the instruction
	 * @return this instance
	 */
	inline ManagedWord &operator++(int)
	{
		// Postfix version
		m_impl.Word++;
		return *this;
	}

	/**
	 * @brief Prefix operator--, decrement now the value of the word
	 * @return this instance
	 */
	inline ManagedWord &operator--()
	{
		--m_impl.Word;
		return *this;
	}

	/**
	 * @brief Postfix operator--, decrement the value after the instruction
	 */
	inline ManagedWord &operator--(int)
	{
		m_impl.Word--;
		return *this;
	}

	friend inline std::ostream &operator<<(std::ostream &, const ManagedWord &);

private:
	// An union represent a single value in memory.
	// The Byte and Word contain the same value
	// except with Byte you can get the high and low byte
	union
	{
		struct
		{
#ifdef LEGACYSPC_LSB
			byte low, high;
#else
			byte high, low;
#endif
		} Byte;
		uint16 Word;
	} m_impl;
};

typedef ManagedWord word;

/**
 * @brief Output value of a word to a stream
 * @param stream The stream
 * @param value Word value to output
 * @return the modified stream
 */
inline std::ostream &operator<<(std::ostream &stream, const ManagedWord &value)
{
	stream << value.m_impl.Word;
	return stream;
}

};

 #endif
