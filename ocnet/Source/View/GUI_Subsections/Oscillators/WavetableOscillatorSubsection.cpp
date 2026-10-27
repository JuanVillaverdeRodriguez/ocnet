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
    setDesiredHeight(136);

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
    numVoicesKnob = std::make_unique<Knob1>(numVoicesParameterID, eventHandler, "Voices");
    detuneAmountKnob = std::make_unique<Knob1>(detuneAmountParameterID, eventHandler, "Detune");
    fmAmountKnob = std::make_unique<Knob1>(fmAmountParameterID, eventHandler, "FM");
    transposeKnob = std::make_unique<Knob1>(transposeParameterID, eventHandler, "Transpose");


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

    /*
    volumeKnob->setValue(0.75f);
    panningKnob->setValue(0.5f);
    numVoicesKnob->setValue(1);
    detuneAmountKnob->setValue(0.20f);
    fmAmountKnob->setValue(1.0f);
    transposeKnob->setValue(0);*/

    // Configuración del ComboBox
    waveTypeComboBox.addItem("Saw", 1);
    waveTypeComboBox.addItem("Square", 2);
    waveTypeComboBox.addItem("Sine", 3);
    //waveTypeComboBox.setSelectedId(1); // Selecciona "Saw" por defecto
    waveTypeComboBox.setName(waveTypeParameterID);
    addAndMakeVisible(waveTypeComboBox);

    // Configuración del ComboBox
    fmFromComboBox.addItem("None", 1);
    //fmFromComboBox.setSelectedId(1);
    fmFromComboBox.setName(fmFromParameterID);

    addAndMakeVisible(fmFromComboBox);

    fmFromComboBox.addListener(this);


}

void WavetableOscillatorSubsection::subsectionResized()
{
    auto content = getContentArea();
    const int x0 = content.getX();
    const int comboY = content.getY();
    const int knobY = comboY + 26 + 8;
    const int groupGap = 25;

    // Oscilador
    waveTypeComboBox.setBounds(x0, comboY, knobWidth * 3, 26);
    placeKnob(*volumeKnob, x0, knobY);
    placeKnob(*panningKnob, x0 + knobWidth, knobY);
    placeKnob(*transposeKnob, x0 + knobWidth * 2, knobY);

    // Unison
    const int unisonX = x0 + knobWidth * 3 + groupGap * 2;
    placeKnob(*numVoicesKnob, unisonX, knobY);
    placeKnob(*detuneAmountKnob, unisonX + knobWidth, knobY);

    // FM
    const int fmX = unisonX + knobWidth * 2 + groupGap * 2;
    fmFromComboBox.setBounds(fmX, comboY, knobWidth * 2, 26);
    placeKnob(*fmAmountKnob, fmX + knobWidth / 2, knobY);
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

void WavetableOscillatorSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(volumeParameterID, std::make_shared<SliderParameter>("volume", 0.75f));
    parameterHandler.addSliderParameter(panningParameterID, std::make_shared<SliderParameter>("panning", 0.5f));
    parameterHandler.addSliderParameter(detuneAmountParameterID, std::make_shared<SliderParameter>("unisonDetuneAmount", 0.20f));
    parameterHandler.addSliderParameter(numVoicesParameterID, std::make_shared<SliderParameter>("unisonNumVoices", 1));
    parameterHandler.addSliderParameter(fmAmountParameterID, std::make_shared<SliderParameter>("fmAmount", 1.0f));
    parameterHandler.addSliderParameter(transposeParameterID, std::make_shared<SliderParameter>("transpose", 0));


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

void WavetableOscillatorSubsection::paintCalled(juce::Graphics& g)
{
    // Separadores verticales entre grupos (oscilador | unison | FM)
    const int top = waveTypeComboBox.getY();
    const int bottom = volumeKnob->getBottom() + 8;
    const int firstDividerX = (transposeKnob->getRight() + numVoicesKnob->getX()) / 2;
    const int secondDividerX = (detuneAmountKnob->getRight() + fmFromComboBox.getX()) / 2;

    g.setColour(Palette::Border);
    g.drawVerticalLine(firstDividerX, (float)top, (float)bottom);
    g.drawVerticalLine(secondDividerX, (float)top, (float)bottom);

    // Titulo del grupo unison (los otros grupos tienen su combo)
    g.setColour(Palette::TextFaint);
    g.setFont(Fonts::caps(10.0f, true, 0.15f));
    g.drawText("UNISON", juce::Rectangle<int>(numVoicesKnob->getX(), top, knobWidth * 2, 26), juce::Justification::centred);
}

