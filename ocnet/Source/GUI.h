/*
  ==============================================================================

    GUI.h
    Created: 1 Jul 2024 6:36:37pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Knobs/Knob1.h"
#include "Controller.h"
#include "Parameter.h"

class GUI : public juce::Component, juce::Button::Listener {
public:
    GUI(Controller controller);

    void buttonClicked(juce::Button *button) override;
    void initializeGUI();
    void resized() override;


private:
    Controller controller;

    std::vector<juce::Component*> getComps();
    Knob1 midiVolume;

    Knob1 attackKnob;
    Knob1 decayKnob;
    Knob1 sustainKnob;
    Knob1 releaseKnob;
    juce::ComboBox oscSelector;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::vector<Parameter> getParams();

    std::unique_ptr<SliderAttachment> attackAttachment; //A
    std::unique_ptr<SliderAttachment> decayAttachment; //D
    std::unique_ptr<SliderAttachment> sustainAttachment; //S
    std::unique_ptr<SliderAttachment> releaseAttachment; //R


    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using OptionAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<ButtonAttachment> buttonAttachment; //R
    std::unique_ptr<OptionAttachment> optionAttachment; //R


    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    //Unique ponteirs para que al cerrar la UI (el plugin se destruye) se libere la memoria


    juce::TextButton button{ "Mostrar Menu" };

};