/*
  ==============================================================================

    Value.h
    Created: 21 Oct 2024 2:07:21pm
    Author:  TecNo

  ==============================================================================
*/

#pragma once

// Valor flotante que incluye metodos de interpolacion (Actualmente solo lineal)
class FloatValue {
public:
    FloatValue(float initialValue);
    FloatValue();
    ~FloatValue() = default;

    // Devuelve la interpolación del valor actual con el valor anterior por el factor indicado.  
    float getLinearInterpolatedValue(float factor);

    // Obtiene el valor actual
    float getValue();

    // Actualiza el valor actual
    void setValue(float newValue);
private:
    float previousValue; // Valor anterior
    float currentValue; // Valor actual
};