/*
    SessionTreePanel.h - This file is part of Element
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

#ifndef ELEMENT_SESSION_TREE_PANEL_H
#define ELEMENT_SESSION_TREE_PANEL_H

#include "session/Session.h"

namespace Element {

    class Session;

namespace Gui {

    class GuiApp;

    class SessionTreePanel : public TreePanelBase
    {
    public:

        explicit SessionTreePanel (GuiApp& gui);
        virtual ~SessionTreePanel();

        void mouseDown (const MouseEvent &event);

        Session& session();

    private:

        GuiApp& gui;

    };


    class SessionMediaTreePanel :  public TreePanelBase
    {

    };

}}

#endif // ELEMENT_SESSION_TREE_PANEL_H
