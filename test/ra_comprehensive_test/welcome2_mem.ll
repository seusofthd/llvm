; ModuleID = 'welcome2_mem.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %add = add nsw i32 1, 2
  %sub = sub nsw i32 4, 5
  %mul = mul nsw i32 6, 7
  %cmp = icmp sgt i32 %sub, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %if.end4

if.else:                                          ; preds = %entry
  %cmp1 = icmp sgt i32 %sub, 0
  br i1 %cmp1, label %if.then2, label %if.else3

if.then2:                                         ; preds = %if.else
  br label %if.end

if.else3:                                         ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  %y.0 = phi i32 [ 2, %if.then2 ], [ 3, %if.else3 ]
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %y.1 = phi i32 [ 1, %if.then ], [ %y.0, %if.end ]
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end4
  %k.0 = phi i32 [ 0, %if.end4 ], [ %add7, %while.body ]
  %i.0 = phi i32 [ 0, %if.end4 ], [ %add6, %while.body ]
  %cmp5 = icmp slt i32 %i.0, 100
  br i1 %cmp5, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %add6 = add nsw i32 %i.0, 1
  %add7 = add nsw i32 %k.0, 1
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %add8 = add nsw i32 %add, %sub
  %add9 = add nsw i32 %add8, %mul
  %add10 = add nsw i32 %add9, %y.1
  %add11 = add nsw i32 %add10, %k.0
  ret i32 %add11
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
