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

    setDesiredHeight(118);

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
    auto content = getContentArea();
    const int x0 = content.getX();
    const int y0 = content.getY();
    const int knobY = y0 + (content.getHeight() - knobCellHeight) / 2;

    placeKnob(*driveKnob, x0, knobY);
    placeKnob(*mixKnob, x0 + knobWidth, knobY);

    distortionGraph.setBounds(x0 + knobWidth * 2 + 16, y0, 150, content.getHeight());
    distortionTypeComboBox.setBounds(distortionGraph.getRight() + 16, y0 + (content.getHeight() - 26) / 2, 150, 26);
}

void DistortionSubsection::paintCalled(juce::Graphics& g)
{
    distortionGraph.updateParams(driveKnob->getValue());
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
    const auto bounds = getLocalBounds().toFloat();
    Draw::graphBackground(g, bounds, 4, 4);

    // Referencia lineal (sin distorsion)
    g.setColour(Palette::BorderStrong);
    g.drawLine((float)constrainedArea.getX(), (float)constrainedArea.getBottom(),
               (float)constrainedArea.getRight(), (float)constrainedArea.getY(), 1.0f);

    Draw::curve(g, distortionPath, bounds, Palette::Tertiary, bounds.getCentreY(), false);
}

void DistortionSubsection::DistortionGraph::recalculatePoints()
{
    // Limpiar la trayectoria previa.
    distortionPath.clear();

    // Determinar el área restringida.
    constrainedArea = getLocalBounds().reduced(6);

    float width = constrainedArea.getWidth();
    float height = constrainedArea.getHeight();
    float midY = constrainedArea.getY() + height / 2.0f; // Punto medio vertical.

    // Parámetros para la función de distorsión.
    float startX = -1.0f;
    float endX = 1.0f;

    // Crear la trayectoria para la función de distorsión.

    float y2 = std::tanh(-1.0f * drive);
    float scaledY2 = juce::jmap(y2, -1.0f, 1.0f, height, 0.0f); // y invertido para que +1 esté arriba
    // Crear la trayectoria de la función de distorsión (tanh)
    distortionPath.startNewSubPath(constrainedArea.getX(), constrainedArea.getY() + scaledY2);

    for (float x = startX + 0.01f; x <= endX; x += 0.01f) {
        // Escalar x al área gráfica.
        float normalizedX = juce::jmap(x, startX, endX, 0.0f, width);

        // Aplicar la función f(x) = tanh(x * drive).
        float y = std::tanh(x * drive);

        // Escalar y al área gráfica (invertir para que +1 esté arriba).
        float scaledY = juce::jmap(y, -1.0f, 1.0f, height, 0.0f);

        // Dibujar el punto en el gráfico.
        distortionPath.lineTo(constrainedArea.getX() + normalizedX, constrainedArea.getY() + scaledY);
    }
}

void DistortionSubsection::DistortionGraph::updateParams(float drive)
{
    if (this->drive != drive) {
        this->drive = drive;
        recalculatePoints(); // Recalcula la trayectoria del gráfico.
        repaint();           // Solicita redibujar el componente.
    }
}
