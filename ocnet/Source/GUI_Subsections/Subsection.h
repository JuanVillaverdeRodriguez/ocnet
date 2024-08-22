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
#include "../ParameterHandler/ButtonParameterAttachment.h"

class Subsection : public juce::Component, juce::Button::Listener {
public:
    Subsection(GUI_EventHandler& eventHandler, int id, const juce::String& type, const juce::String& subType);
    virtual ~Subsection() = default;

    // Crea la ID de un parametro
    juce::String createParameterID(const juce::String& type, int id, const juce::String& parameterName);
    juce::String createParameterID(const juce::String& parameterName);


    int inline getId() const { return id; }
    juce::String inline getIdAsString() const { return juce::String(id); }

    void paint(juce::Graphics& g) override;
    void sectionResized();

    void setBypassed(bool bypassed);

    void buttonClicked(juce::Button* clickedButton) override;

    // Attachea los parametros con los knobs
    virtual void attachParams(ParameterHandler& parameterHandler) = 0 {};

    // Crea los parametros y los añade al parameterHandler
    virtual void addParamsToParameterHandler(ParameterHandler& parameterHandler) = 0 {};

    // Obtiene el tipo de la subseccion (Modulator, Oscillator o Effect)
    juce::String getType();

    // Obtiene el subtipo de la subseccion (Envelope, LFO, WavetableOscillator...)
    juce::String getSubType();

    // Para cosas que deban hacere una vez la subseccion se ha creado completamente
    void onPostInitialization();

private:
    int id;
    juce::String subType;
    juce::String type;
    bool bypassed;


protected:
    GUI_EventHandler& eventHandler;

    juce::TextButton removeButton;
    juce::TextButton moveUpButton;
    juce::TextButton moveDownButton;
    juce::TextButton bypassButton;
    juce::String bypassParameterID;
    std::unique_ptr<ButtonParameterAttachment> bypassButtonAttachment;

    juce::DrawableText subsectionName;

    int defaultKnobSize;


};