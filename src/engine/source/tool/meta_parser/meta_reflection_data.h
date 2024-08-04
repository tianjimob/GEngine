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
  MetaReflectionData(const std::string &headname);
  MetaReflectionData() {}
  void addEnum(const Enum &e);
  void addClass(const Class &c);

  // called after addXXX()
  // setup inheritance chain for any class
  void process();
  // called after process
  inja::json *findRegisterByName(const std::string &registerName) {
    return registers[registerName];
  }
  void addRegister(inja::json *json) {
    m_data["meta_registers"].emplace_back(*json);
  }

  const inja::json &getData() const;
  bool isEmpty() const;
  std::vector<std::string> getRegisterNamesList() const;


private:
  void registerEnum(const Enum &e);
  void registerClass(const Class &c);

  inja::json m_data;
  std::unordered_map<std::string, inja::json *> registers;
};