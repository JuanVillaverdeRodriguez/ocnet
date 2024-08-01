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
    gui_ = std::make_unique<OcnetGUI>();
    //gui_->attachParams(processor->apvts);

    //gui_->getHeaderSection()->addListener(this);
    //gui_->getFooterSection()->addListener(this);
    gui_->getOscillatorsSection()->addListener(this);
    gui_->getModulatorsSection()->addListener(this);
    gui_->getEffectsSection()->addListener(this);
}

void OcnetGUI_interface::addOscillator(int option)
{
    DBG("OcnetGUI_interface::addOscillator(int option)");

    if (option > 0) {
        if (!processor.getHasEnvelope()) {
            addModulator(1);
        }

        if (juce::MessageManager::getInstance()->isThisTheMessageThread()) {
            DBG("addOscillator : IS THIS THE MESSAGE THREAD?: TRUE");
        }
        else {
            DBG("addOscillator : IS THIS THE MESSAGE THREAD?: FALSE");
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

void OcnetGUI_interface::addEffect(int option)
{
    if (option > 0) {
        if (option == 1) {
            gui_->getEffectsSection()->addDistortion(maxCurrentID, processor.parameterHandler);
            processor.addDistortion(maxCurrentID);
            maxCurrentID++;

        }
    }
}

void OcnetGUI_interface::addModulator(int option)
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

void OcnetGUI_interface::connectModulation(int processorModulatorID, Parameter2& parameter)
{
    processor.connectModulation(processorModulatorID, parameter);

}
