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
    auto area = getLocalBounds();

    volumeKnob->setBounds(0, 20, defaultKnobSize, defaultKnobSize);
    panningKnob->setBounds(defaultKnobSize, 20, defaultKnobSize, defaultKnobSize);

    waveformViewRectangleBounds.setBounds((defaultKnobSize * 2) + 10, 20, 240, 40);
    //search.setBounds(waveformViewRectangleBounds.getX() + 225 + (defaultKnobSize * 1.4), 20, defaultKnobSize * 1.4, area.getHeight()-20);
    search.setBounds(waveformViewRectangleBounds.getX() + (waveformViewRectangleBounds.getWidth() / 2) - (waveformViewRectangleBounds.getWidth() / 4), waveformViewRectangleBounds.getHeight() + 20, waveformViewRectangleBounds.getWidth()/2, 20);
    searchButtonNext.setBounds(waveformViewRectangleBounds.getX() + waveformViewRectangleBounds.getWidth() - 20, waveformViewRectangleBounds.getHeight() + 20, 20, 20);
    searchButtonPrevious.setBounds(waveformViewRectangleBounds.getX(), waveformViewRectangleBounds.getHeight() + 20, 20, 20);
    
    volumeKnob->showLabel(*this, *volumeKnob);
    panningKnob->showLabel(*this, *panningKnob);
}

void SamplerSubsection::updateWaveformView(juce::Array<float> samples)
{
    waveformSamples = samples;
    repaintWaveform = true;
    repaint();
}

void SamplerSubsection::paintCalled(juce::Graphics& g)
{
    if (repaintWaveform) {
        repaintWaveform = false;
        // Dibujar la forma de onda

        g.setColour(Palette::White); // Color de la forma de onda

        // Obtener el tamaño del componente
        auto width = waveformViewRectangleBounds.getWidth();
        auto height = waveformViewRectangleBounds.getHeight();

        if (waveformSamples.isEmpty())
            return;

        // Dibujar la forma de onda
        juce::Path waveformPath;
        float centerY = height / 2.0f;
        float scaleX = static_cast<float>(width) / static_cast<float>(waveformSamples.size());
        float scaleY = centerY; // Escala vertical para ajustar la amplitud

        waveformPath.startNewSubPath(waveformViewRectangleBounds.getX(), waveformViewRectangleBounds.getY() + centerY); // Empieza en el centro del componente

        for (int i = 0; i < waveformSamples.size(); i+=4)
        {
            float x = i * scaleX;
            float y = centerY - (waveformSamples[i] * scaleY); // Invertimos la Y para que la onda suba y baje
            waveformPath.lineTo(waveformViewRectangleBounds.getX() + x, waveformViewRectangleBounds.getY() + y);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(1.0f)); // Dibujar la línea de la forma de onda
    }
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
