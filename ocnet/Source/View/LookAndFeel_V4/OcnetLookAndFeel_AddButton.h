#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

class OcnetLookAndFeel_AddButton : public juce::LookAndFeel_V4
{
public:
    static OcnetLookAndFeel_AddButton& getInstance()
    {
        static OcnetLookAndFeel_AddButton instance;
        return instance;
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        // Obtener los límites del botón
        auto bounds = button.getLocalBounds().toFloat();

        // Aplicar color de fondo
        g.setColour(Palette::BackgroundSecondary);

        // Dibujar un rectángulo sólido sin bordes
        g.fillRect(bounds);

        // Efecto visual para hover y clic
        if (isMouseOverButton)
        {
            g.setColour(Palette::BackgroundSecondary.brighter(0.1f));
            g.fillRect(bounds);
        }
        if (isButtonDown)
        {
            g.setColour(Palette::BackgroundSecondary.darker(0.1f));
            g.fillRect(bounds);
        }
    }
};