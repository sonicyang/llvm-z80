//===- Z80Subtarget.cpp - Z80 Subtarget Information ---------------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Z80 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "Z80Subtarget.h"
#include "Z80.h"
#include "Z80TargetMachine.h"
#include "Z80SelectionDAGInfo.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR

#define DEBUG_TYPE "z80-subtarget"
#include "Z80GenSubtargetInfo.inc"

using namespace llvm;

Z80Subtarget::Z80Subtarget(const std::string &TT, const std::string &CPU, const std::string &FS,
                           const Z80TargetMachine * tm)
  : Z80GenSubtargetInfo(TT, CPU, FS),
    DL("e-p:16:8:8-i8:8:8-i16:8:8-n8:16"),
    TM(tm),
    TSInfo(DL)
{}

const DataLayout *Z80Subtarget::getDataLayout() const
{
    return &(this->DL);
}


const TargetRegisterInfo *Z80Subtarget::getRegisterInfo() const
{
    return this->TM->getRegisterInfo();
}

const TargetInstrInfo *Z80Subtarget::getInstrInfo() const
{
    return this->TM->getInstrInfo();
}

const TargetFrameLowering *Z80Subtarget::getFrameLowering() const
{
    return this->TM->getFrameLowering();
}

const TargetLowering *Z80Subtarget::getTargetLowering() const
{
    return this->TM->getTargetLowering();
}

const TargetSelectionDAGInfo *Z80Subtarget::getSelectionDAGInfo() const
{
    return &(this->TSInfo);
}
