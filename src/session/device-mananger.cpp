/*
    device-manager.cpp - This file is part of Element
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

#include "element/juce.hpp"
#include "element/session/device-manager.hpp"
#include "element/engine.hpp"

using namespace element;

namespace element {

    class DeviceManager::Private
    {
    public:

        Private() { }
        ~Private() { }

        Shared<Engine> activeEngine;
    };

    class IdleCallback :  public AudioIODeviceCallback
    {
    public:

        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                    float** outputChannelData, int numOutputChannels,
                                    int numSamples)
        {  }

        void audioDeviceAboutToStart (AudioIODevice* device)
        { }

        void audioDeviceStopped()
        {
            std::clog << "Device Stopping\n";
        }

        void audioDeviceError (const String& e)
        {
            String msg = "Device Error: " ; msg << e;
            Logger::writeToLog (msg);
        }

        void shutdown() { }
    };

    DeviceManager::DeviceManager()
    {
        priv.reset (new Private());
    }

    DeviceManager::~DeviceManager()
    {
        closeAudioDevice();
        attach (Shared<Engine>());
    }

    void
    DeviceManager::attach (Shared<Engine> engine)
    {
        if (priv->activeEngine == engine)
            return;

        Shared<Engine> old = priv->activeEngine;

        if (old != nullptr)
        {
            removeAudioCallback (&old->callback());
            // SIGNAL drop references (or something) ???
        }

        if (engine)
            addAudioCallback (&engine->callback());
        else
            closeAudioDevice();

        priv->activeEngine = engine;
    }

    static void
    addNotNull (OwnedArray <AudioIODeviceType>& list, AudioIODeviceType* const device)
    {
        if (device != nullptr)
            list.add (device);
    }

    void
    DeviceManager::createAudioDeviceTypes (OwnedArray <AudioIODeviceType>& list)
    {
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_WASAPI());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_DirectSound());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_ASIO());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_CoreAudio());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_iOSAudio());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_ALSA());
        // addNotNull (list, AudioIODeviceType::createAudioIODeviceType_JACK());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_OpenSLES());
        addNotNull (list, AudioIODeviceType::createAudioIODeviceType_Android());
    }



    void
    DeviceManager::getAudioDrivers (StringArray& drivers)
    {
        for (auto* d : getAvailableDeviceTypes())
            drivers.add (d->getTypeName());
    }

    void
    DeviceManager::selectAudioDriver (const String& name)
    {
        setCurrentAudioDeviceType (name, true);
    }
}
