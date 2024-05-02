#pragma once
#include <JuceHeader.h>

class FullGui : juce::Component {
public:

	FullGui();

	//Aqui se ensamblaria la interfaz general por secciones (Osciladores, efectos, interfaz general...)
	juce::Component createGUI();


private:
};