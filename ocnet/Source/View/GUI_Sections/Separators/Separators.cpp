/*
  ==============================================================================

    Separators.cpp
    Created: 13 Nov 2024 10:46:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "Separators.h"

Separators::Separators()
{
}

void Separators::resized()
{
    //auto bounds = getLocalBounds();

    //float half = bounds.getWidth() / 2;

    //centralRect.setBounds(half, 0, 3, bounds.getHeight());
}

void Separators::paint(juce::Graphics& g)
{
    /*auto area = getLocalBounds();
    auto headerSectionBounds = area.withTrimmedRight(area.getWidth() / 2.2).withTrimmedBottom(area.getHeight() - 50);

    area = getLocalBounds();

    g.setColour(Palette::Subsection);

    //const float centralDividerX = area.getWidth() / 2;
    g.fillRect(juce::Rectangle<float>(headerSectionBounds.getWidth(), 0, 5, area.getHeight()-headerSectionBounds.getHeight()));*/
}
