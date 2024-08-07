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

    volumeKnob = std::make_unique<Knob1>(createParameterID("WavetableOscillator", getId(), "volume"), eventHandler);
    panningKnob = std::make_unique<Knob1>(createParameterID("WavetableOscillator", getId(), "panning"), eventHandler);

    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);

    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

    subsectionName.setText(juce::String("Oscillator ") + juce::String(getId()));

    // Configuración del ComboBox
    waveTypeComboBox.addItem("Saw", 1);
    waveTypeComboBox.addItem("Square", 2);
    waveTypeComboBox.addItem("Sine", 3);
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

    parameterHandler.addSliderParameter(createParameterID("WavetableOscillator", getId(), "volume"), std::make_shared<SliderParameter>("volume"));
    volumeParameterAttachment = std::make_unique<OcnetSliderAttachment>(*volumeKnob, *parameterHandler.getSliderParameter(createParameterID("WavetableOscillator", getId(), "volume"))->get());

    parameterHandler.addSliderParameter(createParameterID("WavetableOscillator", getId(), "panning"), std::make_shared<SliderParameter>("panning"));
    panningParameterAttachment = std::make_unique<OcnetSliderAttachment>(*panningKnob, *parameterHandler.getSliderParameter(createParameterID("WavetableOscillator", getId(), "panning"))->get());

    parameterHandler.addComboBoxParameter(createParameterID("WavetableOscillator", getId(), "waveType"), std::make_shared<ComboBoxParameter>("waveType", juce::StringArray{"Saw", "Square", "Sine"}, 0));
    waveTypeParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(waveTypeComboBox, *parameterHandler.getComboBoxParameter(createParameterID("WavetableOscillator", getId(), "waveType"))->get());
}