/*
    MidiSequencePlayer.cpp - This file is part of Element
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

#include "element/engine/MidiSequencePlayer.h"

#define NOTE_CHANNEL       1
#define NOTE_VELOCITY      0.8f
#define NOTE_PREFRAMES     0.001

namespace element {



MidiSequencePlayer::MidiSequencePlayer()
    : midiSequence (new MidiMessageSequence()),
      doAllNotesOff (false),
      allNotesOff (MidiMessage::allNotesOff (1))
{
    enabled = true;
    shuttle = nullptr;
    stopRecording = false;
}

MidiSequencePlayer::~MidiSequencePlayer ()
{
    midiSequence = nullptr;
    shuttle      = nullptr;
}

void MidiSequencePlayer::prepareToPlay (double sampleRate, int /* blockSize */)
{
	noteOffs.clear();
    startTimer (1000 / 25);
    shuttle->setSampleRate (sampleRate);
}

void MidiSequencePlayer::releaseResources()
{
    stopTimer();
}

void MidiSequencePlayer::renderSequence (int numSamples, MidiBuffer& midiMessages)
{

    const int blockSize = numSamples;
    MidiBuffer* midiBuffer =& midiMessages;

    if (shuttle->isPlaying())
    {
        const int frameCounter  = shuttle->positionInFrames();
        const int framesPerBeat = shuttle->framesPerBeat();

        const double framePerBeatDelta = 1.0f / (double) framesPerBeat;

        const int nextBlockFrameNumber = frameCounter + blockSize;

        const int seqIndex = loopRepeatIndex();
        const double beatCount = loopBeatPosition();

       // std::clog << "loop: " << seqIndex << " beat: " << beatCount << " transport: " << shuttle->positionInBeats() << std::endl;
       // return;

        const double frameLenBeatCount = (nextBlockFrameNumber - frameCounter) / (double) framesPerBeat;
		double frameEndBeatCount = beatCount + frameLenBeatCount;
		if (frameEndBeatCount > getLengthInBeats())
			frameEndBeatCount -= getLengthInBeats();

        if (shuttle->isRecording())
        {
            if (recordingSequence.getNumEvents() > 0)
            {
                midiSequence->addSequence (recordingSequence, 0.0, 0.0, 10000000000.0);
                midiSequence->updateMatchedPairs();
                recordingSequence.clear();
            }

            int samplePos = 0;
            MidiMessage msg (0xf4, 0.0);

            MidiBuffer::Iterator eventIterator (*midiBuffer);
            while (eventIterator.getNextEvent (msg, samplePos))
            {
                if (msg.isNoteOnOrOff())
                {
                    msg.setTimeStamp (beatCount + (samplePos * framePerBeatDelta));
                    recordingSequence.addEvent (msg);
                    std::clog << "capture: " << msg.getNoteNumber() << " at " << msg.getTimeStamp() << std::endl;
                }
            }
        }

		if (frameEndBeatCount > beatCount)
		{
			renderEventsInRange(*midiSequence, midiBuffer, beatCount, frameEndBeatCount, frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex, blockSize);
			renderEventsInRange(noteOffs, midiBuffer, beatCount, frameEndBeatCount, frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex, blockSize);
			cleanUpNoteOffs(beatCount, frameEndBeatCount);
		}
		else
		{
            renderEventsInRange (*midiSequence, midiBuffer, beatCount, getLengthInBeats(), frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex, blockSize);
            renderEventsInRange (*midiSequence, midiBuffer, 0.00, frameEndBeatCount, frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex + 1, blockSize);
            renderEventsInRange (noteOffs, midiBuffer, beatCount, getLengthInBeats(), frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex, blockSize);
            renderEventsInRange (noteOffs, midiBuffer, 0.00, frameEndBeatCount, frameCounter, framesPerBeat, nextBlockFrameNumber, seqIndex+1, blockSize);
            cleanUpNoteOffs (beatCount, getLengthInBeats());
            cleanUpNoteOffs (0.00, frameEndBeatCount);
		}

    }

    if (doAllNotesOff || shuttle->willSendAllNotesOff())
    {
        midiBuffer->addEvent (allNotesOff, blockSize - 1);
        doAllNotesOff = false;
    }
}

void MidiSequencePlayer::renderEventsInRange (const MidiMessageSequence& source, MidiBuffer* midiBuffer,
                                              double beatCount, double frameEndBeatCount,
                                              const int frameCounter, const int framesPerBeat,
                                              const int nextBlockFrameNumber, const int seqIndex,
                                              const int blockSize)
{


    for (int i = source.getNextIndexAtTime(beatCount); i < source.getNumEvents(); ++i)
	{
        int timeStampInSeq = roundFloatToInt (source.getEventTime(i) * framesPerBeat);
        int timeStamp = timeStampInSeq + (seqIndex * getLengthInBeats() * framesPerBeat);

        MidiMessage* msg = &source.getEventPointer(i)->message;
        if (timeStamp >= nextBlockFrameNumber || ! msg)
		{
//            std::clog << "timestamp past next block.\n";
			break;
		}

        if (timeStamp >= shuttle->lengthInFrames())
		{
//            std::clog << "timestamp out of shuttle range\n";
			break;
		}
		
        // now play the event - unless we are disabled (muted), or it is a note off
        // (always play note offs so we don't get dangling note-ons when user disables
        // during a note sounding)
        if (msg->isNoteOff() || enabled)
		{
            midiBuffer->addEvent (*msg, timeStamp - frameCounter);

			// look for matching note off - if it is past the loop end then we need to wrap it and ensure it gets played at the right time
            if (msg->isNoteOn())
			{
                std::clog << "note on " << msg->getNoteNumber() << " at " << timeStamp << " cycle: " << timeStamp - frameCounter
                          << " beatCount: " << beatCount << std::endl;

                int up = source.getIndexOfMatchingKeyUp (i);
                if (source.getEventPointer (up)->message.isNoteOff() &&
                    source.getEventPointer (up)->message.getTimeStamp() > getLengthInBeats())
                    noteOffs.addEvent (source.getEventPointer (up)->message, - getLengthInBeats());
			}
		}	
	}
}

void MidiSequencePlayer::cleanUpNoteOffs (double fromTime, double toTime)
{
	for (int i=noteOffs.getNumEvents()-1; i>=0; i--)
	{
        if (noteOffs.getEventPointer (i)->message.getTimeStamp() >= fromTime &&
            noteOffs.getEventPointer (i)->message.getTimeStamp() < toTime)
            noteOffs.deleteEvent (i, false);
	}
}

void MidiSequencePlayer::timerCallback ()
{
    if (shuttle->isRecording () && stopRecording)
    {
        DBG ("Finalizing recording !");

        MidiMessageSequence* oldSequence = midiSequence.get();
        ScopedPointer<MidiMessageSequence> sequence = new MidiMessageSequence();

        sequence->addSequence (*oldSequence, 0.0f, 0.0f, 1000000000.0f);
        sequence->updateMatchedPairs ();
        sequence->addSequence (recordingSequence, 0.0f, 0.0f, 1000000000.0f);
        sequence->updateMatchedPairs ();

        {
            // const ScopedLock sl (parentHost->getCallbackLock());
            midiSequence.swapWith (sequence);
        }

        setRecordingFlag (false);
        // shuttle->resetRecording();

        sequence = nullptr;
        recordingSequence.clear ();

        // sendChangeMessage (this);

        DBG ("Recording OK !");
    }
}


//==============================================================================
bool
MidiSequencePlayer::timeSignatureChanged (const int barsCount, const int timeDenominator)
{
#if 0
	// only change the transport if we have a bigger duration
    if (barsCount > shuttle->getNumBars())
	{
        shuttle->setTimeSignature (shuttle->getTempo (), barsCount, timeDenominator);
        shuttle->setRightLocator (barsCount * timeDenominator);
	}
#endif
    return true;
}

bool MidiSequencePlayer::playingPositionChanged (const float absolutePosition)
{
    //shuttle->setPositionAbsolute (absolutePosition);
    return true;
}

bool MidiSequencePlayer::addNote (const int noteNumber,
                                  const float beatNumber,
                                  const float noteLength)
{
    MidiMessage msgOn = MidiMessage::noteOn (NOTE_CHANNEL, noteNumber, NOTE_VELOCITY);
    msgOn.setTimeStamp (beatNumber);
    MidiMessage msgOff = MidiMessage::noteOff (NOTE_CHANNEL, noteNumber);
    msgOff.setTimeStamp ((beatNumber + noteLength) - NOTE_PREFRAMES);

    DBG ("Adding:" + String (noteNumber) + " " + String (beatNumber));

    {
        // const ScopedLock sl (parentHost->getCallbackLock ());

        midiSequence->addEvent (msgOn);
        midiSequence->addEvent (msgOff);
        midiSequence->updateMatchedPairs ();
    }

    DBG ("Added:" + String (noteNumber) + " " + String (beatNumber));

    return true;
}

bool MidiSequencePlayer::removeNote (const int noteNumber,
                                      const float beatNumber,
                                      const float noteLength)
{
    DBG ("Removing:" + String (noteNumber) + " " + String (beatNumber));

    double noteOnBeats = beatNumber - NOTE_PREFRAMES;

    int eventIndex = midiSequence->getNextIndexAtTime (noteOnBeats);
    while (eventIndex < midiSequence->getNumEvents ())
    {
        MidiMessage* eventOn = &midiSequence->getEventPointer (eventIndex)->message;

        if (eventOn->isNoteOn () && eventOn->getNoteNumber () == noteNumber)
        {
            // TODO - check note off distance == noteLength
            {
            // const ScopedLock sl (parentHost->getCallbackLock());

            midiSequence->deleteEvent (eventIndex, true);
            midiSequence->updateMatchedPairs ();
            }

            DBG ("Removed:" + String (eventIndex) + " > " + String (noteNumber) + " @ " + String (beatNumber));

            if (shuttle->isPlaying ())
                doAllNotesOff = true;

            return true;
        }

        eventIndex++;
    }

    DBG (">>> Remove failed:" + String (noteNumber) + " @ " + String (beatNumber));

    return false;
}

bool MidiSequencePlayer::moveNote (const int oldNote,
                                    const float oldBeat,
                                    const int noteNumber,
                                    const float beatNumber,
                                    const float noteLength)
{
    DBG ("Moving:" + String (oldNote) + " " + String (oldBeat));

    double noteOnBeats = oldBeat - NOTE_PREFRAMES;

    int eventIndex = midiSequence->getNextIndexAtTime (noteOnBeats);
    while (eventIndex < midiSequence->getNumEvents())
    {
        MidiMessage* eventOn = &midiSequence->getEventPointer (eventIndex)->message;

        if (eventOn->isNoteOn () && eventOn->getNoteNumber () == oldNote)
        {
            // TODO - check old note off distance == oldNoteLength

            MidiMessage msgOn = MidiMessage::noteOn (NOTE_CHANNEL, noteNumber, NOTE_VELOCITY);
            msgOn.setTimeStamp (beatNumber);
            MidiMessage msgOff = MidiMessage::noteOff (NOTE_CHANNEL, noteNumber);
            msgOff.setTimeStamp ((beatNumber + noteLength) - NOTE_PREFRAMES);

            {
            // const ScopedLock sl (parentHost->getCallbackLock());

            // remove old events
            midiSequence->deleteEvent (eventIndex, true);
            midiSequence->updateMatchedPairs ();
            // add new events
            midiSequence->addEvent (msgOn);
            midiSequence->addEvent (msgOff);
            midiSequence->updateMatchedPairs ();
            }

            DBG ("Moved:" + String (eventIndex) + " > "
                          + String (oldNote) + " @ " + String (oldBeat) + " to "
                          + String (noteNumber) + " @ " + String (beatNumber));

            if (shuttle->isPlaying ())
                doAllNotesOff = true;

            return true;
        }

        eventIndex++;
    }

    DBG (">>> Move failed:" + String (oldNote) + " @ " + String (oldBeat));

    return false;
}

bool MidiSequencePlayer::resizeNote (const int noteNumber, const float beatNumber, const float noteLength)
{
    DBG ("Resizing:" + String (noteNumber) + " " + String (beatNumber));

    double noteOnBeats = beatNumber - NOTE_PREFRAMES;

    int eventIndex = midiSequence->getNextIndexAtTime (noteOnBeats);
    while (eventIndex < midiSequence->getNumEvents ())
    {
        MidiMessage* eventOn = &midiSequence->getEventPointer (eventIndex)->message;

        if (eventOn->isNoteOn () && eventOn->getNoteNumber () == noteNumber)
        {
            // TODO - check old note off distance == oldNoteLength

            MidiMessage msgOn = MidiMessage::noteOn (NOTE_CHANNEL, noteNumber, NOTE_VELOCITY);
            msgOn.setTimeStamp (beatNumber);
            MidiMessage msgOff = MidiMessage::noteOff (NOTE_CHANNEL, noteNumber);
            msgOff.setTimeStamp ((beatNumber + noteLength) - NOTE_PREFRAMES);

            {
                // const ScopedLock sl (parentHost->getCallbackLock());

                // delete old events
                midiSequence->deleteEvent (eventIndex, true);
                midiSequence->updateMatchedPairs ();
                // add new events
                midiSequence->addEvent (msgOn);
                midiSequence->addEvent (msgOff);
                midiSequence->updateMatchedPairs ();
            }

            DBG ("Resized:" + String (eventIndex) + " > "
                             + String (noteNumber) + " @ " + String (beatNumber) + " to " + String (noteLength));

            if (shuttle->isPlaying ())
                doAllNotesOff = true;

            return true;
        }

        eventIndex++;
    }

    DBG (">>> Resize failed:" + String (noteNumber) + " @ " + String (beatNumber));

    return false;
}

bool MidiSequencePlayer::clearAllNotes()
{
    {
    // const ScopedLock sl (parentHost->getCallbackLock());

    midiSequence->clear();

    if (shuttle->isPlaying())
        doAllNotesOff = true;
    }

    return true;
}

//==============================================================================
void MidiSequencePlayer::getNoteOnIndexed (const int index, int& note, float& beat, float& length)
{
    int numNoteOn = 0;
    for (int i = 0; i < midiSequence->getNumEvents (); i++)
    {
        MidiMessageSequence::MidiEventHolder* eventOn  = midiSequence->getEventPointer (i);
        MidiMessageSequence::MidiEventHolder* eventOff = midiSequence->getEventPointer (i)->noteOffObject;

        MidiMessage* msgOn =& eventOn->message;
        if (eventOn->message.isNoteOn () && eventOff)
        {
            MidiMessage* msgOff = & eventOff->message;
            if (index == numNoteOn)
            {
                note = msgOn->getNoteNumber ();
                beat = msgOn->getTimeStamp ();
                length = ((msgOff->getTimeStamp () + NOTE_PREFRAMES) - msgOn->getTimeStamp ());
                break;
            }
            numNoteOn++;
        }
    }
}

int MidiSequencePlayer::countNoteOns() const
{
    int numNoteOn = 0;

    for (int i = 0; i < midiSequence->getNumEvents (); i++)
        if (midiSequence->getEventPointer (i)->message.isNoteOn ()) numNoteOn++;

    return numNoteOn;
}

//==============================================================================
static void dumpMidiMessageSequence (MidiMessageSequence* midiSequence)
{
#if JUCE_DEBUG
    for (int i = 0; i < midiSequence->getNumEvents (); i++)
    {
        MidiMessage* midiMessage = &midiSequence->getEventPointer (i)->message;

        DBG (String (midiMessage->getNoteNumber()) + " "
             + (midiMessage->isNoteOn() ? "ON " : "OFF ")
             + String (midiMessage->getTimeStamp()));

        // DBG ("Playing event @ " + String (frameCounter) + " : " + String (timeStamp));
    }
#endif
}


void MidiSequencePlayer::setSequence (const MidiMessageSequence* mseq)
{
    if (mseq)
    {
        ScopedPointer<MidiMessageSequence> next = new MidiMessageSequence(*mseq);
        midiSequence.swapWith (next);
        midiSequence->updateMatchedPairs();
        noteOffs.clear();
	}
}

int MidiSequencePlayer::loopRepeatIndex()
{ 
    return static_cast<int> (floor (shuttle->positionInBeats())) / getLengthInBeats();
}

double MidiSequencePlayer::loopBeatPosition()
{
    return shuttle->positionInBeats() - (loopRepeatIndex() * getLengthInBeats());
}

}
