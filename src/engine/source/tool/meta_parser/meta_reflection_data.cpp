#include "meta_reflection_data.h"
#include "class.h"
#include "enum.h"
#include "inja.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

MetaReflectionData::MetaReflectionData(const std::string& headname)
{
  m_data["headfile_name"] = headname;
}


void MetaReflectionData::addEnum(const Enum &e) { registerEnum(e); }

void MetaReflectionData::addClass(const Class &c) { registerClass(c); }

const inja::json &MetaReflectionData::getData() const { return m_data; }

bool MetaReflectionData::isEmpty() const
{
  return m_data.find("meta_registers") == m_data.end();
}

std::vector<std::string> MetaReflectionData::getRegisterNamesList() const {
  std::vector<std::string> ret;
  for (auto &metaRegister : m_data["meta_registers"]) {
    ret.emplace_back(metaRegister["meta_name"]);
  }
  return ret;
}

void MetaReflectionData::registerEnum(const Enum &e) {
  inja::json meta_register;
  auto namespacePos = e.getEnumName().rfind("::");
  auto nameWithoutNamespace = e.getEnumName().substr(namespacePos+2);
  meta_register["meta_name_without_namespace"] = nameWithoutNamespace;
  meta_register["meta_name"] = e.getEnumName();
  meta_register["type"] = "Enum";
  auto &entries = e.getConstantNamesList();
  meta_register["entry_names"] = inja::json::array();
  for (int i = 0; i < entries.size(); ++i) {
    meta_register["entry_names"][i] = entries[i];
  }
  m_data["meta_registers"].emplace_back(meta_register);
}

void MetaReflectionData::registerClass(const Class &c) {
  inja::json meta_register;
  auto namespacePos = c.getClassName().rfind("::");
  auto nameWithoutNamespace = c.getClassName().substr(namespacePos+2);
  meta_register["meta_name_without_namespace"] = nameWithoutNamespace;
  meta_register["meta_name"] = c.getClassName();
  meta_register["type"] = "Class";
  const auto &superClassName = c.getSuperClassName();
  meta_register["has_super_class"] = !superClassName.empty();
  if (!superClassName.empty())
    meta_register["super_class_name"] = superClassName;
  meta_register["is_abstract"] = c.isAbstract();
  meta_register["entries"] = inja::json::array();
  auto &fields = c.getFieldsList();
  auto &methods = c.getMethodsList();
  int i = 0;
  for (auto &field : fields) {
    auto &entry = meta_register["entries"][i++];
    switch (field.getArrayType()) {
    case Field::ArrayType::None:
      entry["type"] = "Field";
      break;
    case Field::ArrayType::CArray:
      entry["type"] = "CArrayField";
      entry["element_type_name"] = field.getElementTypeName();
      entry["size"] = field.getCArraySize();
      break;
    case Field::ArrayType::SizeArray:
      entry["type"] = "SizeArrayField";
      entry["element_type_name"] = field.getElementTypeName();
      break;
    default:
      break;
    }
    entry["field_name"] = field.getFieldName();
    entry["field_type_name"] = field.getFieldTypeName();
    entry["is_enum"] = field.isEnum();
  }
  for (auto &method : methods) {
    auto &entry = meta_register["entries"][i++];
    entry["type"] = "Method";
    entry["method_name"] = method.getMethodName();
  }
  m_data["meta_registers"].emplace_back(meta_register);
}

void MetaReflectionData::process() {
  auto &meta_registers = m_data["meta_registers"];
  for (auto &meta_register : meta_registers) {
    registers[meta_register["meta_name"].get<std::string>()] = &meta_register;
  }
  for (auto &meta_register : meta_registers) {
    if (meta_register["type"] != "Class")
      continue;
    
    inja::json chain = inja::json::array();

    inja::json* currRegister = &meta_register;
    while ((*currRegister)["has_super_class"].get<bool>()) {
      inja::json chainItem;
      auto it =
          registers.find((*currRegister)["super_class_name"].get<std::string>());
      if (it == registers.end())
        break;

      auto &superClassRegister = *it->second;
      chainItem["meta_name"] = (*currRegister)["super_class_name"].get<std::string>();
      chainItem["entries"] = superClassRegister["entries"];
      chain.emplace_back(chainItem);
      currRegister = &superClassRegister;
    }

    std::reverse(chain.begin(), chain.end());
    meta_register["chain"] = chain;
  }
}
