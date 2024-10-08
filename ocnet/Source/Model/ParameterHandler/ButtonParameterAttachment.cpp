/*
  ==============================================================================

    ButtonParameterAttachment.cpp
    Created: 20 Aug 2024 2:02:41pm
    Author:  TecNo

  ==============================================================================
*/

#include "ButtonParameterAttachment.h"

ButtonParameterAttachment::ButtonParameterAttachment(juce::Button& button, ButtonParameter& parameter) : button(button), parameter(parameter)
{
    button.addListener(this);
    button.setToggleState(parameter.getState(), juce::NotificationType::sendNotificationSync);
}

void ButtonParameterAttachment::buttonClicked(juce::Button* buttonClicked)
{
    if (&button == buttonClicked) {
        parameter.setState(button.getToggleState());
    }
}
