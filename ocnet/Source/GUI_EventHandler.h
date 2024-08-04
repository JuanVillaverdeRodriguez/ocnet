/*
  ==============================================================================

    GUI_EventHandler.h
    Created: 4 Aug 2024 8:59:38pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once
#include "ParameterHandler/Parameter.h"

class GUI_EventHandler {
public:
    virtual void onAddEffect(int effectID) {};
    virtual void onAddOscillator(int option) {};
    virtual void onAddModulator(int option) {};

    virtual void onDeleteEffect(int processorID) {};
    virtual void onDeleteOscillator(int processorID) {};
    virtual void onDeleteModulator(int processorID) {};

    virtual void onConnectModulation(int processorModulatorID, std::shared_ptr<Parameter2> parameter) {};

private:

};