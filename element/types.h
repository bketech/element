/*
    types.h - This file is part of Element
    Copyright (C) 2013  Michael Fisher <mfisher31@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef ELEMENT_TYPES_H
#define ELEMENT_TYPES_H

#include <inttypes.h>

#include "element/pointer.hpp"

typedef float    Sample;
typedef uint32_t SampleCount;
typedef uint32_t FrameTime;
typedef uint16_t Channel;

namespace element {

    enum PluginFormat
    {
        AUDIO_UNIT, LADSPA, LV2, VST
    };

}

#endif // ELEMENT_TYPES_H
