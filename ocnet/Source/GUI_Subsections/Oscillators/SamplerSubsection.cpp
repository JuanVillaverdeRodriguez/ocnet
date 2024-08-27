/*
  ==============================================================================

    SamplerSubsection.cpp
    Created: 26 Aug 2024 5:13:13pm
    Author:  TecNo

  ==============================================================================
*/

#include "SamplerSubsection.h"

SamplerSubsection::SamplerSubsection(int id, GUI_EventHandler& eventHandler) : OscillatorsSubsection(eventHandler, id, "Sampler"), eventHandler(eventHandler)
{
    filenameParameterID = createParameterID("filename");
    volumeParameterID = createParameterID("volume");
    panningParameterID = createParameterID("panning");

    volumeKnob = std::make_unique<Knob1>(volumeParameterID, eventHandler, "Level");
    panningKnob = std::make_unique<Knob1>(panningParameterID, eventHandler, "Pan");

    this->addAndMakeVisible(*volumeKnob);
    this->addAndMakeVisible(*panningKnob);
    this->addAndMakeVisible(search);

    volumeKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;
    panningKnob->setRange(0.0f, 1.0f, 0.01f); // Quizas mejor seria volumeKnob.setRange(0, 1, 0.01f)?;

    search.addListener(this);
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

    auto area = getLocalBounds();

    volumeKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);
    panningKnob->setBounds(defaultKnobSize, 20, defaultKnobSize, defaultKnobSize);

    search.setBounds(defaultKnobSize*2, area.getHeight() - defaultKnobSize, defaultKnobSize * 2, defaultKnobSize - 10);

    volumeKnob->showLabel(*this, *volumeKnob);
    panningKnob->showLabel(*this, *panningKnob);
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
    searchMenu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(search),
        [this](int result)
        {
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
