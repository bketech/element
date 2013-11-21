/*
    Globals.cpp - This file is part of Element
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

#include <element/formats/lv2/LV2World.h>
#include <element/formats/PluginManager.h>
#include <element/SymbolMap.h>

#include "models/Session.h"
#include "Globals.h"
#include "MediaManager.h"

namespace Element {

    class Globals::Internal
    {
    public:

        Internal (Globals& g)
            : owner(g), symbols()
        {
            lv2     = new LV2World();
            plugins = new PluginManager (*lv2);
            devices = new DeviceManager();
            media   = new MediaManager();

            {
                PropertiesFile::Options opts;
                opts.applicationName     = "Element";
                opts.filenameSuffix      = "conf";
                opts.folderName          = opts.applicationName;
                opts.osxLibrarySubFolder = "Application Support";
                opts.storageFormat       = PropertiesFile::storeAsXML;
                owner.settings().setStorageParameters (opts);
            }

            plugins->restoreUserPlugins (owner.settings());
        }

        void freeAll()
        {
            session  = nullptr;
            devices  = nullptr;
            media    = nullptr;
            plugins  = nullptr;
            lv2      = nullptr;
        }

        ~Internal() { }

        Globals& owner;

        SymbolMap symbols;

        ScopedPointer<DeviceManager> devices;
        ScopedPointer<LV2World>      lv2;
        ScopedPointer<MediaManager>  media;
        ScopedPointer<PluginManager> plugins;
        ScopedPointer<Session>       session;

    private:

    };

    Globals::Globals()
    {
        impl = new Internal (*this);
        uris = new Element::URIs (boost::bind (&SymbolMap::map, &impl->symbols, ::_1));
    }

    Globals::~Globals()
    {
        impl->freeAll();
        uris = nullptr;
    }

    DeviceManager& Globals::devices() { return *impl->devices.get(); }
    MediaManager& Globals::media()
    {
        assert (impl->media != nullptr);
        return *impl->media;
    }

    PluginManager& Globals::plugins()
    {
        assert (impl->plugins != nullptr);
        return *impl->plugins;
    }

    SymbolMap&
    Globals::symbols()
    {
        return impl->symbols;
    }

    Session&
    Globals::session()
    {
        assert (impl->session != nullptr);
        return *impl->session;
    }

    void
    Globals::setEngine (Shared<Element::Engine> engine)
    {
        Element::World::setEngine (engine);

        if (impl->session == nullptr) {
            impl->session = new Session (*this);
        }

        devices().attach (engine);
    }
}
