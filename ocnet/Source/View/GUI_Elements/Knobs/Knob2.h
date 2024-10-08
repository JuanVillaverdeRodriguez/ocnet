/*
  ==============================================================================

    Knob2.h
    Created: 22 Aug 2024 4:03:34pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../GUI_Subsections/Subsection.h"
#include "../../../Model/ParameterHandler/Parameter.h"
#include "../../../Controller/GUI_EventHandler.h"
#include "../../LookAndFeel_V4/OcnetLookAndFeel.h"

class Knob2 : public juce::Slider, public juce::DragAndDropTarget {
public:
	Knob2(const juce::String& parameterID, GUI_EventHandler& eventHandler);
	Knob2(const juce::String& parameterID, GUI_EventHandler& eventHandler, juce::String knobLabelText);
	~Knob2();

	void itemDropped(const SourceDetails& dragSourceDetails) override;
	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;

	juce::String getTextFromValue(double value) override;

	juce::String getParameterID();

	// Colocar la label en bounds
	void showLabel(juce::Component& component, juce::Rectangle<int> bounds);

	// Colocar la label debajo de attachToComp
	void showLabel(juce::Component& parentComp, juce::Component& attachToComp);

private:
	OcnetLookAndFeel lookAndFeel;

	GUI_EventHandler& eventHandler;
	juce::String parameterID;
	juce::Label knobLabel;

	juce::String propertyName;

	double value = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob2)

};