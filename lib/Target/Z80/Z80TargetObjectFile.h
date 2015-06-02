//===-- Z80TargetObjectFile.h - Z80 Object Info -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Z80_Z80TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_Z80_Z80TARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {

class MCContext;
class TargetMachine;

class Z80ELFTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  Z80ELFTargetObjectFile() :
    TargetLoweringObjectFileELF()
  {}
};

} // end namespace llvm

#endif
