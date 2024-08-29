/*
  ==============================================================================

    EffectsSection.h
    Created: 7 Jul 2024 9:44:54pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"
#include "../ParameterHandler/ParameterHandler.h"
#include "../GUI_Subsections/Effects/DistortionSubsection.h"
#include "../GUI_Subsections/Effects/FilterSubsection.h"
#include "../GUI_Subsections/Effects/ReverbSubsection.h"
#include "../GUI_EventHandler.h"
#include "Section.h"

using namespace Ocnet;

class EffectsSection : public Section, juce::Button::Listener {
public:
    EffectsSection(GUI_EventHandler& eventHandler);

    std::unique_ptr<Subsection>* addEffect(int processorType, int id, ParameterHandler& parameterHandler);

    void buttonClicked(juce::Button* clickedButton) override;
    void paint(juce::Graphics& g) override;
    void resized() override;
    std::vector<std::unique_ptr<Subsection>>* getListOfSubsections() override;


private:
    GUI_EventHandler& eventHandler;

    juce::TextButton addEffectButton;

    OcnetLookAndFeel lookAndFeel;

    std::vector<std::unique_ptr<Subsection>> subsectionsVector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsSection)


        

};