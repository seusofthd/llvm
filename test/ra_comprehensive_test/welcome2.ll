; ModuleID = 'welcome2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %test1 = alloca i32, align 4
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  %p = alloca i32, align 4
  %test2 = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %q = alloca i32, align 4
  %test3 = alloca i32, align 4
  %test4 = alloca i32, align 4
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  %test5 = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 1, i32* %x, align 4
  store i32 2, i32* %y, align 4
  %0 = load i32* %x, align 4
  %1 = load i32* %y, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* %z, align 4
  %2 = load i32* %z, align 4
  store i32 %2, i32* %test1, align 4
  store i32 4, i32* %m, align 4
  store i32 5, i32* %n, align 4
  %3 = load i32* %m, align 4
  %4 = load i32* %n, align 4
  %sub = sub nsw i32 %3, %4
  store i32 %sub, i32* %p, align 4
  %5 = load i32* %p, align 4
  store i32 %5, i32* %test2, align 4
  store i32 6, i32* %a, align 4
  store i32 7, i32* %b, align 4
  %6 = load i32* %a, align 4
  %7 = load i32* %b, align 4
  %mul = mul nsw i32 %6, %7
  store i32 %mul, i32* %q, align 4
  %8 = load i32* %q, align 4
  store i32 %8, i32* %test3, align 4
  %9 = load i32* %p, align 4
  %cmp = icmp sgt i32 %9, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 1, i32* %y, align 4
  br label %if.end4

if.else:                                          ; preds = %entry
  %10 = load i32* %p, align 4
  %cmp1 = icmp sgt i32 %10, 0
  br i1 %cmp1, label %if.then2, label %if.else3

if.then2:                                         ; preds = %if.else
  store i32 2, i32* %y, align 4
  br label %if.end

if.else3:                                         ; preds = %if.else
  store i32 3, i32* %y, align 4
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %11 = load i32* %y, align 4
  store i32 %11, i32* %test4, align 4
  store i32 0, i32* %k, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end4
  %12 = load i32* %i, align 4
  %cmp5 = icmp slt i32 %12, 100
  br i1 %cmp5, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %13 = load i32* %i, align 4
  %add6 = add nsw i32 %13, 1
  store i32 %add6, i32* %i, align 4
  %14 = load i32* %k, align 4
  %add7 = add nsw i32 %14, 1
  store i32 %add7, i32* %k, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %15 = load i32* %k, align 4
  store i32 %15, i32* %test5, align 4
  %16 = load i32* %test1, align 4
  %17 = load i32* %test2, align 4
  %add8 = add nsw i32 %16, %17
  %18 = load i32* %test3, align 4
  %add9 = add nsw i32 %add8, %18
  %19 = load i32* %test4, align 4
  %add10 = add nsw i32 %add9, %19
  %20 = load i32* %test5, align 4
  %add11 = add nsw i32 %add10, %20
  ret i32 %add11
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
