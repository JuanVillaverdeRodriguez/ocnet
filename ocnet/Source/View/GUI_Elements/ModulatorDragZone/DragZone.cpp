/*
  ==============================================================================

    DragZone.cpp
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#include "DragZone.h"
#include "../../GUI_Subsections/Modulators/ModulatorsSubsection.h"

DragZone::DragZone(GUI_EventHandler& eventHandler) : mouseHovering(false), eventHandler(eventHandler)
{
    setSize(50, 50);       
}

void DragZone::setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer2, ModulatorsSubsection& parentComponent2)
{
    parentContainer = &parentContainer2;
    parentComponent = &parentComponent2;
}

void DragZone::resized()
{
}

// Icono de modulacion: tres nodos conectados
static void drawModulationIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour colour)
{
    const float r = area.getWidth() * 0.16f;
    const juce::Point<float> nodes[] = { { area.getX() + r, area.getBottom() - r },
                                         { area.getCentreX(), area.getY() + r },
                                         { area.getRight() - r, area.getBottom() - r } };

    g.setColour(colour);
    g.drawLine({ nodes[0], nodes[1] }, 1.6f);
    g.drawLine({ nodes[1], nodes[2] }, 1.6f);

    for (auto node : nodes) {
        g.setColour(Palette::Subsection);
        g.fillEllipse(juce::Rectangle<float>(r * 2.0f, r * 2.0f).withCentre(node));
        g.setColour(colour);
        g.drawEllipse(juce::Rectangle<float>(r * 2.0f, r * 2.0f).withCentre(node), 1.6f);
    }
}

void DragZone::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Franja lateral de la tarjeta (esquina inferior izquierda redondeada)
    juce::Path strip;
    strip.addRoundedRectangle(bounds.getX() + 1.0f, bounds.getY(), bounds.getWidth() - 1.0f, bounds.getHeight() - 1.0f,
                              7.0f, 7.0f, false, false, true, false);
    g.setColour(mouseHovering ? Palette::Modulation.withAlpha(0.10f) : Palette::Inset.withAlpha(0.45f));
    g.fillPath(strip);

    g.setColour(Palette::Border);
    g.drawVerticalLine(getWidth() - 1, 0.0f, bounds.getBottom());

    auto iconArea = bounds.withSizeKeepingCentre(20.0f, 20.0f);
    drawModulationIcon(g, iconArea, mouseHovering ? Palette::Modulation : Palette::TextDim);
}

void DragZone::mouseDown(const juce::MouseEvent& event)
{
    if (parentContainer != nullptr)
    {
        juce::var dragDescription = "LFO1"; // Descripción de lo que se está arrastrando

        // Imagen que sigue al cursor mientras se arrastra la modulacion
        juce::Image dragImage(juce::Image::ARGB, 40, 40, true);
        {
            juce::Graphics ig(dragImage);
            auto area = juce::Rectangle<float>(40.0f, 40.0f).reduced(2.0f);
            ig.setColour(Palette::Modulation.withAlpha(0.18f));
            ig.fillEllipse(area);
            ig.setColour(Palette::Modulation);
            ig.drawEllipse(area.reduced(1.0f), 2.0f);
            drawModulationIcon(ig, area.withSizeKeepingCentre(18.0f, 18.0f), Palette::Modulation);
        }

        if (dragImage.isValid()) // Verifica que la imagen se haya cargado correctamente
        {
            // Iniciar la operación de arrastre con la imagen cargada

            // Cambiar el cursor a una mano de arrastre
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);

            eventHandler.onDraggingModulationStarted(*parentComponent);
            parentContainer->startDragging(dragDescription, parentComponent, dragImage, true);
        }
        else
        {
            DBG("Error: No se pudo cargar la imagen desde el archivo.");
        }
    }
}

void DragZone::mouseUp(const juce::MouseEvent& event)
{
    // Restaurar el cursor al predeterminado después de soltar el mouse
    eventHandler.onDraggingModulationEnded();
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void DragZone::mouseEnter(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    mouseHovering = true;
    repaint();
}

void DragZone::mouseExit(const juce::MouseEvent& event)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
    mouseHovering = false;
    repaint();
}
