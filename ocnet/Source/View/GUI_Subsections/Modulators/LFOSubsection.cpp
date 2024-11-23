/*
  ==============================================================================

    LFOSubsection.cpp
    Created: 12 Jul 2024 1:37:08pm
    Author:  TecNo

  ==============================================================================
*/

#include "LFOSubsection.h"

LFOSubsection::LFOSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "LFO")
{
    setDesiredHeight(175);

    speedParameterID = createParameterID("freq");
    tempoComboParameterID = createParameterID("tempo");

    speedKnob = std::make_unique<Knob1>(speedParameterID, eventHandler, "Frequency");

    speedKnob->setRange(1.0f, 20.0f, 0.01f);

    //speedKnob->setValue(0.5f);

    this->addAndMakeVisible(*speedKnob);

    tempoComboBox.addItem("Free", 1);
    tempoComboBox.addItem("1/1", 2);
    tempoComboBox.addItem("1/2", 3);
    tempoComboBox.addItem("1/4", 4);
    tempoComboBox.addItem("1/8", 5);
    tempoComboBox.addItem("1/16", 6);
    tempoComboBox.setName(tempoComboParameterID);
    addAndMakeVisible(tempoComboBox);
    addAndMakeVisible(lfoGraph);
}

void LFOSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    speedParameterAttachment = std::make_unique<OcnetSliderAttachment>(*speedKnob, *parameterHandler.getSliderParameter(speedParameterID)->get());
    tempoComboBoxAttachment = std::make_unique<OcnetComboBoxAttachment>(tempoComboBox, *parameterHandler.getComboBoxParameter(tempoComboParameterID)->get());
}

void LFOSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(speedParameterID, std::make_shared<SliderParameter>("freq", 1.0f));
    parameterHandler.addComboBoxParameter(tempoComboParameterID, std::make_shared<ComboBoxParameter>("tempo", juce::StringArray{ "Free", "1/1", "1/2", "1/4", "1/8", "1/16"}, 0));
}

void LFOSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 35;
    tempoComboBox.setBounds(10 + posX, 25, defaultKnobSize * 2, defaultKnobSize - 10);
    posX += defaultKnobSize * 2;
    
    speedKnob->setBounds(10 + posX, 20, defaultKnobSize+30, defaultKnobSize);
    speedKnob->showLabel(*this, *speedKnob);


    posX = 35 + 5;
    lfoGraph.setBounds(posX, 75, area.getWidth() - 45, area.getHeight() - 80);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }
}

void LFOSubsection::paintCalled(juce::Graphics& g)
{
    lfoGraph.updateParams(speedKnob->getValue());
}

void LFOSubsection::LFOGraph::paint(juce::Graphics& g)
{
    const float cornerRadius = 5.0f;

    // Dibujar fondo con esquinas redondeadas.
    juce::Path roundedRect;
    roundedRect.addRoundedRectangle(0, 0, getWidth(), getHeight(), cornerRadius, cornerRadius, true, true, true, true);
    g.setColour(Palette::Section);
    g.fillPath(roundedRect);

    // Dibujar la onda sinusoidal precomputada.
    g.setColour(juce::Colours::white);
    g.strokePath(sineWavePath, juce::PathStrokeType(2.0f));
}
void LFOSubsection::LFOGraph::updateParams(float freq)
{

    if (this->freq != freq) {
        this->freq = freq;

        recalculatePoints(); // Recalcular los puntos del gráfico.
        repaint(); // Esto no afecta al padre.
    }

}

void LFOSubsection::LFOGraph::recalculatePoints()
{
    // Limpiar la trayectoria previa.
    sineWavePath.clear();

    // Determinar el área restringida.
    const float lineThickness = 3.0f;
    auto area = getLocalBounds();
    constrainedArea = juce::Rectangle<int>(
        area.getX() + lineThickness,
        area.getY() + lineThickness,
        area.getWidth() - (lineThickness * 2),
        area.getHeight() - (lineThickness * 2)
    );

    float width = constrainedArea.getWidth();
    float height = constrainedArea.getHeight();
    float midY = constrainedArea.getY() + height / 2.0f; // Punto medio vertical.

    // Parámetros de la onda.
    float amplitude = height / 2.0f; // Amplitud de la onda (mitad de la altura).
    float frequency = freq / width; // Frecuencia relativa a la anchura.

    // Construir la onda sinusoidal.
    sineWavePath.startNewSubPath(constrainedArea.getX(), midY);
    for (float x = 0; x < width; ++x) {
        float y = std::sin(x * juce::MathConstants<float>::twoPi * frequency) * amplitude;
        sineWavePath.lineTo(constrainedArea.getX() + x, midY - y);
    }
}