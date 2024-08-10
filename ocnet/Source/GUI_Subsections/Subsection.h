/*
  ==============================================================================

    Subsection.h
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../ParameterHandler/ParameterHandler.h"
#include "../GUI_EventHandler.h"

class Subsection : public juce::Component, juce::Button::Listener {
public:
    Subsection(GUI_EventHandler& eventHandler);
    virtual ~Subsection() = default;

    // Crea la ID de un parametro
    juce::String createParameterID(const juce::String& type, int id, const juce::String& parameterName);
    juce::String createParameterID(const juce::String& parameterName);


    void inline setId(int id) { this->id = id; }
    int inline getId() const { return id; }
    juce::String inline getIdAsString() const { return juce::String(id); }

    void paint(juce::Graphics& g) override;
    void sectionResized();

    void setBypassed(bool bypassed);

    void buttonClicked(juce::Button* clickedButton) override;

    // Attachea los parametros con los knobs
    virtual void attachParams(ParameterHandler& parameterHandler) = 0 {};

    // Crea los parametros y los añade al parameterHandler
    virtual void addParametersToParameterHandler(ParameterHandler& parameterHandler) = 0 {};

    // Obtiene el tipo de la subseccion (Modulator, Oscillator o Effect)
    virtual juce::String getType() = 0 { return juce::String(""); };

    // Obtiene el subtipo de la subseccion (Envelope, LFO, WavetableOscillator...)
    virtual juce::String getSubType() = 0 { return juce::String(""); };

    // Utlizado al reconstruir la GUI para setear el valor de los parametros al guardado en el arbol
    virtual void setParameterValue(const juce::String& propertyName, const juce::String& propertyValue) = 0 {};

    // Para cosas que deban hacere una vez la subseccion se ha creado completamente
    void onPostInitialization();

private:
    int id;
    GUI_EventHandler& eventHandler;

    bool bypassed;


protected:
    juce::TextButton removeButton;
    juce::TextButton moveUpButton;
    juce::TextButton moveDownButton;
    juce::TextButton bypassButton;


    juce::DrawableText subsectionName;

    int defaultKnobSize;


};