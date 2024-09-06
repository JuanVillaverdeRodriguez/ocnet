#pragma once

#include <JuceHeader.h>
#include "../GUI_EventHandler.h"
#include "../LookAndFeel_V4/OcnetLookAndFeel.h"

using namespace Ocnet;

class Knob1 : public juce::Slider, public juce::DragAndDropTarget {
public:
	Knob1(const juce::String& parameterID, GUI_EventHandler &eventHandler);
	Knob1(const juce::String& parameterID, GUI_EventHandler& eventHandler, juce::String knobLabelText);
	~Knob1();

	void itemDropped(const SourceDetails& dragSourceDetails) override;
	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;

	juce::String getTextFromValue(double value) override;

	juce::String getParameterID();

	// Colocar la label en bounds
	void showLabel(juce::Component& component, juce::Rectangle<int> bounds);

	void paint(juce::Graphics& g) override;

	// Colocar la label debajo de attachToComp
	void showLabel(juce::Component& parentComp, juce::Component& attachToComp);
	void showModulationTarget(bool shouldShow);

private:
	OcnetLookAndFeel lookAndFeel;
	bool shouldShowModulationTarget = false;  // Bandera para mostrar el borde

	juce::Rectangle<int> modulationTargetBox;

	GUI_EventHandler& eventHandler;
	juce::String parameterID;
	juce::Label knobLabel;

	juce::String propertyName;

	double value = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob1)

};

