#pragma once

#include <map>
#include <memory>
#include <string>

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"


namespace GEngine {

struct KeyDetails {
  bool isAnalog;
};

class Keys;

CLASS(Key) : public GObject {
  REFLECTION_BODY(Key)

 public:
  Key() = default;
  Key(const std::string &keyName) : m_keyName(keyName) {}
  Key(const Key & key) : m_keyName(key.m_keyName), m_keyDetails(key.m_keyDetails) {}

  const std::string &getKeyName() { return m_keyName; }
  bool operator==(const Key &right) const {
    return m_keyName == right.m_keyName;
  }
  bool operator!=(const Key &right) const {
    return m_keyName != right.m_keyName;
  }
  bool operator<(const Key &right) const { return m_keyName < right.m_keyName; }

  virtual void postLoad(std::weak_ptr<GObject> parentObject) override;

  bool isAnalog() const { return m_keyDetails->isAnalog; }

 private:
  friend class Keys;

  META_FIELD()
  std::string m_keyName;

  std::shared_ptr<KeyDetails> m_keyDetails;
};

struct Keys {
  static const Key AnyKey;

  static const Key A;
  static const Key B;
  static const Key C;
  static const Key D;
  static const Key E;
  static const Key F;
  static const Key G;
  static const Key H;
  static const Key I;
  static const Key J;
  static const Key K;
  static const Key L;
  static const Key M;
  static const Key N;
  static const Key O;
  static const Key P;
  static const Key Q;
  static const Key R;
  static const Key S;
  static const Key T;
  static const Key U;
  static const Key V;
  static const Key W;
  static const Key X;
  static const Key Y;
  static const Key Z;

  static const Key Shift;
  static const Key Ctrl;
  static const Key Alt;
  static const Key Cmd;

  static void init();

  static std::shared_ptr<KeyDetails> &getKeyDetails(Key &key) {
    return m_keyToDetails[key];
  }

 private:
  static std::map<Key, std::shared_ptr<KeyDetails>> m_keyToDetails;
  static void addKeyWithDetails(const Key &key) {
    m_keyToDetails[key] = std::make_shared<KeyDetails>();
    const_cast<Key &>(key).m_keyDetails = m_keyToDetails[AnyKey];
  }
};

}  // namespace GEngine