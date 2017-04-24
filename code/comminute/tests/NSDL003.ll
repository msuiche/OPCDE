; ModuleID = 'NSDL003.bc'
source_filename = "NSDL003.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.addrinfo = type { i32, i32, i32, i32, i32, %struct.sockaddr*, i8*, %struct.addrinfo* }
%struct.sockaddr = type { i16, [14 x i8] }

@.str = private unnamed_addr constant [19 x i8] c"www.cw-complex.com\00", align 1
@.str.1 = private unnamed_addr constant [15 x i8] c"enter passwd: \00", align 1
@.str.2 = private unnamed_addr constant [5 x i8] c"http\00", align 1

; Function Attrs: nounwind uwtable
define void @leaks_passwd(i32) #0 {
  %2 = alloca %struct.addrinfo, align 8
  %3 = alloca %struct.addrinfo*, align 8
  %4 = icmp ne i32 %0, 0
  br i1 %4, label %5, label %7

; <label>:5:                                      ; preds = %1
  %6 = call i8* @getpass(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.1, i32 0, i32 0))
  br label %8

; <label>:7:                                      ; preds = %1
  br label %8

; <label>:8:                                      ; preds = %7, %5
  %.0 = phi i8* [ %6, %5 ], [ getelementptr inbounds ([19 x i8], [19 x i8]* @.str, i32 0, i32 0), %7 ]
  %9 = bitcast %struct.addrinfo* %2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %9, i8 0, i64 48, i32 8, i1 false)
  %10 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 1
  store i32 0, i32* %10, align 4
  %11 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 2
  store i32 2, i32* %11, align 8
  %12 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 0
  store i32 0, i32* %12, align 8
  %13 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 3
  store i32 0, i32* %13, align 4
  %14 = call i32 @getaddrinfo(i8* %.0, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.2, i32 0, i32 0), %struct.addrinfo* %2, %struct.addrinfo** %3)
  ret void
}

declare i8* @getpass(i8*) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #2

declare i32 @getaddrinfo(i8*, i8*, %struct.addrinfo*, %struct.addrinfo**) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32, i8**) #0 {
  %3 = call i64 @random() #4
  %4 = trunc i64 %3 to i32
  call void @leaks_passwd(i32 %4)
  ret i32 0
}

; Function Attrs: nounwind
declare i64 @random() #3

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.9.0-1ubuntu1 (tags/RELEASE_390/final)"}
