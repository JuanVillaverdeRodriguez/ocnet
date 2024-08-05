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
}

void OcnetGUI_interface::onConnectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter)
{
    processor.connectModulation(processorModulatorID, parameter);
}

void OcnetGUI_interface::onDeleteSubsection(Subsection& subsection)
{
    int id = subsection.getId();
    juce::String type = subsection.getType();

    gui_->getSection(subsection.getType())->deleteSubsection(id);
    processor.deleteProcessor(id);
    processor.parameterHandler.deleteAttachedParameters(type, juce::String(id));
}

void OcnetGUI_interface::onMoveSubsection(Subsection& subsection, int positions)
{
    gui_->getSection(subsection.getType())->moveSubsection(subsection.getId(), positions);
    processor.moveProcessor(subsection.getId(), positions);
}

OcnetGUI* OcnetGUI_interface::getGui() {
    return gui_.get(); 
}
