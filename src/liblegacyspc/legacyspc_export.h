/*
 * LegacySPC - A portable object-oriented SPC emulator.
 * Copyright 2007 by Michaël Larouche <larouche@kde.org>
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
#ifndef LEGACYSPC_EXPORT_H
#define LEGACYSPC_EXPORT_H

#ifdef __LEGACYSPC_HAVE_GCC_VISIBILITY
#define LEGACYSPC_EXPORT __attribute__ ((visibility("default")))
#define LEGACYSPC_IMPORT LEGACYSPC_EXPORT
#elif defined(_WIN32) || defined(_WIN64)
#define LEGACYSPC_EXPORT __declspec(dllexport)
#define LEGACYSPC_IMPORT __declspec(dllimport)
#else
#define LEGACYSPC_EXPORT
#define LEGACYSPC_IMPORT
#endif

#endif
