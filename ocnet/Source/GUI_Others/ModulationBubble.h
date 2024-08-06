#pragma once

#include "JuceHeader.h"

class ModulationBubble : public juce::Component
{
public:
    ModulationBubble()
    {
        setAlwaysOnTop(true); // Asegura que el componente siempre est� encima

        setSize(initialComponentWidth, initialComponentHeight); // Tama�o inicial del componente

        // Configuraci�n del label
        addAndMakeVisible(textLabel);
        addAndMakeVisible(modulationLabel);

        textLabel.setText("env 1", juce::dontSendNotification);
        textLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        textLabel.setJustificationType(juce::Justification::centred);
        textLabel.setVisible(true); // Hacer visible el label por defecto

        modulationLabel.setText(juce::String(0.0), juce::dontSendNotification);
        modulationLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        modulationLabel.setJustificationType(juce::Justification::centred);
        modulationLabel.setVisible(true); // Hacer visible el label por defecto
        modulationLabel.setAlwaysOnTop(true); // Asegura que el label siempre est� encima
    }

    void setModulationAmount(float amount)
    {
        modulationAmount = amount;
        modulationLabel.setText(juce::String(modulationAmount, 2), juce::dontSendNotification);
        repaint();
    }

    float getModulationAmount() const { return modulationAmount; }

    void paint(juce::Graphics& g) override
    {
        // Dibujar la burbuja
        g.setColour(juce::Colours::blue.withAlpha(0.5f));
        g.fillEllipse(getBubbleArea().toFloat());

        // Dibujar el texto de la cantidad de modulaci�n
        g.setColour(juce::Colours::white);
        g.drawText(textLabel.getText(), getBubbleArea(), juce::Justification::centred, false);
    }

    void paintOverChildren(juce::Graphics& g) override
    {
        if (isMouseOver)
        {
            // Dibuja la burbuja cuando el rat�n est� sobre ella
            g.setColour(juce::Colours::blue);
            g.fillEllipse(getBubbleArea().toFloat());
            g.setColour(juce::Colours::white);
            g.drawText(textLabel.getText(), getBubbleArea(), juce::Justification::centred, false);
        }
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        auto labelHeight = 20;

        // Ajustar la posici�n del label debajo de la burbuja
        auto bubbleArea = getBubbleArea();
        modulationLabel.setBounds(bounds.withTop(bubbleArea.getBottom()).withHeight(labelHeight));
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        float newAmount = juce::jlimit(-1.0f, 1.0f, modulationAmount + event.getDistanceFromDragStartY() * -0.01f);
        setModulationAmount(newAmount);

        if (onAmountChange)
            onAmountChange(modulationAmount);
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        // Ocultar el cursor al hacer clic en la burbuja
        setMouseCursor(juce::MouseCursor::NoCursor);
        // Almacenar la posici�n inicial del rat�n en coordenadas globales
        initialMousePos = event.getEventRelativeTo(this).getScreenPosition();
    }

    void mouseUp(const juce::MouseEvent& event) override
    {
        // Restaurar el cursor al soltar el clic
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
        // Restablecer el cursor a la posici�n inicial guardada
        juce::Desktop::getInstance().setMousePosition(initialMousePos);
    }

    void mouseEnter(const juce::MouseEvent& event) override
    {
        // Mover el componente al frente y mostrar el label al entrar el rat�n
        toFront(true);
        isMouseOver = true;
        modulationLabel.setVisible(true);
        repaint();
    }

    void mouseExit(const juce::MouseEvent& event) override
    {
        // Ocultar el label al salir el rat�n
        isMouseOver = false;
        modulationLabel.setVisible(false);
        repaint();
    }

    std::function<void(float)> onAmountChange;

private:
    float modulationAmount = 0.0f;
    juce::Point<int> initialMousePos; // Posici�n inicial del rat�n
    const int initialComponentWidth = 50; // Ancho inicial del componente
    const int initialComponentHeight = 70; // Altura inicial del componente
    const int bubbleSize = 15; // Tama�o de la burbuja
    const int expandedBubbleSize = 30; // Tama�o expandido de la burbuja
    juce::Label textLabel; // Label para mostrar el texto
    juce::Label modulationLabel; // Label para mostrar el texto

    bool isMouseOver = false; // Indica si el rat�n est� sobre la burbuja

    // Calcula el �rea donde se dibuja la burbuja
    juce::Rectangle<int> getBubbleArea() const
    {
        int size = isMouseOver ? expandedBubbleSize : bubbleSize;
        return getLocalBounds().withSizeKeepingCentre(size, size);
    }
};
