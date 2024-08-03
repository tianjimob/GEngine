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

  addKeyWithDetails(A);
  addKeyWithDetails(B);
  addKeyWithDetails(C);
  addKeyWithDetails(D);
  addKeyWithDetails(E);
  addKeyWithDetails(F);
  addKeyWithDetails(G);
  addKeyWithDetails(H);
  addKeyWithDetails(I);
  addKeyWithDetails(J);
  addKeyWithDetails(K);
  addKeyWithDetails(L);
  addKeyWithDetails(M);
  addKeyWithDetails(N);
  addKeyWithDetails(O);
  addKeyWithDetails(P);
  addKeyWithDetails(Q);
  addKeyWithDetails(R);
  addKeyWithDetails(S);
  addKeyWithDetails(T);
  addKeyWithDetails(U);
  addKeyWithDetails(V);
  addKeyWithDetails(W);
  addKeyWithDetails(X);
  addKeyWithDetails(Y);
  addKeyWithDetails(Z);

  addKeyWithDetails(SpaceBar);
  addKeyWithDetails(Shift);
  addKeyWithDetails(Ctrl);
  addKeyWithDetails(Alt);
  addKeyWithDetails(Cmd);
}

}