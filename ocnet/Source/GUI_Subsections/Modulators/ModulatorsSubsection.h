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

class ModulatorsSubsection : public Subsection, public Knob1::Listener {
public:
    class Listener {
    public:
        virtual ~Listener() { }
        virtual void connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) = 0;
    };

    void inline addListener(Listener* listener) {
        listeners.push_back(listener);
    }

    void inline connectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {
        DBG("connectModulation(int processorModulatorID, std::unique_ptr<Parameter2>& parameter) ");
        for (auto listener : listeners) {
            listener->connectModulation(processorModulatorID, parameter);
        }
    }

private:
    std::vector<Listener*> listeners;

};