/*
  ==============================================================================

    EnvelopeSubsection.cpp
    Created: 8 Jul 2024 4:53:10pm
    Author:  TecNo

  ==============================================================================
*/

#include "EnvelopeSubsection.h"

EnvelopeSubsection::EnvelopeSubsection(int id, GUI_EventHandler& eventHandler) : ModulatorsSubsection(eventHandler, id, "Envelope")
{
    setDesiredHeight(132);

    // Significa que es el envelope principal, no se puede eliminar
    if (id == 0) {
        removeButton.setEnabled(false);
        bypassButton.setEnabled(false);
        bypassButton.setVisible(false);
    }

    attackParameterID = createParameterID("Envelope", getId(), "attack");
    decayParameterID = createParameterID("Envelope", getId(), "decay");
    sustainParameterID = createParameterID("Envelope", getId(), "sustain");
    releaseParameterID = createParameterID("Envelope", getId(), "release");

    attackKnob = std::make_unique<Knob1>(attackParameterID, eventHandler, "Attack");
    decayKnob = std::make_unique<Knob1>(decayParameterID, eventHandler, "Decay");
    sustainKnob = std::make_unique<Knob1>(sustainParameterID, eventHandler, "Sustain");
    releaseKnob = std::make_unique<Knob1>(releaseParameterID, eventHandler, "Release");

    attackKnob->shouldRepaintParentWhenChanged(true);
    decayKnob->shouldRepaintParentWhenChanged(true);
    sustainKnob->shouldRepaintParentWhenChanged(true);
    releaseKnob->shouldRepaintParentWhenChanged(true);

    attackKnob->setRange(0.0f, 1.0f, 0.01f);
    decayKnob->setRange(0.0f, 1.0f, 0.01f);
    sustainKnob->setRange(0.0f, 1.0f, 0.01f);
    releaseKnob->setRange(0.0f, 1.0f, 0.01f);

    /*attackKnob->setValue(0.1f);
    decayKnob->setValue(0.5f);
    sustainKnob->setValue(1.0f);
    releaseKnob->setValue(0.1f);*/

    this->addAndMakeVisible(*attackKnob);
    this->addAndMakeVisible(*decayKnob);
    this->addAndMakeVisible(*sustainKnob);
    this->addAndMakeVisible(*releaseKnob);

    this->addAndMakeVisible(envelopeGraph);

}

void EnvelopeSubsection::subsectionResized()
{
    auto content = getContentArea();
    const int knobY = content.getY() + (content.getHeight() - knobCellHeight) / 2;

    int posX = content.getX();
    placeKnob(*attackKnob, posX, knobY);
    posX += knobWidth;
    placeKnob(*decayKnob, posX, knobY);
    posX += knobWidth;
    placeKnob(*sustainKnob, posX, knobY);
    posX += knobWidth;
    placeKnob(*releaseKnob, posX, knobY);
    posX += knobWidth + 12;

    envelopeGraph.setBounds(content.withLeft(posX));

    layoutModulationBubbles();
}

void EnvelopeSubsection::paintCalled(juce::Graphics& g)
{
    envelopeGraph.updateParams(
        attackKnob->getValue(), 
        decayKnob->getValue(), 
        sustainKnob->getValue(), 
        releaseKnob->getValue()
    );
}


void EnvelopeSubsection::attachParameters(ParameterHandler& parameterHandler) {
    attackParameterAttachment = std::make_unique<OcnetSliderAttachment>(*attackKnob, *parameterHandler.getSliderParameter(attackParameterID)->get());
    decayParameterAttachment = std::make_unique<OcnetSliderAttachment>(*decayKnob, *parameterHandler.getSliderParameter(decayParameterID)->get());
    sustainParameterAttachment = std::make_unique<OcnetSliderAttachment>(*sustainKnob, *parameterHandler.getSliderParameter(sustainParameterID)->get());
    releaseParameterAttachment = std::make_unique<OcnetSliderAttachment>(*releaseKnob, *parameterHandler.getSliderParameter(releaseParameterID)->get());

}

// Mover a audioProcessor
void EnvelopeSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{

    parameterHandler.addSliderParameter(attackParameterID, std::make_shared<SliderParameter>("attack", 0.1f));
    parameterHandler.addSliderParameter(decayParameterID, std::make_shared<SliderParameter>("decay", 0.5f));
    parameterHandler.addSliderParameter(sustainParameterID, std::make_shared<SliderParameter>("sustain", 1.0f));
    parameterHandler.addSliderParameter(releaseParameterID, std::make_shared<SliderParameter>("release", 0.1f));

    //parameterHandler.getSliderParameter(attackParameterID)->get()->setValue(0.1f);
    //parameterHandler.getSliderParameter(decayParameterID)->get()->setValue(0.5f);
    //parameterHandler.getSliderParameter(sustainParameterID)->get()->setValue(1.0f);
    //parameterHandler.getSliderParameter(releaseParameterID)->get()->setValue(0.1f);
}

void EnvelopeSubsection::EnvelopeGraph::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    Draw::graphBackground(g, bounds);

    juce::Path envelope;
    envelope.startNewSubPath(start);
    envelope.lineTo(attackPoint);
    envelope.lineTo(decayPoint);
    envelope.lineTo(sustainPoint);
    envelope.lineTo(releasePoint);

    Draw::curve(g, envelope, bounds, Palette::Modulation, (float)constrainedArea.getBottom());

    // Puntos de control
    for (auto point : { attackPoint, decayPoint, sustainPoint }) {
        g.setColour(Palette::Inset);
        g.fillEllipse(juce::Rectangle<float>(7.0f, 7.0f).withCentre(point));
        g.setColour(Palette::Modulation);
        g.drawEllipse(juce::Rectangle<float>(7.0f, 7.0f).withCentre(point), 1.5f);
    }
}

void EnvelopeSubsection::EnvelopeGraph::recalculatePoints()
{
    // Area util del grafico (con margen para el trazo y los puntos)
    constrainedArea = getLocalBounds().reduced(8, 8);

    const float width = (float)constrainedArea.getWidth();
    const float height = (float)constrainedArea.getHeight();

    // Ataque, decay y release ocupan como maximo 1/3.3 del ancho cada uno y el
    // sustain un tramo fijo, para que la envolvente siempre quepa en el grafico.
    const float segment = width / 3.3f;
    const float sustainHold = segment * 0.3f;

    start = juce::Point<float>((float)constrainedArea.getX(), (float)constrainedArea.getBottom());
    attackPoint = juce::Point<float>(start.getX() + attack * segment, (float)constrainedArea.getY());
    decayPoint = juce::Point<float>(attackPoint.getX() + decay * segment, constrainedArea.getBottom() - sustain * height);
    sustainPoint = juce::Point<float>(decayPoint.getX() + sustainHold, decayPoint.getY());
    releasePoint = juce::Point<float>(sustainPoint.getX() + release * segment, (float)constrainedArea.getBottom());
}

void EnvelopeSubsection::EnvelopeGraph::updateParams(float attack, float decay, float sustain, float release)
{
    // Solo repintamos este componente si los valores han cambiado
    if (this->attack != attack || this->decay != decay ||
        this->sustain != sustain || this->release != release)
    {
        this->attack = attack;
        this->decay = decay;
        this->sustain = sustain;
        this->release = release;

        recalculatePoints(); // Recalcular los puntos del gráfico.
        repaint(); // Esto no afecta al padre.
    }
}
