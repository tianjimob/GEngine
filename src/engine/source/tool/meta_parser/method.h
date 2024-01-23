#pragma once

#include "cursor.h"

class Method {
public:
  Method(const Cursor& cursor);

  const std::string getMethodName() const;
  
private:
  std::string m_methodName;
};