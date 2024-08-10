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

    volumeParameterID = createParameterID("WavetableOscillator", getId(), "volume");
    panningParameterID = createParameterID("WavetableOscillator", getId(), "panning");
    waveTypeParameterID = createParameterID("WavetableOscillator", getId(), "waveType");

    volumeKnob = std::make_unique<Knob1>(volumeParameterID, eventHandler);
    panningKnob = std::make_unique<Knob1>(panningParameterID, eventHandler);

    volumeKnobLabel.setText("Volume", juce::dontSendNotification);
    volumeKnobLabel.attachToComponent(&*volumeKnob, false); // Se ancla al knob, pero sin moverse con él
    volumeKnobLabel.setJustificationType(juce::Justification::centred); // Centrar el texto
    volumeKnobLabel.setFont(juce::Font(12.0f, juce::Font::bold));

    addAndMakeVisible(volumeKnobLabel);

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
    waveTypeComboBox.setName(waveTypeParameterID);

    addAndMakeVisible(waveTypeComboBox);

}

void WavetableOscillatorSubsection::resized()
{
    //DBG("WavetableOscillatorSubsection::resized()");

    auto area = getLocalBounds();

    sectionResized();

    volumeKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);
    panningKnob->setBounds(defaultKnobSize, 20, defaultKnobSize, defaultKnobSize);
    volumeKnobLabel.setBounds(0, 55, 45, 18);

    waveTypeComboBox.setBounds(2 * defaultKnobSize, area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize);

}

juce::String WavetableOscillatorSubsection::getSubType()
{
    return juce::String("WavetableOscillator");
}

void WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)
{
    //DBG("WavetableOscillatorSubsection::attachParams(ParameterHandler& parameterHandler)");
    //DBG(juce::String(getId()));

    volumeParameterAttachment = std::make_unique<OcnetSliderAttachment>(*volumeKnob, *parameterHandler.getSliderParameter(volumeParameterID)->get());
    panningParameterAttachment = std::make_unique<OcnetSliderAttachment>(*panningKnob, *parameterHandler.getSliderParameter(panningParameterID)->get());
    waveTypeParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(waveTypeComboBox, *parameterHandler.getComboBoxParameter(waveTypeParameterID)->get());
}

void WavetableOscillatorSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == volumeKnob->getParameterID())
        volumeKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == panningKnob->getParameterID())
        panningKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == waveTypeComboBox.getName())
        waveTypeComboBox.setSelectedItemIndex(propertyValue.getIntValue());
}

void WavetableOscillatorSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(volumeParameterID, std::make_shared<SliderParameter>("volume"));
    parameterHandler.addSliderParameter(panningParameterID, std::make_shared<SliderParameter>("panning"));
    parameterHandler.addComboBoxParameter(waveTypeParameterID, std::make_shared<ComboBoxParameter>("waveType", juce::StringArray{ "Saw", "Square", "Sine" }, 0));
}