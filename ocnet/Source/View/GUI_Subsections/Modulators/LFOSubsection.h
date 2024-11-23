/*
  ==============================================================================

    LFOSubsection.h
    Created: 12 Jul 2024 1:37:08pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "ModulatorsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetComboBoxAttachment.h"

#include <JuceHeader.h>

class LFOSubsection : public ModulatorsSubsection {
public:
    LFOSubsection(int id, GUI_EventHandler& eventHandler);
    ~LFOSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler) override;
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;
    void paintCalled(juce::Graphics& g) override;

private:
    struct LFOGraph : public juce::Component {
    public:
        void paint(juce::Graphics& g) override;
        void updateParams(float freq);

    private:
        void recalculatePoints(); // Método para recalcular los puntos de la onda.

        float freq = 1.0f;

        juce::Rectangle<int> constrainedArea; // Área restringida para la onda.
        juce::Path sineWavePath; // Ruta precomputada para la onda.
    };

    LFOGraph lfoGraph;

    juce::String speedParameterID;
    juce::String tempoComboParameterID;

    std::unique_ptr<Knob1> speedKnob;
    std::unique_ptr<OcnetSliderAttachment> speedParameterAttachment;

    OcnetComboBox tempoComboBox;
    std::unique_ptr<OcnetComboBoxAttachment> tempoComboBoxAttachment;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOSubsection)

};