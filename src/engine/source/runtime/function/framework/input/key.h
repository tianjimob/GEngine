#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "core/reflection/reflection.h"
#include "function/framework/object/object.h"
#include "function/framework/ui/input.h"

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
  Key(const Key &key)
      : m_keyName(key.m_keyName), m_keyDetails(key.m_keyDetails) {}

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

  static const Key SpaceBar;
  static const Key Shift;
  static const Key Ctrl;
  static const Key Alt;
  static const Key Cmd;

  static void init();

  static std::shared_ptr<KeyDetails> &getKeyDetails(Key &key) {
    return m_keyToDetails[key];
  }

  static const Key &getKeyFromVirtualCode(VirtualCode code) {
    static std::unordered_map<VirtualCode, const Key *> codeToKey{
        {VirtualCode::a, &A},
        {VirtualCode::b, &B},
        {VirtualCode::c, &C},
        {VirtualCode::d, &D},
        {VirtualCode::e, &E},
        {VirtualCode::f, &F},
        {VirtualCode::g, &G},
        {VirtualCode::h, &H},
        {VirtualCode::i, &I},
        {VirtualCode::j, &J},
        {VirtualCode::k, &K},
        {VirtualCode::l, &L},
        {VirtualCode::m, &M},
        {VirtualCode::n, &N},
        {VirtualCode::o, &O},
        {VirtualCode::p, &P},
        {VirtualCode::q, &Q},
        {VirtualCode::r, &R},
        {VirtualCode::s, &S},
        {VirtualCode::t, &T},
        {VirtualCode::u, &U},
        {VirtualCode::v, &V},
        {VirtualCode::w, &W},
        {VirtualCode::x, &X},
        {VirtualCode::y, &Y},
        {VirtualCode::z, &Z},
        {VirtualCode::SPACE, &SpaceBar},
        {VirtualCode::LSHIFT, &Shift},
        {VirtualCode::RSHIFT, &Shift},
        {VirtualCode::LCTRL, &Ctrl},
        {VirtualCode::RCTRL, &Ctrl},
        {VirtualCode::LALT, &Alt},
        {VirtualCode::RALT, &Alt},
        {VirtualCode::LGUI, &Cmd},
        {VirtualCode::RGUI, &Cmd},
    };

    return *codeToKey[code];
  }

  static const Key &getKeyFromModifierKey(ModifierKey code) {
    static std::unordered_map<ModifierKey, const Key *> modifierToKey{
        {ModifierKey::LeftShift, &Shift}, {ModifierKey::RightShift, &Shift},
        {ModifierKey::LeftCtrl, &Ctrl},   {ModifierKey::RightCtrl, &Ctrl},
        {ModifierKey::LeftAlt, &Alt},     {ModifierKey::RightAlt, &Alt},
        {ModifierKey::LeftGui, &Cmd},     {ModifierKey::RightGui, &Cmd},
    };

    return *modifierToKey[code];
  }

 private:
  static std::map<Key, std::shared_ptr<KeyDetails>> m_keyToDetails;
  static void addKeyWithDetails(const Key &key) {
    m_keyToDetails[key] = std::make_shared<KeyDetails>();
    const_cast<Key &>(key).m_keyDetails = m_keyToDetails[key];
  }
};

}  // namespace GEngine