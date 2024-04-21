#include "input_settings.h"

namespace GEngine {

void InputSettings::addAxisKeyMapping(
    const InputAxisKeyMapping &axisKeyMapping) {
  bool found = false;
  for (const auto &mapping : m_axisMappings) {
    if (axisKeyMapping == mapping) {
      found = true;
      break;
    }
  }
  if (!found) m_axisMappings.emplace_back(axisKeyMapping);
}

void InputSettings::addActionKeyMapping(
    const InputActionKeyMapping &actionKeyMapping) {
  bool found = false;
  for (const auto &mapping : m_actionMappings) {
    if (actionKeyMapping == mapping) {
      found = true;
      break;
    }
  }
  if (!found) m_actionMappings.emplace_back(actionKeyMapping);
}

void InputSettings::getAxisMappingByName(
    const std::string &axisName,
    std::vector<InputAxisKeyMapping> &outMappings) {
  for (auto &mapping : m_axisMappings) {
    if (mapping.axisName == axisName) {
      outMappings.emplace_back(mapping);
    }
  }
}

void InputSettings::getActionsMappingByName(
    const std::string &actionName,
    std::vector<InputActionKeyMapping> &outMappings) {
  for (auto &mapping : m_actionMappings) {
    if (mapping.actionName == actionName) {
      outMappings.emplace_back(mapping);
    }
  }
}

}  // namespace GEngine