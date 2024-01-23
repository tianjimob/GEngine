#pragma once

#include <string>
#include <vector>
#pragma once

#include "class.h"
#include "enum.h"


#include <inja.hpp>

class MetaReflectionData {
public:
public:
  MetaReflectionData(const std::string& headname);
  void addEnum(const Enum &e);
  void addClass(const Class &c);

  const inja::json &getData() const;
  bool isEmpty() const;
  std::vector<std::string> getRegisterNamesList() const;

private:
  void registerEnum(const Enum &e);
  void registerClass(const Class &c);

  inja::json m_data;
};