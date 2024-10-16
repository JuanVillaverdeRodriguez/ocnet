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
    setDesiredHeight(175);

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
    auto area = getLocalBounds();

    int posX = 35;
    attackKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    decayKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    sustainKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    releaseKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    posX = 35 + 5;
    envelopeGraph.setBounds(posX, 75, area.getWidth() - 45, area.getHeight() - 80);

    attackKnob->showLabel(*this, *attackKnob);
    decayKnob->showLabel(*this, *decayKnob);
    sustainKnob->showLabel(*this, *sustainKnob);
    releaseKnob->showLabel(*this, *releaseKnob);

    int lastX = 0;
    for (auto& modulationBubble : modulationBubblesVector) {
        juce::Rectangle<int> globalBounds = this->getParentComponent()->getParentComponent()->getLocalArea(this, dragZone.getBounds());

        modulationBubble->setBounds(globalBounds.getX() + lastX, globalBounds.getY(), defaultKnobSize - 25, defaultKnobSize - 25);
        lastX += modulationBubble->getBounds().getWidth() + 3;
    }

}

void EnvelopeSubsection::paintCalled(juce::Graphics& g)
{
    envelopeGraph.updateParams(
        attackKnob->getValue(), 
        decayKnob->getValue(), 
        sustainKnob->getValue(), 
        releaseKnob->getValue()
    );


    envelopeGraph.repaint();
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
    // Definir las dimensiones del gr�fico de ADSR
//auto area = getLocalBounds().reduced(10); // Margen dentro del componente
    const float cornerRadius = 5.0f; // Radio para los bordes redondeados
    const float lineThickness = 3.0f;

    juce::Path roundedRect;
    roundedRect.addRoundedRectangle(0, 0, getWidth(), getHeight(), cornerRadius, cornerRadius, true, true, true, true);
    g.setColour(Palette::Section);
    g.fillPath(roundedRect);

    auto area = getLocalBounds();
    juce::Rectangle<int> constrainedArea = juce::Rectangle<int>(area.getX() + lineThickness, area.getY() + lineThickness, area.getWidth() - (lineThickness * 2), area.getHeight() - (lineThickness*2));

    float width = constrainedArea.getWidth();
    float height = constrainedArea.getHeight();

    // Definir puntos iniciales y proporciones
    juce::Point<float> start(constrainedArea.getX(), constrainedArea.getBottom()); // Inicio en el fondo (punto 0)
    juce::Point<float> attackPoint(start.getX() + attack * width, constrainedArea.getY()); // Punto final del ataque (sube a la parte superior)
    juce::Point<float> decayPoint(attackPoint.getX() + decay * width, constrainedArea.getBottom() - sustain * height); // Decae hasta el nivel de sustain
    juce::Point<float> sustainPoint(decayPoint.getX(), decayPoint.getY()); // Mantiene sustain
    juce::Point<float> releasePoint(sustainPoint.getX() + release * width, constrainedArea.getBottom()); // Baja hasta el fondo en release

    // Dibujar l�neas del ADSR
    g.setColour(Palette::White); // Color de las l�neas
    g.drawLine(start.getX(), start.getY(), attackPoint.getX(), attackPoint.getY(), lineThickness); // Ataque
    g.drawLine(attackPoint.getX(), attackPoint.getY(), decayPoint.getX(), decayPoint.getY(), lineThickness); // Decay
    g.drawLine(decayPoint.getX(), decayPoint.getY(), sustainPoint.getX(), sustainPoint.getY(), lineThickness); // Sustain
    g.drawLine(sustainPoint.getX(), sustainPoint.getY(), releasePoint.getX(), releasePoint.getY(), lineThickness); // Release
}

void EnvelopeSubsection::EnvelopeGraph::updateParams(float attack, float decay, float sustain, float release)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
}