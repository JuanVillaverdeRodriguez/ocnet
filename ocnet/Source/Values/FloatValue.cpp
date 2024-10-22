/*
  ==============================================================================

    Value.cpp
    Created: 21 Oct 2024 2:07:21pm
    Author:  TecNo

  ==============================================================================
*/

#include "FloatValue.h"

FloatValue::FloatValue(float initialValue) : previousValue(initialValue), currentValue(initialValue)
{
}

FloatValue::FloatValue() : previousValue(0.0f), currentValue(0.0f)
{
}

float FloatValue::getLinearInterpolatedValue(float factor)
{
    return previousValue + factor * (currentValue - previousValue);
}

float FloatValue::getValue()
{
    return currentValue;
}

void FloatValue::setValue(float newValue)
{
    previousValue = currentValue;
    currentValue = newValue;
}
