/*
  ==============================================================================

    EqualizerSubsection.cpp
    Created: 31 Aug 2024 12:11:04pm
    Author:  TecNo

  ==============================================================================
*/

#include "EqualizerSubsection.h"

EqualizerSubsection::EqualizerSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler, id, "Equalizer")
{
    setDesiredHeight(200);

    gainLParameterID = createParameterID("gainL");
    cutOffLParameterID = createParameterID("cutOffL");
    resonanceLParameterID = createParameterID("resonanceL");

    gainMParameterID = createParameterID("gainM");
    cutOffMParameterID = createParameterID("cutOffM");
    resonanceMParameterID = createParameterID("resonanceM");

    gainHParameterID = createParameterID("gainH");
    cutOffHParameterID = createParameterID("cutOffH");
    resonanceHParameterID = createParameterID("resonanceH");

    lowFreqParameterID = createParameterID("lowFreq");
    midFreqParameterID = createParameterID("midFreq");
    highFreqParameterID = createParameterID("highFreq");

    lowCutSlopeSelectorParameterID = createParameterID("lowCutSlope");
    highCutSlopeSelectorParameterID = createParameterID("highCutSlope");


    gainLKnob = std::make_unique<Knob1>(gainLParameterID, eventHandler, "Gain");
    cutOffLKnob = std::make_unique<Knob1>(cutOffLParameterID, eventHandler, "CutOff");
    resonanceLKnob = std::make_unique<Knob1>(resonanceLParameterID, eventHandler, "Resonance");

    gainMKnob = std::make_unique<Knob1>(gainMParameterID, eventHandler, "Gain");
    cutOffMKnob = std::make_unique<Knob1>(cutOffMParameterID, eventHandler, "CutOff");
    resonanceMKnob = std::make_unique<Knob1>(resonanceMParameterID, eventHandler, "Resonance");

    gainHKnob = std::make_unique<Knob1>(gainHParameterID, eventHandler, "Gain");
    cutOffHKnob = std::make_unique<Knob1>(cutOffHParameterID, eventHandler, "CutOff");
    resonanceHKnob = std::make_unique<Knob1>(resonanceHParameterID, eventHandler, "Resonance");

    lowFreqButton.setButtonText("LOW");
    midFreqButton.setButtonText("MID");
    highFreqButton.setButtonText("HIGH");

    lowFreqButton.setLookAndFeel(&lookAndFeelEqButtons);
    midFreqButton.setLookAndFeel(&lookAndFeelEqButtons);
    highFreqButton.setLookAndFeel(&lookAndFeelEqButtons);

    lowFreqButton.setToggleState(true, juce::dontSendNotification);
    lowFreqButton.setToggleState(false, juce::dontSendNotification);
    lowFreqButton.setToggleState(false, juce::dontSendNotification);

    lowFreqButton.setName(lowFreqParameterID);
    midFreqButton.setName(midFreqParameterID);
    highFreqButton.setName(highFreqParameterID);

    gainLKnob->setRange(-24.0f, 24.0f, 0.01f);
    cutOffLKnob->setRange(20.0f, 20000.0f, 1.0f);
    resonanceLKnob->setRange(0.1f, 10.0f, 0.01f);

    gainMKnob->setRange(-24.0f, 24.0f, 0.01f);
    cutOffMKnob->setRange(20.0f, 20000.0f, 1.0f);
    resonanceMKnob->setRange(0.1f, 10.0f, 0.01f);

    gainHKnob->setRange(-24.0f, 24.0f, 0.01f);
    cutOffHKnob->setRange(20.0f, 20000.0f, 1.0f);
    resonanceHKnob->setRange(0.1f, 10.0f, 0.01f);

    cutOffLKnob->setSkewFactor(0.2f);
    cutOffMKnob->setSkewFactor(0.2f);
    cutOffHKnob->setSkewFactor(0.2f);

    lowFreqButton.setToggleable(true);
    midFreqButton.setToggleable(true);
    highFreqButton.setToggleable(true);

    lowFreqButton.addListener(this);
    midFreqButton.addListener(this);
    highFreqButton.addListener(this);

    lowCutSlopeSelectorComboBox.addItem("12db/Oct", 1);
    lowCutSlopeSelectorComboBox.addItem("24db/Oct", 2);
    lowCutSlopeSelectorComboBox.addItem("36db/Oct", 3);
    lowCutSlopeSelectorComboBox.addItem("48db/Oct", 4);

    highCutSlopeSelectorComboBox.addItem("12db/Oct", 1);
    highCutSlopeSelectorComboBox.addItem("24db/Oct", 2);
    highCutSlopeSelectorComboBox.addItem("36db/Oct", 3);
    highCutSlopeSelectorComboBox.addItem("48db/Oct", 4);

    highCutSlopeSelectorComboBox.setName(highCutSlopeSelectorParameterID);
    lowCutSlopeSelectorComboBox.setName(lowCutSlopeSelectorParameterID);

    lowCutSlopeLabel.setText("LowCut slope", juce::NotificationType::dontSendNotification);
    highCutSlopeLabel.setText("HighCut slope", juce::NotificationType::dontSendNotification);

    addAndMakeVisible(*gainLKnob);
    addAndMakeVisible(*cutOffLKnob);
    addAndMakeVisible(*resonanceLKnob);

    addAndMakeVisible(*gainMKnob);
    addAndMakeVisible(*cutOffMKnob);
    addAndMakeVisible(*resonanceMKnob);

    addAndMakeVisible(*gainHKnob);
    addAndMakeVisible(*cutOffHKnob);
    addAndMakeVisible(*resonanceHKnob);

    addAndMakeVisible(lowFreqButton);
    addAndMakeVisible(midFreqButton);
    addAndMakeVisible(highFreqButton);

    addAndMakeVisible(lowCutSlopeSelectorComboBox);
    addAndMakeVisible(highCutSlopeSelectorComboBox);

    addAndMakeVisible(lowCutSlopeLabel);
    addAndMakeVisible(highCutSlopeLabel);

}

void EqualizerSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    gainLParameterAttachment = std::make_unique<OcnetSliderAttachment>(*gainLKnob, *parameterHandler.getSliderParameter(gainLParameterID)->get());
    cutOffLParameterAttachment = std::make_unique<OcnetSliderAttachment>(*cutOffLKnob, *parameterHandler.getSliderParameter(cutOffLParameterID)->get());
    resonanceLParameterAttachment = std::make_unique<OcnetSliderAttachment>(*resonanceLKnob, *parameterHandler.getSliderParameter(resonanceLParameterID)->get());

    gainMParameterAttachment = std::make_unique<OcnetSliderAttachment>(*gainMKnob, *parameterHandler.getSliderParameter(gainMParameterID)->get());
    cutOffMParameterAttachment = std::make_unique<OcnetSliderAttachment>(*cutOffMKnob, *parameterHandler.getSliderParameter(cutOffMParameterID)->get());
    resonanceMParameterAttachment = std::make_unique<OcnetSliderAttachment>(*resonanceMKnob, *parameterHandler.getSliderParameter(resonanceMParameterID)->get());

    gainHParameterAttachment = std::make_unique<OcnetSliderAttachment>(*gainHKnob, *parameterHandler.getSliderParameter(gainHParameterID)->get());
    cutOffHParameterAttachment = std::make_unique<OcnetSliderAttachment>(*cutOffHKnob, *parameterHandler.getSliderParameter(cutOffHParameterID)->get());
    resonanceHParameterAttachment = std::make_unique<OcnetSliderAttachment>(*resonanceHKnob, *parameterHandler.getSliderParameter(resonanceHParameterID)->get());

    lowFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(lowFreqButton, *parameterHandler.getButtonParameter(lowFreqParameterID)->get());
    midFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(midFreqButton, *parameterHandler.getButtonParameter(midFreqParameterID)->get());
    highFreqButtonAttachment = std::make_unique<ButtonParameterAttachment>(highFreqButton, *parameterHandler.getButtonParameter(highFreqParameterID)->get());

    lowCutSlopeSelectorComboBoxAttachment = std::make_unique<OcnetComboBoxAttachment>(lowCutSlopeSelectorComboBox, *parameterHandler.getComboBoxParameter(lowCutSlopeSelectorParameterID)->get());
    highCutSlopeSelectorComboBoxAttachment = std::make_unique<OcnetComboBoxAttachment>(highCutSlopeSelectorComboBox, *parameterHandler.getComboBoxParameter(highCutSlopeSelectorParameterID)->get());
}

void EqualizerSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(gainLParameterID, std::make_shared<SliderParameter>("gainL", 0.5f));
    parameterHandler.addSliderParameter(cutOffLParameterID, std::make_shared<SliderParameter>("cutOffL", 20.0f));
    parameterHandler.addSliderParameter(resonanceLParameterID, std::make_shared<SliderParameter>("resonanceL", 0.5f));

    parameterHandler.addSliderParameter(gainMParameterID, std::make_shared<SliderParameter>("gainM", 0.5f));
    parameterHandler.addSliderParameter(cutOffMParameterID, std::make_shared<SliderParameter>("cutOffM", 500.0f));
    parameterHandler.addSliderParameter(resonanceMParameterID, std::make_shared<SliderParameter>("resonanceM", 0.5f));

    parameterHandler.addSliderParameter(gainHParameterID, std::make_shared<SliderParameter>("gainH", 0.5f));
    parameterHandler.addSliderParameter(cutOffHParameterID, std::make_shared<SliderParameter>("cutOffH", 20000.0f));
    parameterHandler.addSliderParameter(resonanceHParameterID, std::make_shared<SliderParameter>("resonanceH", 0.5f));

    parameterHandler.addButtonParameter(lowFreqParameterID, std::make_shared<ButtonParameter>("lowFreq", true));
    parameterHandler.addButtonParameter(midFreqParameterID, std::make_shared<ButtonParameter>("midFreq", false));
    parameterHandler.addButtonParameter(highFreqParameterID, std::make_shared<ButtonParameter>("highFreq", false));

    parameterHandler.addComboBoxParameter(lowCutSlopeSelectorParameterID, std::make_shared<ComboBoxParameter>("lowCutSlope", juce::StringArray{ "12db/Oct", "24db/Oct", "36db/Oct", "48db/Oct"}, 0));
    parameterHandler.addComboBoxParameter(highCutSlopeSelectorParameterID, std::make_shared<ComboBoxParameter>("highCutSlope", juce::StringArray{ "12db/Oct", "24db/Oct", "36db/Oct", "48db/Oct" }, 0));
}

void EqualizerSubsection::subsectionResized()
{
    auto area = getLocalBounds();
    const int buttonsY = 20;
    const int knobsY = 60;
    const int combosY = 60 + defaultKnobSize + 5;

    int posX = 0;
    gainLKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    cutOffLKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    resonanceLKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    posX = 0;
    gainMKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    cutOffMKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    resonanceMKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    posX = 0;
    gainHKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    cutOffHKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    resonanceHKnob->setBounds(posX, knobsY, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    posX = 0;
    lowFreqButton.setBounds(posX, buttonsY, 60, 40);
    posX += 60;

    midFreqButton.setBounds(posX, buttonsY, 60, 40);
    posX += 60;

    highFreqButton.setBounds(posX, buttonsY, 60, 40);
    posX += 60;

    posX = 0;
    lowCutSlopeSelectorComboBox.setBounds(posX, combosY + 4, 100, 40);
    posX += 100;

    highCutSlopeSelectorComboBox.setBounds(posX, combosY + 4, 100, 40);
    posX += 100;

    lowCutSlopeLabel.setBounds(lowCutSlopeSelectorComboBox.getBounds().getX(), lowCutSlopeSelectorComboBox.getBounds().getY() + lowCutSlopeSelectorComboBox.getBounds().getHeight() + 2, lowCutSlopeSelectorComboBox.getBounds().getWidth(), 20);
    highCutSlopeLabel.setBounds(highCutSlopeSelectorComboBox.getBounds().getX(), highCutSlopeSelectorComboBox.getBounds().getY() + highCutSlopeSelectorComboBox.getBounds().getHeight() + 2, highCutSlopeSelectorComboBox.getBounds().getWidth(), 20);

    gainLKnob->showLabel(*this, *gainLKnob);
    cutOffLKnob->showLabel(*this, *cutOffLKnob);
    resonanceLKnob->showLabel(*this, *resonanceLKnob);

    gainMKnob->showLabel(*this, *gainMKnob);
    cutOffMKnob->showLabel(*this, *cutOffMKnob);
    resonanceMKnob->showLabel(*this, *resonanceMKnob);

    gainHKnob->showLabel(*this, *gainHKnob);
    cutOffHKnob->showLabel(*this, *cutOffHKnob);
    resonanceHKnob->showLabel(*this, *resonanceHKnob);
}

void EqualizerSubsection::buttonClickedCalled(juce::Button* clickedButton)
{
    if (clickedButton == &lowFreqButton) {
        lowFreqButton.setToggleState(true, juce::dontSendNotification);
        midFreqButton.setToggleState(false, juce::dontSendNotification);
        highFreqButton.setToggleState(false, juce::dontSendNotification);


        gainLKnob->setVisible(true);
        cutOffLKnob->setVisible(true);
        resonanceLKnob->setVisible(true);

        gainMKnob->setVisible(false);
        cutOffMKnob->setVisible(false);
        resonanceMKnob->setVisible(false);

        gainHKnob->setVisible(false);
        cutOffHKnob->setVisible(false);
        resonanceHKnob->setVisible(false);
    }

    else if (clickedButton == &midFreqButton) {
        lowFreqButton.setToggleState(false, juce::dontSendNotification);
        midFreqButton.setToggleState(true, juce::dontSendNotification);
        highFreqButton.setToggleState(false, juce::dontSendNotification);

        gainLKnob->setVisible(false);
        cutOffLKnob->setVisible(false);
        resonanceLKnob->setVisible(false);

        gainMKnob->setVisible(true);
        cutOffMKnob->setVisible(true);
        resonanceMKnob->setVisible(true);

        gainHKnob->setVisible(false);
        cutOffHKnob->setVisible(false);
        resonanceHKnob->setVisible(false);

    }

    else if (clickedButton == &highFreqButton) {
        lowFreqButton.setToggleState(false, juce::dontSendNotification);
        midFreqButton.setToggleState(false, juce::dontSendNotification);
        highFreqButton.setToggleState(true, juce::dontSendNotification);

        gainLKnob->setVisible(false);
        cutOffLKnob->setVisible(false);
        resonanceLKnob->setVisible(false);

        gainMKnob->setVisible(false);
        cutOffMKnob->setVisible(false);
        resonanceMKnob->setVisible(false);

        gainHKnob->setVisible(true);
        cutOffHKnob->setVisible(true);
        resonanceHKnob->setVisible(true);

    }
}
