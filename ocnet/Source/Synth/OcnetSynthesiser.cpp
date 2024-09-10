/*
  ==============================================================================

    OcnetSynthesiser.cpp
    Created: 18 Jun 2024 8:21:42pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetSynthesiser.h"

OcnetSynthesiser::OcnetSynthesiser() : shouldStealNotes(true)
{
    hasMainEnvelope = false;
}

OcnetSynthesiser::~OcnetSynthesiser()
{
}

void OcnetSynthesiser::connectModulation(int processorModulatorID, std::shared_ptr<SliderParameter> parameter, const juce::String& parameterID) {
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->connectModulation(processorModulatorID, parameter, parameterID);
        }
    }
}

void OcnetSynthesiser::deleteProcessor(int processorID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->deleteProcessor(processorID);
        }
    }
}

void OcnetSynthesiser::moveProcessor(int id, int positions)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->moveProcessor(id, positions);
        }
    }
}

void OcnetSynthesiser::addEffect(int processorID, int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {

            voice->addEffect(processorID, id);
        }
    }
}

void OcnetSynthesiser::addOscillator(int processorID, int id)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->addOscillator(processorID, id);
        }
    }
}

void OcnetSynthesiser::addModulator(int processorID, int id, int subID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            if (id == 0) {
                hasMainEnvelope = true;
            }
            voice->addModulator(processorID, id, subID);
        }
    }
}

void OcnetSynthesiser::setBypassed(int id, bool bypassed)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->setBypassed(id, bypassed);
        }
    }
}

void OcnetSynthesiser::removeModulation(const juce::String& modulationID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->removeModulation(modulationID);
        }
    }
}

void OcnetSynthesiser::setFmFrom(int carrierID, int modulatorID)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->setFmFrom(carrierID, modulatorID);
        }
    }
}

void OcnetSynthesiser::samplerSampleChanged(int id, const juce::String& filename)
{
    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            voice->samplerSampleChanged(id, filename);
        }
    }
}

juce::Array<float> OcnetSynthesiser::getSamplerSampleSamples(int samplerID)
{
    if (auto voice = dynamic_cast<SynthVoice*>(getVoice(0))) {
        return voice->getSamplerSampleSamples(samplerID);
    }
}

void OcnetSynthesiser::setThisNumberOfVoices(int numberOfVoices)
{
    // Si el numero de voces que deben de estar activas es menor al de las voces actualmente activas, desactiva las que puedas
    if (numberOfVoices != getNumberOfEnabledVoices()) {
        if (numberOfVoices < getNumberOfEnabledVoices()) {
            for (int i = 0; ((i < getNumVoices()) && (numberOfVoices != getNumberOfEnabledVoices())); i++) {
                if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
                    if (voice->getCurrentlyPlayingNote() < 0) {
                        voice->disable();
                    }
                }
            }
        }
        else { // Si es mayor al de las voces activas, activa algunas que esten desactivadas
            for (int i = 0; ((i < getNumVoices()) && (numberOfVoices != getNumberOfEnabledVoices())); i++) {
                if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
                    if (!voice->isEnabled()) {
                        voice->enable();
                    }
                }
            }
        }
    }
}

int OcnetSynthesiser::getNumberOfEnabledVoices() {
    int counter = 0;

    for (int i = 0; i < getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(getVoice(i))) {
            if (voice->isEnabled()) {
                counter++;
            }
        }
    }

    return counter;
}

juce::SynthesiserVoice* OcnetSynthesiser::findVoiceToSteal(juce::SynthesiserSound* soundToPlay, int, int midiNoteNumber) const
{
    // This voice-stealing algorithm applies the following heuristics:
    // - Re-use the oldest notes first
    // - Protect the lowest & topmost notes, even if sustained, but not if they've been released.

    // apparently you are trying to render audio without having any voices...
    jassert(!voices.isEmpty());

    // These are the voices we want to protect (ie: only steal if unavoidable)
    juce::SynthesiserVoice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
    juce::SynthesiserVoice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase

    // All major OSes use double-locking so this will be lock- and wait-free as long as the lock is not
    // contended. This is always the case if you do not call findVoiceToSteal on multiple threads at
    // the same time.
    const juce::ScopedLock sl(stealLock);

    // this is a list of voices we can steal, sorted by how long they've been running
    usableVoicesToStealArray.clear();

    for (auto* voice : voices)
    {
        if (auto synthVoice = dynamic_cast<SynthVoice*>(voice)) {
            if (voice->canPlaySound(soundToPlay) && synthVoice->isEnabled())
            {
                jassert(voice->isVoiceActive()); // We wouldn't be here otherwise

                usableVoicesToStealArray.add(voice);

                // NB: Using a functor rather than a lambda here due to scare-stories about
                // compilers generating code containing heap allocations..
                struct Sorter
                {
                    bool operator() (const juce::SynthesiserVoice* a, const juce::SynthesiserVoice* b) const noexcept { return a->wasStartedBefore(*b); }
                };

                std::sort(usableVoicesToStealArray.begin(), usableVoicesToStealArray.end(), Sorter());

                if (!voice->isPlayingButReleased()) // Don't protect released notes
                {
                    auto note = voice->getCurrentlyPlayingNote();

                    if (low == nullptr || note < low->getCurrentlyPlayingNote())
                        low = voice;

                    if (top == nullptr || note > top->getCurrentlyPlayingNote())
                        top = voice;
                }
            }
        }
    }

    // Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
    if (top == low)
        top = nullptr;

    // The oldest note that's playing with the target pitch is ideal..
    for (auto* voice : usableVoicesToStealArray)
        if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
            return voice;

    // Oldest voice that has been released (no finger on it and not held by sustain pedal)
    for (auto* voice : usableVoicesToStealArray)
        if (voice != low && voice != top && voice->isPlayingButReleased())
            return voice;

    // Oldest voice that doesn't have a finger on it:
    for (auto* voice : usableVoicesToStealArray)
        if (voice != low && voice != top && !voice->isKeyDown())
            return voice;

    // Oldest voice that isn't protected
    for (auto* voice : usableVoicesToStealArray)
        if (voice != low && voice != top)
            return voice;

    // We've only got "protected" voices now: lowest note takes priority
    jassert(low != nullptr);

    // Duophonic synth: give priority to the bass note:
    if (top != nullptr)
        return top;

    return low;
}

juce::SynthesiserVoice* OcnetSynthesiser::findFreeVoice(juce::SynthesiserSound* soundToPlay, int midiChannel, int midiNoteNumber, bool stealIfNoneAvailable) const
{
    const juce::ScopedLock sl(lock);

    for (auto* voice : voices) {
        if (auto synthVoice = dynamic_cast<SynthVoice*>(voice)) {
            if ((!synthVoice->isVoiceActive()) && synthVoice->canPlaySound(soundToPlay) && synthVoice->isEnabled())
                return synthVoice;
        }
    }

    if (stealIfNoneAvailable)
        return findVoiceToSteal(soundToPlay, midiChannel, midiNoteNumber);

    return nullptr;
}

int OcnetSynthesiser::getNewestMidiNote()
{
    return 1;
}

void OcnetSynthesiser::noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff)
{
    const juce::ScopedLock sl(lock);

    for (auto* voice : voices)
    {
        if (voice->getCurrentlyPlayingNote() == midiNoteNumber
            && voice->isPlayingChannel(midiChannel))
        {
            if (auto sound = voice->getCurrentlyPlayingSound())
            {
                if (sound->appliesToNote(midiNoteNumber)
                    && sound->appliesToChannel(midiChannel))
                {
                    //jassert(!voice->keyIsDown || voice->isSustainPedalDown() == sustainPedalsDown[midiChannel]);

                    voice->setKeyDown(false);

                    if (!(voice->isSustainPedalDown() || voice->isSostenutoPedalDown())) {
                        notesThatDidntEnd.remove(notesThatDidntEnd.indexOf(voice->getCurrentlyPlayingNote()));
                        stopVoice(voice, velocity, allowTailOff);
                    }

                    // Si hay notas que quedan pendientes, cambia la voz a reproducir la ultima de esas voces
                    if (!notesThatDidntEnd.isEmpty()) {
                        int newerNote = notesThatDidntEnd.getLast();
                        notesThatDidntEnd.removeLast();
                        noteOn(midiChannel, newerNote, 1.0f);
                    }

                    if (midiNoteNumber == processorInfo.legatoInfo.previousMidiNotePressed)
                        processorInfo.legatoInfo.previousNoteIsBeingPlayed = false;
                }
            }
        }
        else {
            notesThatDidntEnd.remove(notesThatDidntEnd.indexOf(midiNoteNumber));
        }
    }
}

void OcnetSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
    const juce::ScopedLock sl(lock);

    for (auto* sound : sounds)
    {
        if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
        {
            notesThatDidntEnd.add(midiNoteNumber);

            // If hitting a note that's still ringing, stop it first (it could be
            // still playing because of the sustain or sostenuto pedal).
            for (auto* voice : voices)
                if (voice->getCurrentlyPlayingNote() == midiNoteNumber && voice->isPlayingChannel(midiChannel)) {
                    notesThatDidntEnd.remove(notesThatDidntEnd.indexOf(midiNoteNumber));
                    stopVoice(voice, 1.0f, true);
                }

            startVoice(findFreeVoice(sound, midiChannel, midiNoteNumber, shouldStealNotes),
                sound, midiChannel, midiNoteNumber, velocity);
        }
    }
}
