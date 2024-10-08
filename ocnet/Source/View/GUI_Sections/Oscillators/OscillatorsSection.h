/*
  ==============================================================================

    SynthSection.h
    Created: 7 Jul 2024 9:44:44pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../LookAndFeel_V4/OcnetLookAndFeel.h"
#include "../../GUI_Subsections/Oscillators/WavetableOscillatorSubsection.h"
#include "../../GUI_Subsections/Oscillators/SamplerSubsection.h"
#include "../../../Controller/GUI_EventHandler.h"
#include "../Section.h"
#include "../../LookAndFeel_V4/OcnetLookAndFeel.h"
#include "../../../Model/ParameterHandler/ParameterHandler.h"

using namespace Ocnet;

class OscillatorsSection : public Section, juce::Button::Listener {
public:
    OscillatorsSection(GUI_EventHandler& eventHandler);

    std::unique_ptr<Subsection>* addOscillator(int processorType, int numberOfWavetableOscillators, ParameterHandler& parameterHandler);

    std::vector<std::unique_ptr<Subsection>>* getListOfSubsections() override;

    // Actualiza los valores del comboBox de todos los osciladores
    void updateOscillatorsFMCombo();

    void buttonClicked(juce::Button* clickedButton) override;
    void paint(juce::Graphics& g) override;
    void resized() override;


private:
    OcnetLookAndFeel lookAndFeel;

    std::vector<std::unique_ptr<Subsection>> subsectionsVector;

    juce::TextButton addOscillatorButton;

    GUI_EventHandler& eventHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorsSection)



        

};