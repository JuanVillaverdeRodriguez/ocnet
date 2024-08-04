/*
  ==============================================================================

    gui_interface.cpp
    Created: 5 Jul 2024 8:10:36pm
    Author:  TecNo

  ==============================================================================
*/

#include "OcnetGUI_interface.h"

OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor) : processor(*processor)
{
    DBG("OcnetGUI_interface::OcnetGUI_interface(OcnetAudioProcessor *processor)");

    numberOfEnvelopes = 0;
    numberOfWavetableOscillators = 0;
    maxCurrentID = 0;
    gui_ = std::make_unique<OcnetGUI>(*this);
}

void OcnetGUI_interface::onAddOscillator(int option)
{
    DBG("OcnetGUI_interface::addOscillator(int option)");

    if (option > 0) {
        if (!processor.getHasEnvelope()) {
            onAddModulator(1);
        }

        if (option == 1) {
            gui_->getOscillatorsSection()->addWavetableOscillator(maxCurrentID, processor.parameterHandler);
            processor.addWavetableOscillator(maxCurrentID);
            maxCurrentID++;
        }
        else if (option == 2) {
            //processor.addSampler();
        }
        else if (option == 3) {
            //processor.addGranular();
        }
    }
}

void OcnetGUI_interface::onAddEffect(int option)
{
    if (option > 0) {
        if (option == 1) {
            gui_->getEffectsSection()->addDistortion(maxCurrentID, processor.parameterHandler);
            processor.addDistortion(maxCurrentID);
            maxCurrentID++;

        }
    }
}

void OcnetGUI_interface::onAddModulator(int option)
{
    if (option > 0) {
        if (option == 1) {
            gui_->getModulatorsSection()->addEnvelope(maxCurrentID, processor.parameterHandler);
            processor.addEnvelope(maxCurrentID);
            maxCurrentID++;
        }
    }
    DBG("OcnetGUI_interface::addModulator(int option)");
}

void OcnetGUI_interface::onConnectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter)
{
    processor.connectModulation(processorModulatorID, parameter);
}

void OcnetGUI_interface::onDeleteModulator(int processorID)
{
    gui_->getModulatorsSection()->deleteModulator(processorID);
    processor.deleteProcessor(processorID);
    processor.parameterHandler.deleteAttachedParameters(juce::String("Modulators"), juce::String(processorID));
}

void OcnetGUI_interface::onDeleteEffect(int processorID)
{
    gui_->getEffectsSection()->deleteEffect(processorID);
    processor.deleteProcessor(processorID);
    processor.parameterHandler.deleteAttachedParameters(juce::String("Effects"), juce::String(processorID));
}

void OcnetGUI_interface::onDeleteOscillator(int processorID)
{
    gui_->getOscillatorsSection()->deleteOscillator(processorID);
    processor.deleteProcessor(processorID);
    processor.parameterHandler.deleteAttachedParameters(juce::String("Oscillators"), juce::String(processorID));
}


OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}
