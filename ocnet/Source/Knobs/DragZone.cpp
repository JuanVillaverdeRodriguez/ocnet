/*
  ==============================================================================

    DragZone.cpp
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#include "DragZone.h"

DragZone::DragZone(GUI_EventHandler& eventHandler) : mouseHovering(false), eventHandler(eventHandler)
{
    setSize(50, 50);       
}

void DragZone::setParentContainerAndComponent(juce::DragAndDropContainer& parentContainer2, Subsection& parentComponent2)
{
    parentContainer = &parentContainer2;
    parentComponent = &parentComponent2;
}
/*
void DragZone::mouseDown(const juce::MouseEvent& event)
{
    if (parentContainer != nullptr)
    {
        juce::var dragDescription = "LFO1"; // Descripción de lo que se está arrastrando
        parentContainer->startDragging(dragDescription, parentComponent);
        //parentComponent->startDraggingUI();
    }
}*/

void DragZone::resized()
{
}

void DragZone::paint(juce::Graphics& g)
{
    // Llenar el fondo con el color especificado
    g.fillAll(Palette::BackgroundSecondary);

    juce::Image dragImage;
    if (!mouseHovering) {
        dragImage = juce::PNGImageFormat::loadFrom(BinaryData::AddAutomationIcon_NotHovering_png, BinaryData::AddAutomationIcon_NotHovering_pngSize);
    }
    else {
        dragImage = juce::PNGImageFormat::loadFrom(BinaryData::AddAutomationIcon_Hovering_png, BinaryData::AddAutomationIcon_Hovering_pngSize);
    }
    // Cargar la imagen

    // Escalar la imagen al tamaño deseado
    dragImage = dragImage.rescaled(32, 32);

    if (dragImage.isValid())
    {
        // Calcular la posición para centrar la imagen
        int x = (getWidth() - dragImage.getWidth()) / 2;
        int y = (getHeight() - dragImage.getHeight()) / 2;

        // Dibujar la imagen centrada
        g.drawImageAt(dragImage, x, y);
    }
}

void DragZone::mouseDown(const juce::MouseEvent& event)
{
    if (parentContainer != nullptr)
    {
        juce::var dragDescription = "LFO1"; // Descripción de lo que se está arrastrando

        juce::Image dragImage = juce::PNGImageFormat::loadFrom(BinaryData::AddAutomationIcon_Hovering_png, BinaryData::AddAutomationIcon_Hovering_pngSize);
        
        dragImage = dragImage.rescaled(64, 64);

        if (dragImage.isValid()) // Verifica que la imagen se haya cargado correctamente
        {
            // Iniciar la operación de arrastre con la imagen cargada

            // Cambiar el cursor a una mano de arrastre
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);

            eventHandler.onDraggingModulationStarted();
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
