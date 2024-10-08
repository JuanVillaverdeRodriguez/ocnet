/*
  ==============================================================================

    Subsection.h
    Created: 8 Jul 2024 6:35:23pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Model/ParameterHandler/ParameterHandler.h"
#include "../../Controller/GUI_EventHandler.h"
#include "../../Model/ParameterHandler/ButtonParameterAttachment.h"
#include "../LookAndFeel_V4/OcnetLookAndFeel_SubsectionButtons.h"
#include "../../View/GUI_Elements/Knobs/Knob1.h"

using namespace Ocnet;

class Subsection : public juce::Component, public juce::Button::Listener {
public:
    Subsection(GUI_EventHandler& eventHandler, int id, const juce::String& type, const juce::String& subType);
    virtual ~Subsection() = default;

    // Crea la ID de un parametro
    juce::String createParameterID(const juce::String& type, int id, const juce::String& parameterName);
    juce::String createParameterID(const juce::String& parameterName);


    int inline getId() const { return id; }
    juce::String inline getIdAsString() const { return juce::String(id); }

    void paint(juce::Graphics& g) override;

    void setBypassed(bool bypassed);

    void buttonClicked(juce::Button* clickedButton) override;
    virtual void buttonClickedCalled(juce::Button* clickedButton) {};

    // Attachea los parametros con los knobs
    virtual void attachParams(ParameterHandler& parameterHandler) = 0 {};

    // Crea los parametros y los añade al parameterHandler
    virtual void addParamsToParameterHandler(ParameterHandler& parameterHandler) = 0 {};

    // Obtiene el tipo de la subseccion (Modulator, Oscillator o Effect)
    juce::String getType();

    // Obtiene el subtipo de la subseccion (Envelope, LFO, WavetableOscillator...)
    juce::String getSubType();

    juce::Array<Knob1*> getAllKnobs();
    // Para cosas que deban hacere una vez la subseccion se ha creado completamente
    void onPostInitialization();

    void setDesiredHeight(int newHeight);
    int getDesiredHeight();

    /*struct Graph : public juce::Component {
    public:
        void paint(juce::Graphics& g) override;

    private:
        float attack;
        float decay;
        float sustain;
        float release;
    };*/
private:
    int id;
    juce::String subType;
    juce::String type;
    bool bypassed;
    int desiredHeight;

protected:
    virtual void paintCalled(juce::Graphics& g) {};

    OcnetLookAndFeel_2 lookAndFeel;
    GUI_EventHandler& eventHandler;

    juce::TextButton removeButton;
    juce::TextButton moveUpButton;
    juce::TextButton moveDownButton;
    juce::TextButton bypassButton;
    juce::String bypassParameterID;
    std::unique_ptr<ButtonParameterAttachment> bypassButtonAttachment;

    juce::Label subsectionName;

    int defaultKnobSize;


};