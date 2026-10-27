/*
  ==============================================================================

    SamplerSubsection.cpp
    Created: 26 Aug 2024 5:13:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "SamplerSubsection.h"

SamplerSubsection::SamplerSubsection(int id, GUI_EventHandler& eventHandler) : OscillatorsSubsection(eventHandler, id, "Sampler"), eventHandler(eventHandler), waveformSamples(), repaintWaveform(false)
{
    filenameParameterID = createParameterID("filename");
    volumeParameterID = createParameterID("volume");
    panningParameterID = createParameterID("panning");

    volumeKnob = std::make_unique<Knob1>(volumeParameterID, eventHandler, "Level");
    panningKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Pan");

    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);
    this->addAndMakeVisible(search);
    this->addAndMakeVisible(searchButtonPrevious);
    this->addAndMakeVisible(searchButtonNext);


    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

    search.setLookAndFeel(&lookAndFeel);
    searchButtonPrevious.setLookAndFeel(&lookAndFeel);
    searchButtonNext.setLookAndFeel(&lookAndFeel);

    setDesiredHeight(142);

    searchButtonPrevious.setButtonText("<");
    searchButtonNext.setButtonText(">");
    
    search.addListener(this);
    searchButtonPrevious.addListener(this);
    searchButtonNext.addListener(this);

    loadAudioFiles();

}

SamplerSubsection::~SamplerSubsection()
{
}

void SamplerSubsection::attachParameters(ParameterHandler& parameterHandler)
{
    volumeParameterAttachment = std::make_unique<OcnetSliderAttachment>(*volumeKnob, *parameterHandler.getSliderParameter(volumeParameterID)->get());
    panningParameterAttachment = std::make_unique<OcnetSliderAttachment>(*panningKnob, *parameterHandler.getSliderParameter(panningParameterID)->get());
    search.setButtonText(parameterHandler.getParameterValue(filenameParameterID).toString());
}

void SamplerSubsection::addParametersToParameterHandler(ParameterHandler& parameterHandler)
{
    parameterHandler.addSliderParameter(volumeParameterID, std::make_shared<SliderParameter>("volume", 0.75f));
    parameterHandler.addSliderParameter(panningParameterID, std::make_shared<SliderParameter>("panning", 0.5f));
    parameterHandler.addParameter(filenameParameterID, juce::String("Select a sample."));
}

void SamplerSubsection::subsectionResized()
{
    auto content = getContentArea();
    const int x0 = content.getX();
    const int y0 = content.getY();
    const int waveformHeight = 64;
    const int knobY = y0 + (content.getHeight() - knobCellHeight) / 2;

    placeKnob(*volumeKnob, x0, knobY);
    placeKnob(*panningKnob, x0 + knobWidth, knobY);

    const int waveformX = x0 + knobWidth * 2 + 20;
    waveformViewRectangleBounds = juce::Rectangle<float>((float)waveformX, (float)y0,
                                                         (float)juce::jmin(360, content.getRight() - waveformX), (float)waveformHeight);

    auto selectorRow = juce::Rectangle<int>(waveformX, y0 + waveformHeight + 6, (int)waveformViewRectangleBounds.getWidth(), 24);
    searchButtonPrevious.setBounds(selectorRow.removeFromLeft(24));
    searchButtonNext.setBounds(selectorRow.removeFromRight(24));
    search.setBounds(selectorRow.reduced(4, 0));
}

void SamplerSubsection::updateWaveformView(juce::Array<float> samples)
{
    waveformSamples = samples;
    repaintWaveform = true;
    repaint();
}

void SamplerSubsection::paintCalled(juce::Graphics& g)
{
    repaintWaveform = false;

    Draw::graphBackground(g, waveformViewRectangleBounds, 8, 2);

    if (waveformSamples.isEmpty()) {
        g.setColour(Palette::TextFaint);
        g.setFont(Fonts::caps(10.0f, true, 0.12f));
        g.drawText("NO SAMPLE LOADED", waveformViewRectangleBounds, juce::Justification::centred);
        return;
    }

    // Forma de onda
    auto area = waveformViewRectangleBounds.reduced(4.0f, 6.0f);
    const float centerY = area.getCentreY();
    const float scaleX = area.getWidth() / static_cast<float>(waveformSamples.size());
    const float scaleY = area.getHeight() / 2.0f;

    juce::Path waveformPath;
    waveformPath.startNewSubPath(area.getX(), centerY);

    for (int i = 0; i < waveformSamples.size(); i += 4)
        waveformPath.lineTo(area.getX() + i * scaleX, centerY - waveformSamples[i] * scaleY);

    juce::Graphics::ScopedSaveState state(g);
    g.reduceClipRegion(waveformViewRectangleBounds.toNearestInt());
    g.setColour(getTypeColour());
    g.strokePath(waveformPath, juce::PathStrokeType(1.2f));
}

void SamplerSubsection::loadAudioFiles()
{
    juce::File directory(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Ocnet").getChildFile("Sampler"));

    if (!directory.isDirectory())
    {
        jassertfalse; // Asegura que el directorio sea válido para depuración
        return;
    }

    juce::Array<juce::File> audioFiles = directory.findChildFiles(
        juce::File::findFiles, false, "*.wav;*.mp3;*.aiff");

    int id = 1;
    for (auto& file : audioFiles)
    {
        searchMenu.addItem(id, file.getFileName());
        sampleFilenames.add(file.getFileName());
        id++;
    }

}

void SamplerSubsection::openMenu()
{
    searchMenu.setLookAndFeel(&getLookAndFeel());
    searchMenu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(search),
        [this](int result)
        {
            if (result <= 0)
                return;

            eventHandler.onSamplerSampleChanged(getId(), filenameParameterID, sampleFilenames[result - 1]);
            search.setButtonText(sampleFilenames[result - 1]);
        });
}

void SamplerSubsection::buttonClickedCalled(juce::Button* buttonClicked)
{
    if (&search == buttonClicked) {
        openMenu();
    }
}
