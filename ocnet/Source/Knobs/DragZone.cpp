/*
  ==============================================================================

    DragZone.cpp
    Created: 20 Jul 2024 7:23:19pm
    Author:  TecNo

  ==============================================================================
*/

#include "DragZone.h"

DragZone::DragZone()
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
    g.fillAll(juce::Colours::darkcyan);
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds(), 2);
}

void DragZone::mouseDown(const juce::MouseEvent& event)
{
    if (parentContainer != nullptr)
    {
        juce::var dragDescription = "LFO1"; // Descripción de lo que se está arrastrando

        juce::Image dragImage = juce::PNGImageFormat::loadFrom(BinaryData::Arrows_png, BinaryData::Arrows_pngSize);
        
        dragImage = dragImage.rescaled(64, 64);

        if (dragImage.isValid()) // Verifica que la imagen se haya cargado correctamente
        {
            // Iniciar la operación de arrastre con la imagen cargada

            // Cambiar el cursor a una mano de arrastre
            setMouseCursor(juce::MouseCursor::DraggingHandCursor);

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
    setMouseCursor(juce::MouseCursor::NormalCursor);
}