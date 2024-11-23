/*
  ==============================================================================

    EnvelopeSubsection.h
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../../../View/GUI_Elements/Knobs/Knob1.h"
#include "../Subsection.h"
#include <JuceHeader.h>
#include "../../../View/GUI_Elements/ModulatorDragZone/DragZone.h"
#include "../../../Controller/GUI_EventHandler.h"
#include "ModulatorsSubsection.h"
#include "../../../Model/ParameterHandler/OcnetSliderAttachment.h"

class EnvelopeSubsection : public ModulatorsSubsection 
{
public:
    EnvelopeSubsection(int id, GUI_EventHandler& eventHandler);
    ~EnvelopeSubsection() = default;

    void attachParameters(ParameterHandler& parameterHandler);
    void addParametersToParameterHandler(ParameterHandler& parameterHandler) override;
    void subsectionResized() override;
    void paintCalled(juce::Graphics& g) override;

private:
    struct EnvelopeGraph : public juce::Component {
    public:
        void paint(juce::Graphics& g) override;
        void updateParams(float attack, float decay, float sustain, float release);

    private:
        void recalculatePoints(); // Método para recalcular los puntos del ADSR.

        float attack = 0.0f;
        float decay = 0.0f;
        float sustain = 0.0f;
        float release = 0.0f;

        // Puntos precomputados para el gráfico ADSR.
        juce::Point<float> start;
        juce::Point<float> attackPoint;
        juce::Point<float> decayPoint;
        juce::Point<float> sustainPoint;
        juce::Point<float> releasePoint;

        juce::Rectangle<int> constrainedArea; // Área restringida para el gráfico.
    };

    EnvelopeGraph envelopeGraph;

    juce::String attackParameterID;
    juce::String decayParameterID;
    juce::String sustainParameterID;
    juce::String releaseParameterID;

    std::unique_ptr<Knob1> attackKnob;
    std::unique_ptr<OcnetSliderAttachment> attackParameterAttachment;

    std::unique_ptr<Knob1> decayKnob;
    std::unique_ptr<OcnetSliderAttachment> decayParameterAttachment;

    std::unique_ptr<Knob1> sustainKnob;
    std::unique_ptr<OcnetSliderAttachment> sustainParameterAttachment;

    std::unique_ptr<Knob1> releaseKnob;
    std::unique_ptr<OcnetSliderAttachment> releaseParameterAttachment;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSubsection)
};