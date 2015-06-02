//==-- Z80TargetMachine.h - Define TargetMachine for Z80 ---------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Z80 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef Z80TARGETMACHINE_H
#define Z80TARGETMACHINE_H

#include "Z80.h"
#include "Z80FrameLowering.h"
#include "Z80ISelLowering.h"
#include "Z80InstrInfo.h"
#include "Z80SelectionDAGInfo.h"
#include "Z80Subtarget.h"
#include "llvm/IR/DataLayout.h"

namespace llvm {
  class Z80TargetMachine : public LLVMTargetMachine {
    std::unique_ptr<TargetLoweringObjectFile> TLOF;
    Z80FrameLowering FrameLowering;
    Z80InstrInfo InstrInfo;
    Z80Subtarget Subtarget;
    Z80TargetLowering TLInfo;
  public:
    Z80TargetMachine(const Target &T, StringRef TT, StringRef CPU,
      StringRef FS, const TargetOptions &Options, Reloc::Model RM,
      CodeModel::Model CM, CodeGenOpt::Level OL);
    ~Z80TargetMachine() override;
    virtual const DataLayout *getDataLayout() const { return this->getSubtargetImpl()->getDataLayout(); }
    virtual const Z80FrameLowering *getFrameLowering() const {
      return &FrameLowering;
    }
    virtual const Z80InstrInfo *getInstrInfo() const { return &InstrInfo; }
    virtual const Z80RegisterInfo *getRegisterInfo() const {
      return &getInstrInfo()->getRegisterInfo();
    }
    virtual const Z80SelectionDAGInfo *getSelectionDAGInfo() const {
      return (Z80SelectionDAGInfo*)(this->getSubtargetImpl()->getSelectionDAGInfo());
    }
    virtual const TargetSubtargetInfo *getSubtargetImpl() const override { return &Subtarget; }
    virtual const Z80TargetLowering *getTargetLowering() const {
      return &TLInfo;
    }
    virtual TargetLoweringObjectFile *getObjFileLowering() const override {
      return TLOF.get();
    }
    virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  }; // end class Z80TargetMachine
} // end namespace llvm

#endif
