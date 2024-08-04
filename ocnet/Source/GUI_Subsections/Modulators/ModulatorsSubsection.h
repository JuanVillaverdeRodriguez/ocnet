/*
  ==============================================================================

    ModulatorsSubsection.h
    Created: 3 Aug 2024 3:33:04pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../Subsection.h"
#include "../../Knobs/Knob1.h"

class ModulatorsSubsection : public Subsection, public Knob1::Listener, juce::Button::Listener {
public:
    ModulatorsSubsection();

    class Listener {
    public:
        virtual ~Listener() { }
        virtual void connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) = 0;
        virtual void deleteModulator(int id) = 0;
    };

    void inline addListener(Listener* listener) {
        listeners.push_back(listener);
    }

    void inline connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {
        for (auto listener : listeners) {
            listener->connectModulation(processorModulatorID, parameter);
        }
    }

    void buttonClicked(juce::Button* clickedButton) override;

private:
    std::vector<Listener*> listeners;

};