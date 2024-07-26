/*
  ==============================================================================

    EnvelopeSubsection.h
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../../Knobs/Knob1.h"
#include "../Subsection.h"

#include <JuceHeader.h>
#include "../../Knobs/DragZone.h"

class EnvelopeSubsection : public Subsection, public Knob1::Listener {
public:
    EnvelopeSubsection(int id);

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void connectModulation(int processorModulatorID, Parameter2& parameter) = 0;
    };

    void resized() override;
    void paint(juce::Graphics& g) override;

    void inline addListener(Listener* listener) {
        listeners.push_back(listener);
    }

    void inline connectModulation(int processorModulatorID, Parameter2& parameter) {
        DBG("connectModulation(int processorModulatorID, Parameter2& parameter) ");
        for (auto listener : listeners) {
            listener->connectModulation(processorModulatorID, parameter);
        }
    }

    void attachParams(ParameterHandler& parameterHandler);

    bool isMainEnvelope() { return this->mainEnvelope; }
    void setAsMainEnvelope() { this->mainEnvelope = true; }
    void unsetAsMainEnvelope() { this->mainEnvelope = false; }
private:
    std::vector<Listener*> listeners;

    std::unique_ptr<Knob1> attackKnob;
    std::unique_ptr<Knob1> decayKnob;
    std::unique_ptr<Knob1> sustainKnob;
    std::unique_ptr<Knob1> releaseKnob;

    bool mainEnvelope;

    DragZone dragZone;

    //using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    //using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
};