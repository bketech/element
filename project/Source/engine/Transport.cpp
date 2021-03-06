/*
    Transport.cpp - This file is part of Element
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

#include "Transport.h"

namespace Element {


    Transport::Transport()
        : playState (false),
          recordState (false)
    {
        Array<int> a;
        playPos.reset (new Monitor (a, 0));
    }


    Transport::~Transport() { }


    void
    Transport::preProcess (int nframes)
    {

        if (recording != recordState.get()) {
            recording = recordState.get();
        }

        if (playing != playState.get()) {
            playing = playState.get();
        }

        if (playing) {
            playPos->set (positionInSeconds());
        }

    }

    Shared<Monitor>
    Transport::monitor()
    {
        return playPos;
    }

    void
    Transport::postProcess (int nframes)
    {
        if (tempo() != nextTempo.get())
        {
            setTempo (nextTempo.get());
            std::clog << "Realtime Transport Update: BPM: " << tempo() << std::endl;
            nextTempo.set (tempo());
        }
    }
}
