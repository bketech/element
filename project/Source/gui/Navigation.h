/*
    Navigation.h - This file is part of Element
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

#ifndef ELEMENT_NAVIGATION_H
#define ELEMENT_NAVIGATION_H

#include <element/Juce.h>

namespace Element {
namespace Gui {

class ContentComponent;
class SessionTreePanel;

class Navigator : public Component
{
public:

    Navigator();
    ~Navigator();

    ContentComponent* content();

private:

    ScopedPointer<ResizableBorderComponent> resizer;
    ScopedPointer<SessionTreePanel> sessionTree;

};

}}

#endif // ELEMENT_NAVIGATION_H
