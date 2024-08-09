#include "key.h"
#include <memory>

namespace GEngine {

std::map<Key, std::shared_ptr<KeyDetails>> Keys::m_keyToDetails;

void Key::postLoad(std::weak_ptr<GObject> parentObject) {
  m_keyDetails = Keys::getKeyDetails(*this);
}

const Key Keys::AnyKey("AnyKey");

const Key Keys::A("A");
const Key Keys::B("B");
const Key Keys::C("C");
const Key Keys::D("D");
const Key Keys::E("E");
const Key Keys::F("F");
const Key Keys::G("G");
const Key Keys::H("H");
const Key Keys::I("I");
const Key Keys::J("J");
const Key Keys::K("K");
const Key Keys::L("L");
const Key Keys::M("M");
const Key Keys::N("N");
const Key Keys::O("O");
const Key Keys::P("P");
const Key Keys::Q("Q");
const Key Keys::R("R");
const Key Keys::S("S");
const Key Keys::T("T");
const Key Keys::U("U");
const Key Keys::V("V");
const Key Keys::W("W");
const Key Keys::X("X");
const Key Keys::Y("Y");
const Key Keys::Z("Z");

const Key Keys::SpaceBar("SpaceBar");
const Key Keys::Shift("Shift");
const Key Keys::Ctrl("Ctrl");
const Key Keys::Alt("Alt");
const Key Keys::Cmd("Cmd");

void Keys::init() {
  addKeyWithDetails(AnyKey);
  m_keyToDetails[AnyKey]->isAnalog = false;

  addKeyWithDetails(A);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(B);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(C);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(D);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(E);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(F);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(G);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(H);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(I);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(J);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(K);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(L);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(M);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(N);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(O);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(P);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Q);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(R);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(S);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(T);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(U);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(V);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(W);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(X);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Y);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Z);
  m_keyToDetails[AnyKey]->isAnalog = false;

  addKeyWithDetails(SpaceBar);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Shift);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Ctrl);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Alt);
  m_keyToDetails[AnyKey]->isAnalog = false;
  addKeyWithDetails(Cmd);
  m_keyToDetails[AnyKey]->isAnalog = false;
}

}