/*
    Slugs.h - This file is part of Element
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

#ifndef ELEMENT_SLUGS_H
#define ELEMENT_SLUGS_H

#include <element/juce.hpp>

//namespace Element {

/** Commonly used Identifiers, these might represent different things depending
    on the context of use */
namespace Slugs {

    static const Identifier asset    = "asset";
    static const Identifier assetId  = "assetId";
    static const Identifier block    = "block";
    static const Identifier bpm      = "bpm";
    static const Identifier channel  = "channel";
    static const Identifier clip     = "clip";
    static const Identifier file     = "file";
    static const Identifier fsid     = "fsid";
    static const Identifier events   = "events";
    static const Identifier graph    = "graph";
    static const Identifier group    = "group";
    static const Identifier id       = "id";
    static const Identifier keyId    = "keyId";
    static const Identifier length   = "length";
    static const Identifier name     = "name";
    static const Identifier nodeId   = "nodeId";
    static const Identifier note     = "note";
    static const Identifier path     = "path";
    static const Identifier pattern  = "pattern";
    static const Identifier ppq      = "ppq";
    static const Identifier sampler  = "sampler";
    static const Identifier start    = "start";
    static const Identifier sequence = "sequence";
    static const Identifier tempo    = "tempo";
    static const Identifier track    = "track";
    static const Identifier type     = "type";
    static const Identifier velocity = "velocity";
    static const Identifier version  = "version";

} //}

#endif // ELEMENT_SLUGS_H
