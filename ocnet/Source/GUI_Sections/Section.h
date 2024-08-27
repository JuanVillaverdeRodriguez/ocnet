/*
  ==============================================================================

    Section.h
    Created: 5 Aug 2024 1:06:18am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include "../GUI_Subsections/Subsection.h"
#include <JuceHeader.h>

class Section : public juce::Component {
public:
    //virtual bool hasProcessorWithID(int id) = 0 {}
    //virtual bool move(int id, const juce::String& direction) = 0 {}
    //virtual bool deleteProcessor(int id) = 0 {}

    virtual std::vector<std::unique_ptr<Subsection>>* getListOfSubsections();
    void moveSubsection(int id, int positions);
    void deleteSubsection(int id);
    int sectionResized();

    Subsection* getSubsectionWithID(int id);

protected:

};