/*
    element_module.cpp - This file is part of Element
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

#if defined (ELEMENT_MODELS_H_INCLUDED) && ! JUCE_AMALGAMATED_INCLUDE
/* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"
#include "element_module.h"

#include "src/Arc.cpp"
#include "src/Asset.cpp"
#include "src/AssetTree.cpp"
#include "src/PortWriter.cpp"
#include "src/RingBuffer.cpp"

#include "src/engine/AtomBuffer.cpp"
#include "src/engine/Buffer.cpp"
#include "src/engine/BufferFactory.cpp"
#include "src/engine/ClipFactory.cpp"
#include "src/engine/ClipSource.cpp"
#include "src/engine/EngineBase.cpp"
#include "src/engine/GraphController.cpp"
#include "src/engine/GraphDocument.cpp"
#include "src/engine/GraphPlayer.cpp"
#include "src/engine/GraphPort.cpp"
#include "src/engine/GraphProcessor.cpp"
#include "src/engine/LV2EventBuffer.cpp"
#include "src/engine/MidiDataBase.cpp"
#include "src/engine/MidiSequencePlayer.cpp"
#include "src/engine/Processor.cpp"
#include "src/engine/SequenceCursor.cpp"
#include "src/engine/Sequencer.cpp"
#include "src/engine/SequencerTrack.cpp"

#if ELEMENT_PLUGINHOST_LV2
#include "src/formats/lv2/LV2Module.cpp"
#include "src/formats/lv2/LV2PluginFormat.cpp"
#include "src/formats/lv2/LV2World.cpp"
#endif

#include "src/gui/DecibelScaleComponent.cpp"

#if ELEMENT_DOCKING
#include "src/gui/Dock.cpp"
#include "src/gui/docking/DockArea.cpp"
#include "src/gui/docking/DockItem.cpp"
#include "src/gui/docking/DockLayout.cpp"
#endif

#include "src/gui/embed/NativeComponent.cpp"

#include "src/gui/GraphEditorBase.cpp"
#include "src/gui/HighQualityMeter.cpp"
#include "src/gui/Icons.cpp"
#include "src/gui/MidiEditorBody.cpp"
#include "src/gui/PluginWindow.cpp"
#include "src/gui/Style.cpp"
#include "src/gui/TimelineBase.cpp"
#include "src/gui/TimelineClip.cpp"
#include "src/gui/TreeviewBase.cpp"

#include "src/models/BlockModel.cpp"
#include "src/models/ObjectModel.cpp"
#include "src/models/Note.cpp"
#include "src/models/NoteSequence.cpp"
#include "src/models/SequenceModel.cpp"

#include "src/session/DeviceManager.cpp"
#include "src/session/PluginManager.cpp"
#include "src/session/WorldBase.cpp"

#include "src/time/TimeScale.cpp"

#include "src/util/FileHelpers.cpp"
#include "src/util/UUID.cpp"
