; RUN: llc < %s -mtriple=x86_64-apple-darwin | FileCheck %s

; Trivial patchpoint codegen
;
; FIXME: We should verify that the call target is materialize after
; the label immediately before the call.
; <rdar://15187295> [JS] llvm.webkit.patchpoint call target should be
; materialized in nop slide.
define i64 @trivial_patchpoint_codegen(i64 %p1, i64 %p2, i64 %p3, i64 %p4) {
entry:
; CHECK-LABEL: _trivial_patchpoint_codegen:
; CHECK:      Ltmp
; CHECK:      callq *%rax
; CHECK-NEXT: nop
; CHECK:      movq %rax, %[[REG:r.+]]
; CHECK:      callq *%rax
; CHECK-NEXT: nop
; CHECK:      movq %[[REG]], %rax
; CHECK:      ret
  %resolveCall2 = inttoptr i64 -559038736 to i8*
  %result = tail call i64 (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.i64(i32 2, i32 12, i8* %resolveCall2, i32 4, i64 %p1, i64 %p2, i64 %p3, i64 %p4)
  %resolveCall3 = inttoptr i64 -559038737 to i8*
  tail call void (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.void(i32 3, i32 12, i8* %resolveCall3, i32 2, i64 %p1, i64 %result)
  ret i64 %result
}

; Caller frame metadata with stackmaps. This should not be optimized
; as a leaf function.
;
; CHECK-LABEL: caller_meta_leaf
; CHECK: subq $24, %rsp
; CHECK: Ltmp
; CHECK: addq $24, %rsp
; CHECK: ret
define void @caller_meta_leaf() {
entry:
  %metadata = alloca i64, i32 3, align 8
  store i64 11, i64* %metadata
  store i64 12, i64* %metadata
  store i64 13, i64* %metadata
  call void (i32, i32, ...)* @llvm.experimental.stackmap(i32 4, i32 0, i64* %metadata)
  ret void
}

; Test the webkit_jscc calling convention.
; Two arguments will be pushed on the stack.
; Return value in $rax.
define void @jscall_patchpoint_codegen(i64 %p1, i64 %p2, i64 %p3, i64 %p4) {
entry:
; CHECK-LABEL: jscall_patchpoint_codegen:
; CHECK: Ltmp
; CHECK: movq %r{{.+}}, 8(%rsp)
; CHECK: movq %r{{.+}}, (%rsp)
; CHECK: movq $-559038736, %rax
; CHECK: Ltmp
; CHECK: callq *%rax
; CHECK: movq %rax, 8(%rsp)
; CHECK: callq
  %resolveCall2 = inttoptr i64 -559038736 to i8*
  %result = tail call webkit_jscc i64 (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.i64(i32 5, i32 12, i8* %resolveCall2, i32 2, i64 %p1, i64 %p2)
  %resolveCall3 = inttoptr i64 -559038737 to i8*
  tail call webkit_jscc void (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.void(i32 6, i32 12, i8* %resolveCall3, i32 2, i64 %p1, i64 %result)
  ret void
}

; Test patchpoints reusing the same TargetConstant.
; <rdar:15390785> Assertion failed: (CI.getNumArgOperands() >= NumArgs + 4)
; There is no way to verify this, since it depends on memory allocation.
; But I think it's useful to include as a working example.
define i64 @testLowerConstant(i64 %arg, i64 %tmp2, i64 %tmp10, i64* %tmp33, i64 %tmp79) {
entry:
  %tmp80 = add i64 %tmp79, -16
  %tmp81 = inttoptr i64 %tmp80 to i64*
  %tmp82 = load i64* %tmp81, align 8
  tail call void (i32, i32, ...)* @llvm.experimental.stackmap(i32 14, i32 5, i64 %arg, i64 %tmp2, i64 %tmp10, i64 %tmp82)
  tail call void (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.void(i32 15, i32 30, i8* null, i32 3, i64 %arg, i64 %tmp10, i64 %tmp82)
  %tmp83 = load i64* %tmp33, align 8
  %tmp84 = add i64 %tmp83, -24
  %tmp85 = inttoptr i64 %tmp84 to i64*
  %tmp86 = load i64* %tmp85, align 8
  tail call void (i32, i32, ...)* @llvm.experimental.stackmap(i32 17, i32 5, i64 %arg, i64 %tmp10, i64 %tmp86)
  tail call void (i32, i32, i8*, i32, ...)* @llvm.experimental.patchpoint.void(i32 18, i32 30, i8* null, i32 3, i64 %arg, i64 %tmp10, i64 %tmp86)
  ret i64 10
}

declare void @llvm.experimental.stackmap(i32, i32, ...)
declare void @llvm.experimental.patchpoint.void(i32, i32, i8*, i32, ...)
declare i64 @llvm.experimental.patchpoint.i64(i32, i32, i8*, i32, ...)
