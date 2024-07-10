/*
  ==============================================================================

    ParameterHandler.cpp
    Created: 9 Jul 2024 9:56:15pm
    Author:  TecNo

  ==============================================================================
*/

#include "ParameterHandler.h"

float ParameterHandler::getParameterValue(int parameterOwnerType, int parameterTag, int ownerID)
{
    if (parameterOwnerType == ocnet::ParameterOwnerType::Envelope) {
        if (ownerID < 0 || ownerID >= envelopeAttachments.size()) {
            DBG("Error: ownerID fuera de rango.");
            return -1.0f; 
        }
        return envelopeAttachments[ownerID]->getParameterValue(parameterTag);
    }
    return -1.0f; 
}


void ParameterHandler::attachParameter(std::unique_ptr<EnvelopeAttachment> attachment)
{
    envelopeAttachments.push_back(std::move(attachment));
}
