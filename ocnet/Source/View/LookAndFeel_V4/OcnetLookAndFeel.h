/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 8 Jul 2024 1:09:47am
    Author:  TecNo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Utils/OcnetTypes.h"

using namespace Ocnet;

// Base comun para todos los LookAndFeel de Ocnet: esquema de colores, menus,
// popups de valor y scrollbars.
class OcnetBaseLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OcnetBaseLookAndFeel() : juce::LookAndFeel_V4(createColourScheme())
    {
        setColour(juce::ResizableWindow::backgroundColourId, Palette::Background);

        setColour(juce::PopupMenu::backgroundColourId, Palette::BackgroundSecondary);
        setColour(juce::PopupMenu::textColourId, Palette::Text);
        setColour(juce::PopupMenu::headerTextColourId, Palette::TextDim);
        setColour(juce::PopupMenu::highlightedBackgroundColourId, Palette::Accent.withAlpha(0.22f));
        setColour(juce::PopupMenu::highlightedTextColourId, Palette::White);

        setColour(juce::BubbleComponent::backgroundColourId, Palette::Inset);
        setColour(juce::BubbleComponent::outlineColourId, Palette::BorderStrong);
        setColour(juce::TooltipWindow::backgroundColourId, Palette::Inset);
        setColour(juce::TooltipWindow::textColourId, Palette::Text);
        setColour(juce::TooltipWindow::outlineColourId, Palette::BorderStrong);

        setColour(juce::Label::textColourId, Palette::Text);
        setColour(juce::ScrollBar::thumbColourId, Palette::BorderStrong);

        setColour(juce::TextEditor::backgroundColourId, Palette::Inset);
        setColour(juce::TextEditor::textColourId, Palette::Text);
        setColour(juce::TextEditor::outlineColourId, Palette::BorderStrong);
        setColour(juce::TextEditor::focusedOutlineColourId, Palette::Accent);
        setColour(juce::TextEditor::highlightColourId, Palette::Accent.withAlpha(0.35f));
        setColour(juce::CaretComponent::caretColourId, Palette::Accent);

        setColour(juce::ComboBox::backgroundColourId, Palette::Inset);
        setColour(juce::ComboBox::textColourId, Palette::Text);
        setColour(juce::ComboBox::outlineColourId, Palette::Border);
        setColour(juce::ComboBox::arrowColourId, Palette::TextDim);

        setColour(juce::TextButton::buttonOnColourId, Palette::Accent);
    }

    static juce::LookAndFeel_V4::ColourScheme createColourScheme()
    {
        return { Palette::Background, Palette::BackgroundSecondary, Palette::BackgroundSecondary,
                 Palette::Border, Palette::Text, Palette::Accent,
                 Palette::White, Palette::Accent, Palette::Text };
    }

    //==============================================================================
    juce::Font getPopupMenuFont() override { return Fonts::ui(15.0f); }

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override
    {
        g.fillAll(Palette::BackgroundSecondary);
        g.setColour(Palette::BorderStrong);
        g.drawRect(0, 0, width, height);
    }

    juce::Font getSliderPopupFont(juce::Slider&) override { return Fonts::ui(13.0f, true); }
    int getSliderPopupPlacement(juce::Slider&) override { return juce::BubbleComponent::above; }

    void drawBubble(juce::Graphics& g, juce::BubbleComponent& bubble, const juce::Point<float>&,
                    const juce::Rectangle<float>& body) override
    {
        g.setColour(bubble.findColour(juce::BubbleComponent::backgroundColourId));
        g.fillRoundedRectangle(body, 5.0f);
        g.setColour(bubble.findColour(juce::BubbleComponent::outlineColourId));
        g.drawRoundedRectangle(body.reduced(0.5f), 5.0f, 1.0f);
    }

    int getDefaultScrollbarWidth() override { return 8; }

    void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height,
                       bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
                       bool isMouseOver, bool isMouseDown) override
    {
        juce::Rectangle<float> thumb;

        if (isScrollbarVertical)
            thumb = { (float)x + 2.0f, (float)thumbStartPosition, (float)width - 4.0f, (float)thumbSize };
        else
            thumb = { (float)thumbStartPosition, (float)y + 2.0f, (float)thumbSize, (float)height - 4.0f };

        auto colour = scrollbar.findColour(juce::ScrollBar::thumbColourId);
        g.setColour(isMouseOver || isMouseDown ? colour.brighter(0.3f) : colour);
        g.fillRoundedRectangle(thumb, juce::jmin(thumb.getWidth(), thumb.getHeight()) * 0.5f);
    }
};

// LookAndFeel de knobs, display de voces y botones conmutables (pestanas, legato, EQ...)
class OcnetLookAndFeel : public OcnetBaseLookAndFeel
{

public:
    static OcnetLookAndFeel& getInstance()
    {
        static OcnetLookAndFeel instance;
        return instance;
    }

    // Display numerico (numero de voces)
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle, juce::Slider& slider) override
    {
        // Se usa todo el componente (x, y, width, height solo cubren la pista del slider)
        auto bounds = slider.getLocalBounds().toFloat().reduced(0.5f);

        g.setColour(Palette::Inset);
        g.fillRoundedRectangle(bounds, 6.0f);
        g.setColour(slider.isMouseOverOrDragging() ? Palette::BorderStrong.brighter(0.2f) : Palette::Border);
        g.drawRoundedRectangle(bounds, 6.0f, 1.0f);

        auto content = bounds.reduced(10.0f, 0.0f);

        g.setColour(Palette::TextDim);
        g.setFont(Fonts::caps(10.0f));
        g.drawText("VOICES", content, juce::Justification::centredLeft);

        // Flechas arriba / abajo como pista de que se arrastra en vertical
        auto arrows = content.removeFromRight(8.0f).withSizeKeepingCentre(8.0f, 14.0f);
        juce::Path up, down;
        up.addTriangle(arrows.getX(), arrows.getY() + 5.0f, arrows.getRight(), arrows.getY() + 5.0f, arrows.getCentreX(), arrows.getY());
        down.addTriangle(arrows.getX(), arrows.getBottom() - 5.0f, arrows.getRight(), arrows.getBottom() - 5.0f, arrows.getCentreX(), arrows.getBottom());
        g.setColour(Palette::TextFaint);
        g.fillPath(up);
        g.fillPath(down);

        g.setColour(Palette::Text);
        g.setFont(Fonts::ui(20.0f, true));
        g.drawText(juce::String(static_cast<int>(slider.getValue())), content.withTrimmedRight(6.0f),
                   juce::Justification::centredRight);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        const auto bounds = juce::Rectangle<float>((float)x, (float)y, (float)width, (float)height);
        const float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 4.0f;
        const auto knob = bounds.withSizeKeepingCentre(size, size);
        const auto centre = knob.getCentre();
        const float radius = size * 0.5f;
        const float trackWidth = juce::jmax(2.5f, size * 0.075f);
        const float arcRadius = radius - trackWidth * 0.5f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        const bool hovering = slider.isMouseOverOrDragging() && slider.isEnabled();
        auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);
        if (hovering)
            fill = fill.brighter(0.25f);
        if (!slider.isEnabled())
            fill = fill.withSaturation(0.0f).withAlpha(0.5f);

        const juce::PathStrokeType stroke(trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

        // Pista del arco
        juce::Path track;
        track.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(Palette::SliderBackground);
        g.strokePath(track, stroke);

        // Arco del valor (con un halo suave)
        if (sliderPosProportional > 0.001f)
        {
            juce::Path value;
            value.addCentredArc(centre.x, centre.y, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
            g.setColour(fill.withAlpha(0.22f));
            g.strokePath(value, juce::PathStrokeType(trackWidth * 2.4f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.setColour(fill);
            g.strokePath(value, stroke);
        }

        // Cuerpo del knob
        const float bodyRadius = radius - trackWidth - 3.0f;
        const auto body = juce::Rectangle<float>(bodyRadius * 2.0f, bodyRadius * 2.0f).withCentre(centre);

        g.setColour(juce::Colours::black.withAlpha(0.35f));
        g.fillEllipse(body.translated(0.0f, 1.5f));

        g.setGradientFill(juce::ColourGradient(juce::Colour(0xff2c313d), centre.x, body.getY(),
                                               juce::Colour(0xff1a1d25), centre.x, body.getBottom(), false));
        g.fillEllipse(body);
        g.setColour(hovering ? Palette::BorderStrong.brighter(0.25f) : Palette::BorderStrong);
        g.drawEllipse(body.reduced(0.5f), 1.0f);

        // Indicador
        const auto inner = centre.getPointOnCircumference(bodyRadius * 0.25f, angle);
        const auto outer = centre.getPointOnCircumference(bodyRadius * 0.82f, angle);
        g.setColour(slider.isEnabled() ? Palette::White : Palette::TextFaint);
        g.drawLine({ inner, outer }, juce::jmax(1.8f, size * 0.05f));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
        const float radius = juce::jmin(6.0f, bounds.getHeight() * 0.5f);
        const bool on = button.getToggleState();
        const bool isTab = isSegmentedTab(button);
        const auto onColour = button.findColour(juce::TextButton::buttonOnColourId);

        if (on)
        {
            g.setColour(onColour.withAlpha(shouldDrawButtonAsHighlighted ? 0.26f : 0.18f));
            g.fillRoundedRectangle(bounds, radius);
            g.setColour(onColour.withAlpha(0.65f));
            g.drawRoundedRectangle(bounds, radius, 1.0f);
        }
        else if (isTab)
        {
            if (shouldDrawButtonAsHighlighted)
            {
                g.setColour(Palette::White.withAlpha(0.04f));
                g.fillRoundedRectangle(bounds, radius);
            }
        }
        else
        {
            g.setColour(shouldDrawButtonAsHighlighted ? Palette::BackgroundSecondary.brighter(0.08f) : Palette::BackgroundSecondary);
            g.fillRoundedRectangle(bounds, radius);
            g.setColour(Palette::BorderStrong);
            g.drawRoundedRectangle(bounds, radius, 1.0f);
        }

        if (shouldDrawButtonAsDown)
        {
            g.setColour(juce::Colours::black.withAlpha(0.15f));
            g.fillRoundedRectangle(bounds, radius);
        }
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const bool on = button.getToggleState();
        const auto onColour = button.findColour(juce::TextButton::buttonOnColourId);

        juce::Colour textColour = on ? onColour.interpolatedWith(Palette::White, 0.45f)
                                     : (shouldDrawButtonAsHighlighted ? Palette::Text : Palette::TextDim);

        g.setFont(Fonts::caps(isSegmentedTab(button) ? 13.0f : 11.0f, true, 0.1f));
        g.setColour(textColour);
        g.drawText(button.getButtonText().toUpperCase(), button.getLocalBounds(), juce::Justification::centred, false);
    }

    // Las pestanas (OSC / EFFECTS) no dibujan fondo cuando no estan activas
    static void markAsSegmentedTab(juce::Button& button) { button.getProperties().set("ocnetTab", true); }
    static bool isSegmentedTab(juce::Button& button) { return button.getProperties().contains("ocnetTab"); }

};
