/*
    ClipFactory.cpp - This file is part of Element
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

#include "element/engine/ClipFactory.h"

namespace Element {

// a dummy clip that can be used for testing purposes
class DummyClip :  public ClipSource
{
    int32 blockSize;

public:

    DummyClip()
    {
        Logger::writeToLog ("DummyClip(): created");
        blockSize = 0;
    }

    ~DummyClip() { }

    void openClip (int blockSize, double sampleRate) { Logger::writeToLog ("DummyClip::open()"); }
    void closeClip() { Logger::writeToLog ("DummyClip::close()"); }

    void renderClip (const Position& pos, AudioSourceChannelInfo& chans)
    {
        std::clog << "render: " << pos.timeInSeconds << std::endl;
    }

    void seekLocalFrame (const int64& frame) {
        std::clog << "Dummy Clip Seek: " << frame << std::endl;
    }

    void prepareToPlay (int block, double rate) { blockSize = block; }
    void releaseResources() { }

    void getNextAudioBlock (const AudioSourceChannelInfo &buffer)
    {
        std::clog << "clip pos: " << getNextReadPosition() << "-" << getNextReadPosition() + blockSize << " len: " << frameLength() << std::endl;
    }

    class Type :  public ClipType
    {
    public:

        Type() { }
        ~Type() { }

        ClipData* createClipData (Engine&, const ClipModel&)
        {
            return new ClipData();
        }

        bool canCreateFrom (const ClipModel& model) { return model.getProperty("type",String::empty) == String("dummy"); }
        bool canCreateFrom (const File &file) { return false; }
        ClipSource* createSource (Engine&, const File&) { return new DummyClip(); }
        ClipSource* createSource (Engine&, const ClipModel&) { return new DummyClip(); }

    };

};


class ClipFactory::Impl
{
public:

    Impl (ClipFactory& owner, Engine& e)
        : engine (e), factory (owner)
    { }

    ~Impl()
    {
        sources.clear();
        data.clear();
        types.clear();
    }

    void attachSourceData (ClipType* type, ClipSource* source)
    {
        ClipModel model (source->model());
        assert (model.isValid());

        assert (model.node().hasProperty (Slugs::assetId));
        const String id = model.getProperty (Slugs::assetId);
        assert (id != String::empty);

        const int64 hash = id.hashCode64();
        if (data.contains (hash))
        {
            //Logger::writeToLog("ClipFactory: using cached clip data: hash: " + String(hash));
            while (! source->setData (data [hash]));
        }
        else if (ClipData* cd = type->createClipData (engine, model))
        {
            //Logger::writeToLog("ClipFactory: creating new clip data: hash: " + String(hash));
            Shared<ClipData> sdata (cd);
            sdata->hash = hash;

            sdata->prepare (44100.f, 256);

            while (! source->setData (sdata));
            data.set (hash, sdata);
        }
        else {
            assert (false);
        }
    }

    Engine&       engine;
    ClipFactory&  factory;

    OwnedArray<ClipSource> sources;
    OwnedArray<ClipType>   types;
    HashMap<int64, Shared<ClipData> > data;

};


ClipFactory::ClipFactory (Engine& e)
{
    impl = new Impl (*this, e);
#if JUCE_DEBUG
    registerType (new DummyClip::Type());
#endif
}


ClipFactory::~ClipFactory()
{
    impl = nullptr;
}


ClipSource*
ClipFactory::createSource (const ClipModel& model)
{
    ClipType* type = nullptr;
    for (ClipType* t : impl->types)
        if (t->canCreateFrom (model))
            { type = t; break; }

    ClipSource* source = type != nullptr ? type->createSource (impl->engine, model)
                                         : nullptr;
    if (source) {
        source->setModel (model);
        impl->attachSourceData (type, source);
    }

    return source;
}

ClipType* ClipFactory::getType (const int32 t) { return impl->types.getUnchecked (t); }
void ClipFactory::registerType (ClipType* type) { impl->types.addIfNotAlreadyThere (type); }
int32 ClipFactory::numTypes() const { return impl->types.size(); }

} /* namespace BTV */
