/*
  ==============================================================================

    gui_interface.h
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "OcnetGUI.h"
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "GUI_Sections/EffectsSection.h"

class OcnetGUI_interface : EffectsSection::Listener, ModulatorsSection::Listener, OscillatorsSection::Listener {
public:
    OcnetGUI_interface(OcnetAudioProcessor *processor);

    void addOscillator(int option) override;

    void addEffectsChain() override;

    void addModulator(int option) override;

    OcnetGUI* getGui() { return gui_.get(); }

private:
    std::unique_ptr<OcnetGUI> gui_;
    OcnetAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OcnetGUI_interface)

};