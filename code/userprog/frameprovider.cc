// frameprovider.cc
//
#include "frameprovider.h"

FrameProvider::FrameProvider() {
  bm = new BitMap(10); //10? juste pour tester
}

FrameProvider::~FrameProvider() {
  delete bm;
}

int FrameProvider::GetEmptyFrame(){
  return 0;
}

bool FrameProvider::ReleaseFrame(int virtualAddr){
  return false;
}

int FrameProvider::NumAvailFrame(){
  return 0;
}
