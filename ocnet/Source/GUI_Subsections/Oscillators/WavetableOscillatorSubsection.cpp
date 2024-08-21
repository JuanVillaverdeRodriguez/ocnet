/*
  ==============================================================================

    OscillatorSubsection.cpp
    Created: 8 Jul 2024 4:50:23pm
    Author:  TecNo

  ==============================================================================
*/

#include "WavetableOscillatorSubsection.h"

WavetableOscillatorSubsection::WavetableOscillatorSubsection(int id, GUI_EventHandler& eventHandler) : OscillatorsSubsection(eventHandler, id, "WavetableOscillator")
{
    volumeParameterID = createParameterID("WavetableOscillator", getId(), "volume");
    panningParameterID = createParameterID("WavetableOscillator", getId(), "panning");
    waveTypeParameterID = createParameterID("WavetableOscillator", getId(), "waveType");
    fmFromParameterID = createParameterID("fmFrom");
    fmAmountParameterID = createParameterID("fmAmount");
    numVoicesParameterID = createParameterID("unisonNumVoices");
    detuneAmountParameterID = createParameterID("unisonDetuneAmount");
    transposeParameterID = createParameterID("transpose");

    volumeKnob = std::make_unique<Knob1>(volumeParameterID, eventHandler, "Level");
    panningKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Pan");
    numVoicesKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Voices");
    detuneAmountKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Detune");
    fmAmountKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "FM");
    transposeKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Transpose");


    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);
    this->addAndMakeVisible(*numVoicesKnob);
    this->addAndMakeVisible(*detuneAmountKnob);
    this->addAndMakeVisible(*fmAmountKnob);
    this->addAndMakeVisible(*transposeKnob);



    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    numVoicesKnob->setRange(0, 8, 1); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    detuneAmountKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    fmAmountKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    transposeKnob->setRange(-24, 24, 1); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;


    volumeKnob->setValue(0.75f);
    panningKnob->setValue(0.5f);
    numVoicesKnob->setValue(1);
    detuneAmountKnob->setValue(0.20f);
    fmAmountKnob->setValue(1.0f);
    transposeKnob->setValue(0);


    subsectionName.setText(juce::String("Oscillator ") + juce::String(getId()));

    // Configuración del ComboBox
    waveTypeComboBox.addItem("Saw", 1);
    waveTypeComboBox.addItem("Square", 2);
    waveTypeComboBox.addItem("Sine", 3);
    waveTypeComboBox.setSelectedId(1); // Selecciona "Saw" por defecto
    waveTypeComboBox.setName(waveTypeParameterID);

    // Configuración del ComboBox
    fmFromComboBox.addItem("None", 1);
    fmFromComboBox.setSelectedId(1);
    fmFromComboBox.setName(fmFromParameterID);

    addAndMakeVisible(waveTypeComboBox);
    addAndMakeVisible(fmFromComboBox);

    fmFromComboBox.addListener(this);


}

void WavetableOscillatorSubsection::resized()
{
    //DBG("WavetableOscillatorSubsection::resized()");

    auto area = getLocalBounds();

    sectionResized();

    volumeKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);
    panningKnob->setBounds(defaultKnobSize, 20, defaultKnobSize, defaultKnobSize);
    numVoicesKnob->setBounds(defaultKnobSize*2, 20, defaultKnobSize, defaultKnobSize);
    fmAmountKnob->setBounds(defaultKnobSize * 3, 20, defaultKnobSize, defaultKnobSize);
    detuneAmountKnob->setBounds(defaultKnobSize * 4, 20, defaultKnobSize, defaultKnobSize);
    transposeKnob->setBounds(defaultKnobSize * 5, 20, defaultKnobSize, defaultKnobSize);


    volumeKnob->showLabel(*this, *volumeKnob);
    panningKnob->showLabel(*this, *panningKnob);
    numVoicesKnob->showLabel(*this, *numVoicesKnob);
    fmAmountKnob->showLabel(*this, *fmAmountKnob);
    detuneAmountKnob->showLabel(*this, *detuneAmountKnob);
    transposeKnob->showLabel(*this, *transposeKnob);


    waveTypeComboBox.setBounds(defaultKnobSize * 6, area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize - 10);
    fmFromComboBox.setBounds(defaultKnobSize * 7 + (defaultKnobSize * 2), area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize - 10);
}

void WavetableOscillatorSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    volumeParameterAttachment = std::make_unique<OcnetSliderAttachment>(*volumeKnob, *parameterHandler.getSliderParameter(volumeParameterID)->get());
    panningParameterAttachment = std::make_unique<OcnetSliderAttachment>(*panningKnob, *parameterHandler.getSliderParameter(panningParameterID)->get());
    detuneAmountParameterAttachment = std::make_unique<OcnetSliderAttachment>(*detuneAmountKnob, *parameterHandler.getSliderParameter(detuneAmountParameterID)->get());
    numVoicesParameterAttachment = std::make_unique<OcnetSliderAttachment>(*numVoicesKnob, *parameterHandler.getSliderParameter(numVoicesParameterID)->get());
    fmAmountParameterAttachment = std::make_unique<OcnetSliderAttachment>(*fmAmountKnob, *parameterHandler.getSliderParameter(fmAmountParameterID)->get());
    transposeParameterAttachment = std::make_unique<OcnetSliderAttachment>(*transposeKnob, *parameterHandler.getSliderParameter(transposeParameterID)->get());

    fmFromParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(fmFromComboBox, *parameterHandler.getComboBoxParameter(fmFromParameterID)->get());
    waveTypeParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(waveTypeComboBox, *parameterHandler.getComboBoxParameter(waveTypeParameterID)->get());

}

void WavetableOscillatorSubsection::setParameterValue(const juce::String& parameterID, const juce::String& propertyValue)
{
    if (parameterID == volumeKnob->getParameterID())
        volumeKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == panningKnob->getParameterID())
        panningKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == detuneAmountKnob->getParameterID())
        detuneAmountKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == numVoicesKnob->getParameterID())
        numVoicesKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == fmAmountKnob->getParameterID())
        fmAmountKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == transposeKnob->getParameterID())
        transposeKnob->setValue(propertyValue.getFloatValue());

    else if (parameterID == fmFromComboBox.getName())
        fmFromComboBox.setSelectedItemIndex(propertyValue.getIntValue());

    else if (parameterID == waveTypeComboBox.getName())
        waveTypeComboBox.setSelectedItemIndex(propertyValue.getIntValue());
}

void WavetableOscillatorSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(volumeParameterID, std::make_shared<SliderParameter>("volume"));
    parameterHandler.addSliderParameter(panningParameterID, std::make_shared<SliderParameter>("panning"));
    parameterHandler.addSliderParameter(detuneAmountParameterID, std::make_shared<SliderParameter>("unisonDetuneAmount"));
    parameterHandler.addSliderParameter(numVoicesParameterID, std::make_shared<SliderParameter>("unisonNumVoices"));
    parameterHandler.addSliderParameter(fmAmountParameterID, std::make_shared<SliderParameter>("fmAmount"));
    parameterHandler.addSliderParameter(transposeParameterID, std::make_shared<SliderParameter>("transpose"));


    parameterHandler.addComboBoxParameter(waveTypeParameterID, std::make_shared<ComboBoxParameter>("waveType", juce::StringArray{ "Saw", "Square", "Sine" }, 0));
    parameterHandler.addComboBoxParameter(fmFromParameterID, std::make_shared<ComboBoxParameter>("fmFrom", juce::StringArray{ "1", "2", "3" }, 0));


}

void WavetableOscillatorSubsection::updateFMCombo(juce::Array<int> ids)
{
    int selectedIndex = fmFromComboBox.getSelectedItemIndex();
    fmFromComboBox.clear();
    fmFromComboBox.addItem("None", 1);

    int i = 2;
    for (int id : ids) {
        if (id != getId()) {

            //fmFromComboBox.addItem("FM FROM: " + juce::String(id), i);
            fmFromComboBox.addItem(juce::String(id), i);

            i++;
        }
    }

    fmFromComboBox.setSelectedItemIndex(selectedIndex);

}

void WavetableOscillatorSubsection::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &fmFromComboBox) {
        eventHandler.onFmFromChanged(getIdAsString(), fmFromComboBox.getText());
    }
}
