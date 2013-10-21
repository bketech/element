/*
    BlockModel.h - This file is part of Element
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

#ifndef ELEMENT_BLOCK_MODEL_H
#define ELEMENT_BLOCK_MODEL_H

#include "element/Slugs.h"
#include "element/models/NodeModel.h"

namespace Element
{
    class BlockModel : public NodeModel
    {
    public:
        BlockModel() : NodeModel (Slugs::block) { }
        BlockModel (const Identifier& valueType) : NodeModel (valueType) { }
        BlockModel (const ValueTree& data) : NodeModel (data) { }
    };
}

#endif // ELEMENT_BLOCK_MODEL_H
