#pragma once

#include <JuceHeader.h>
#include <../Source/GUI_Subsections/Subsection.h>
#include <../Source/ParameterHandler/Parameter.h>

class Knob1 : public juce::Slider, public juce::DragAndDropTarget, juce::Slider::Listener {
public:
	Knob1(ParameterInfo parameterInfo);

	class Listener {
	public:
		virtual ~Listener() { }
		virtual void connectModulation(int processorModulatorID, Parameter2& parameter) = 0;
	};

	void itemDropped(const SourceDetails& dragSourceDetails) override;
	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;

	void sliderValueChanged(Slider *slider) override;

	inline std::unique_ptr<Parameter2>& getParameter() { return parameter; }

	void inline setListener(Listener* listener) {
		listeners.push_back(listener);
	}

private:
	std::vector<Listener*> listeners;
	std::unique_ptr<Parameter2> parameter;

	double value = 0.0;
};

