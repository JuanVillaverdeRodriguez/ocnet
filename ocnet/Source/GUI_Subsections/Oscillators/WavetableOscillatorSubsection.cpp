/*
  ==============================================================================

    OscillatorSubsection.cpp
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorSubsection.h"

WavetableOscillatorSubsection::WavetableOscillatorSubsection(int id, GUI_EventHandler& eventHandler) : OscillatorsSubsection(eventHandler)
{
    setId(id);

    baseParameterID = juce::String("Oscillators_") + juce::String(getId()) + juce::String("_");

    volumeKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Oscillators"), juce::String(getId()), juce::String("volume") }, eventHandler);
    panningKnob = std::make_unique<Knob1>(ParameterInfo{ juce::String("Oscillators"), juce::String(getId()), juce::String("panning") }, eventHandler);

    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);

    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

    subsectionName.setText(juce::String("Oscillator ") + juce::String(getId()));

    // Configuración del ComboBox
    waveTypeComboBox.addItem("Saw", 1);
    waveTypeComboBox.addItem("Sine", 2);
    waveTypeComboBox.addItem("Square", 3);
    waveTypeComboBox.setSelectedId(1); // Selecciona "Saw" por defecto
    addAndMakeVisible(waveTypeComboBox);

}

void WavetableOscillatorSubsection::resized()
{
    DBG("WavetableOscillatorSubsection::resized()");

    auto area = getLocalBounds();

    sectionResized();

    volumeKnob->setBounds(0, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);
    panningKnob->setBounds(defaultKnobSize, area.getHeight() - defaultKnobSize, defaultKnobSize, defaultKnobSize);

    waveTypeComboBox.setBounds(2 * defaultKnobSize, area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize);

}

void WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)
{
    DBG("WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)");
    DBG(juce::String(getId()));

    parameterHandler.attachParameter(volumeKnob->getParameter());
    parameterHandler.attachParameter(panningKnob->getParameter());

    parameterHandler.addComboBoxParameter(baseParameterID + juce::String("waveType"), std::make_unique<ComboBoxParameter>("waveType", juce::StringArray{"Saw", "Sine", "Square"}, 0));
    waveTypeParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(waveTypeComboBox, *parameterHandler.getComboBoxParameter(baseParameterID + juce::String("waveType"))->get());
}