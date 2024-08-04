#pragma once

#include <JuceHeader.h>
#include <../Source/GUI_Subsections/Subsection.h>
#include <../Source/ParameterHandler/Parameter.h>
#include "../GUI_EventHandler.h"

class Knob1 : public juce::Slider, public juce::DragAndDropTarget, juce::Slider::Listener {
public:
	Knob1(ParameterInfo parameterInfo, GUI_EventHandler &eventHandler);

	void itemDropped(const SourceDetails& dragSourceDetails) override;
	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;

	void sliderValueChanged(Slider *slider) override;

	inline std::shared_ptr<Parameter2> getParameter() { return parameter; }

private:
	GUI_EventHandler& eventHandler;

	std::shared_ptr<Parameter2> parameter;

	double value = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob1)

};

