/*
    Common includes and definitions of LZMA utils

    Copyright (C) 2005 Lasse Collin <lasse.collin@tukaani.org>

    This software is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
*/

#include "LzmaDecode.h"
#include "version.h"

#ifndef UInt64
#define UInt64 unsigned long long int
#endif

#define LZMA_HEADER_SIZE (LZMA_PROPERTIES_SIZE + 8)
