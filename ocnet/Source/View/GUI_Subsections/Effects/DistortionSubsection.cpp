/*
  ==============================================================================

    Distortion.cpp
    Created: 1 Aug 2024 3:32:37pm
    Author:  TecNo

  ==============================================================================
*/

#include "DistortionSubsection.h"

DistortionSubsection::DistortionSubsection(int id, GUI_EventHandler& eventHandler) : EffectsSubsection(eventHandler, id, "Distortion")
{
    driveParameterID = createParameterID("Distortion", getId(), "drive");
    distortionTypeParameterID = createParameterID("distortionType");
    driveKnob = std::make_unique<Knob1>(driveParameterID, eventHandler, "Drive");

    this->addAndMakeVisible(*driveKnob);

    driveKnob->setRange(0.0f, 10.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    addAndMakeVisible(distortionGraph);

    distortionTypeComboBox.addItem("Soft Clipping", 1);
    distortionTypeComboBox.addItem("Hard Clipping", 2);
    //waveTypeComboBox.setSelectedId(1); // Selecciona "Saw" por defecto
    distortionTypeComboBox.setName(distortionTypeParameterID);
    addAndMakeVisible(distortionTypeComboBox);
}

void DistortionSubsection::subsectionResized()
{
    auto area = getLocalBounds();

    int posX = 0;
    driveKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    mixKnob->setBounds(posX, 20, defaultKnobSize, defaultKnobSize);
    posX += defaultKnobSize;

    distortionGraph.setBounds(posX, 20+5, 100, defaultKnobSize);
    posX += 100+5;

    distortionTypeComboBox.setBounds(posX, area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize - 10);

    driveKnob->showLabel(*this, *driveKnob);
    mixKnob->showLabel(*this, *mixKnob);
}

void DistortionSubsection::paintCalled(juce::Graphics& g)
{
    distortionGraph.updateParams(driveKnob->getValue());
    distortionGraph.repaint();
}

void DistortionSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    driveParameterAttachment = std::make_unique<OcnetSliderAttachment>(*driveKnob, *parameterHandler.getSliderParameter(driveParameterID)->get());
    distortionTypeParameterAttachment = std::make_unique<OcnetComboBoxAttachment>(distortionTypeComboBox, *parameterHandler.getComboBoxParameter(distortionTypeParameterID)->get());
}

void DistortionSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addComboBoxParameter(distortionTypeParameterID, std::make_shared<ComboBoxParameter>("distortionType", juce::StringArray{ "Soft Clipping", "Hard Clipping"}, 0));
    parameterHandler.addSliderParameter(driveParameterID, std::make_shared<SliderParameter>("drive", 5.0f));
}

void DistortionSubsection::DistortionGraph::paint(juce::Graphics& g)
{
    const float cornerRadius = 5.0f; // Radio para los bordes redondeados
    const float lineThickness = 3.0f;

    // Dibujar fondo con esquinas redondeadas
    juce::Path roundedRect;
    roundedRect.addRoundedRectangle(0, 0, getWidth(), getHeight(), cornerRadius, cornerRadius, true, true, true, true);
    g.setColour(Palette::Section);  // Color del fondo
    g.fillPath(roundedRect);

    // Área restringida para dibujar la función
    auto area = getLocalBounds();
    juce::Rectangle<int> constrainedArea = juce::Rectangle<int>(area.getX() + lineThickness, area.getY() + lineThickness, area.getWidth() - (lineThickness * 2), area.getHeight() - (lineThickness * 2));

    // Parámetros del gráfico
    float width = constrainedArea.getWidth();
    float height = constrainedArea.getHeight();
    float midY = constrainedArea.getY() + height / 2.0f;  // Punto medio vertical

    juce::Path distortionPath;

    // Rango de x que representará el gráfico (desde -1 a 1)
    float startX = -1.0f;
    float endX = 1.0f;

    float y2 = std::tanh(-1.0f * drive);
    float scaledY2 = juce::jmap(y2, -1.0f, 1.0f, height, 0.0f); // y invertido para que +1 esté arriba
    // Crear la trayectoria de la función de distorsión (tanh)
    distortionPath.startNewSubPath(constrainedArea.getX(), constrainedArea.getY() + scaledY2);

    // Generar puntos a lo largo del ancho del área
    for (float x = startX + 0.1f; x <= endX; x += 0.1f)
    {
        // Escalar x al área gráfica
        float normalizedX = juce::jmap(x, startX, endX, 0.0f, width);

        // Aplicar la función f(x) = tanh(x * drive)
        float y = std::tanh(x * drive);

        // Escalar y para que ocupe el área vertical del gráfico (mitad de la altura es la amplitud máxima)
        float scaledY = juce::jmap(y, -1.0f, 1.0f, height, 0.0f); // y invertido para que +1 esté arriba

        // Dibujar el punto en el gráfico
        distortionPath.lineTo(constrainedArea.getX() + normalizedX, constrainedArea.getY() + scaledY);
    }

    // Dibujar la función de distorsión
    g.setColour(juce::Colours::white);  // Color de la línea
    g.strokePath(distortionPath, juce::PathStrokeType(2.0f));  // Grosor de la línea
}

void DistortionSubsection::DistortionGraph::updateParams(float drive)
{
    this->drive = drive;
}
