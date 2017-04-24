; ModuleID = 'NSDL002.bc'
source_filename = "NSDL002.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.addrinfo = type { i32, i32, i32, i32, i32, %struct.sockaddr*, i8*, %struct.addrinfo* }
%struct.sockaddr = type { i16, [14 x i8] }

@.str = private unnamed_addr constant [15 x i8] c"enter passwd: \00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"http\00", align 1

; Function Attrs: nounwind uwtable
define void @leaks_passwd(i32) #0 !dbg !18 {
  %2 = alloca %struct.addrinfo, align 8
  %3 = alloca %struct.addrinfo*, align 8
  call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !23, metadata !24), !dbg !25
  call void @llvm.dbg.declare(metadata %struct.addrinfo* %2, metadata !26, metadata !24), !dbg !58
  call void @llvm.dbg.declare(metadata %struct.addrinfo** %3, metadata !59, metadata !24), !dbg !60
  %4 = call i8* @getpass(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i32 0, i32 0)), !dbg !61
  call void @llvm.dbg.value(metadata i8* %4, i64 0, metadata !62, metadata !24), !dbg !63
  %5 = bitcast %struct.addrinfo* %2 to i8*, !dbg !64
  call void @llvm.memset.p0i8.i64(i8* %5, i8 0, i64 48, i32 8, i1 false), !dbg !64
  %6 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 1, !dbg !65
  store i32 0, i32* %6, align 4, !dbg !66
  %7 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 2, !dbg !67
  store i32 2, i32* %7, align 8, !dbg !68
  %8 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 0, !dbg !69
  store i32 0, i32* %8, align 8, !dbg !70
  %9 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 3, !dbg !71
  store i32 0, i32* %9, align 4, !dbg !72
  %10 = call i64 @strlen(i8* %4) #6, !dbg !73
  call void @llvm.memset.p0i8.i64(i8* %4, i8 0, i64 %10, i32 1, i1 false), !dbg !74
  %11 = call i32 @getaddrinfo(i8* %4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i32 0, i32 0), %struct.addrinfo* %2, %struct.addrinfo** %3), !dbg !76
  ret void, !dbg !77
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i8* @getpass(i8*) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #3

; Function Attrs: nounwind readonly
declare i64 @strlen(i8*) #4

declare i32 @getaddrinfo(i8*, i8*, %struct.addrinfo*, %struct.addrinfo**) #2

; Function Attrs: nounwind uwtable
define i32 @main(i32, i8**) #0 !dbg !78 {
  call void @llvm.dbg.value(metadata i32 %0, i64 0, metadata !82, metadata !24), !dbg !83
  call void @llvm.dbg.value(metadata i8** %1, i64 0, metadata !84, metadata !24), !dbg !85
  %3 = call i64 @random() #7, !dbg !86
  %4 = trunc i64 %3 to i32, !dbg !86
  call void @leaks_passwd(i32 %4), !dbg !87
  ret i32 0, !dbg !89
}

; Function Attrs: nounwind
declare i64 @random() #5

; Function Attrs: nounwind readnone
declare void @llvm.dbg.value(metadata, i64, metadata, metadata) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }
attributes #4 = { nounwind readonly "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nounwind readonly }
attributes #7 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!15, !16}
!llvm.ident = !{!17}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 3.9.0-1ubuntu1 (tags/RELEASE_390/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "NSDL002.c", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!2 = !{!3}
!3 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "__socket_type", file: !4, line: 24, size: 32, align: 32, elements: !5)
!4 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket_type.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!5 = !{!6, !7, !8, !9, !10, !11, !12, !13, !14}
!6 = !DIEnumerator(name: "SOCK_STREAM", value: 1)
!7 = !DIEnumerator(name: "SOCK_DGRAM", value: 2)
!8 = !DIEnumerator(name: "SOCK_RAW", value: 3)
!9 = !DIEnumerator(name: "SOCK_RDM", value: 4)
!10 = !DIEnumerator(name: "SOCK_SEQPACKET", value: 5)
!11 = !DIEnumerator(name: "SOCK_DCCP", value: 6)
!12 = !DIEnumerator(name: "SOCK_PACKET", value: 10)
!13 = !DIEnumerator(name: "SOCK_CLOEXEC", value: 524288)
!14 = !DIEnumerator(name: "SOCK_NONBLOCK", value: 2048)
!15 = !{i32 2, !"Dwarf Version", i32 4}
!16 = !{i32 2, !"Debug Info Version", i32 3}
!17 = !{!"clang version 3.9.0-1ubuntu1 (tags/RELEASE_390/final)"}
!18 = distinct !DISubprogram(name: "leaks_passwd", scope: !1, file: !1, line: 16, type: !19, isLocal: false, isDefinition: true, scopeLine: 17, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !22)
!19 = !DISubroutineType(types: !20)
!20 = !{null, !21}
!21 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!22 = !{}
!23 = !DILocalVariable(name: "lookup", arg: 1, scope: !18, file: !1, line: 16, type: !21)
!24 = !DIExpression()
!25 = !DILocation(line: 16, column: 23, scope: !18)
!26 = !DILocalVariable(name: "hints", scope: !18, file: !1, line: 19, type: !27)
!27 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "addrinfo", file: !28, line: 567, size: 384, align: 64, elements: !29)
!28 = !DIFile(filename: "/usr/include/netdb.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!29 = !{!30, !32, !33, !34, !35, !40, !54, !56}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "ai_flags", scope: !27, file: !28, line: 569, baseType: !31, size: 32, align: 32)
!31 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "ai_family", scope: !27, file: !28, line: 570, baseType: !31, size: 32, align: 32, offset: 32)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "ai_socktype", scope: !27, file: !28, line: 571, baseType: !31, size: 32, align: 32, offset: 64)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "ai_protocol", scope: !27, file: !28, line: 572, baseType: !31, size: 32, align: 32, offset: 96)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "ai_addrlen", scope: !27, file: !28, line: 573, baseType: !36, size: 32, align: 32, offset: 128)
!36 = !DIDerivedType(tag: DW_TAG_typedef, name: "socklen_t", file: !37, line: 277, baseType: !38)
!37 = !DIFile(filename: "/usr/include/unistd.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!38 = !DIDerivedType(tag: DW_TAG_typedef, name: "__socklen_t", file: !39, line: 189, baseType: !21)
!39 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!40 = !DIDerivedType(tag: DW_TAG_member, name: "ai_addr", scope: !27, file: !28, line: 574, baseType: !41, size: 64, align: 64, offset: 192)
!41 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !42, size: 64, align: 64)
!42 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr", file: !43, line: 170, size: 128, align: 16, elements: !44)
!43 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!44 = !{!45, !49}
!45 = !DIDerivedType(tag: DW_TAG_member, name: "sa_family", scope: !42, file: !43, line: 172, baseType: !46, size: 16, align: 16)
!46 = !DIDerivedType(tag: DW_TAG_typedef, name: "sa_family_t", file: !47, line: 28, baseType: !48)
!47 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/sockaddr.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!48 = !DIBasicType(name: "unsigned short", size: 16, align: 16, encoding: DW_ATE_unsigned)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "sa_data", scope: !42, file: !43, line: 173, baseType: !50, size: 112, align: 8, offset: 16)
!50 = !DICompositeType(tag: DW_TAG_array_type, baseType: !51, size: 112, align: 8, elements: !52)
!51 = !DIBasicType(name: "char", size: 8, align: 8, encoding: DW_ATE_signed_char)
!52 = !{!53}
!53 = !DISubrange(count: 14)
!54 = !DIDerivedType(tag: DW_TAG_member, name: "ai_canonname", scope: !27, file: !28, line: 575, baseType: !55, size: 64, align: 64, offset: 256)
!55 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !51, size: 64, align: 64)
!56 = !DIDerivedType(tag: DW_TAG_member, name: "ai_next", scope: !27, file: !28, line: 576, baseType: !57, size: 64, align: 64, offset: 320)
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !27, size: 64, align: 64)
!58 = !DILocation(line: 19, column: 18, scope: !18)
!59 = !DILocalVariable(name: "result", scope: !18, file: !1, line: 19, type: !57)
!60 = !DILocation(line: 19, column: 26, scope: !18)
!61 = !DILocation(line: 21, column: 6, scope: !18)
!62 = !DILocalVariable(name: "p", scope: !18, file: !1, line: 18, type: !55)
!63 = !DILocation(line: 18, column: 8, scope: !18)
!64 = !DILocation(line: 23, column: 2, scope: !18)
!65 = !DILocation(line: 24, column: 8, scope: !18)
!66 = !DILocation(line: 24, column: 18, scope: !18)
!67 = !DILocation(line: 25, column: 8, scope: !18)
!68 = !DILocation(line: 25, column: 20, scope: !18)
!69 = !DILocation(line: 26, column: 8, scope: !18)
!70 = !DILocation(line: 26, column: 17, scope: !18)
!71 = !DILocation(line: 27, column: 8, scope: !18)
!72 = !DILocation(line: 27, column: 20, scope: !18)
!73 = !DILocation(line: 29, column: 15, scope: !18)
!74 = !DILocation(line: 29, column: 2, scope: !75)
!75 = !DILexicalBlockFile(scope: !18, file: !1, discriminator: 1)
!76 = !DILocation(line: 30, column: 8, scope: !18)
!77 = !DILocation(line: 31, column: 1, scope: !18)
!78 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 33, type: !79, isLocal: false, isDefinition: true, scopeLine: 34, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !22)
!79 = !DISubroutineType(types: !80)
!80 = !{!31, !31, !81}
!81 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !55, size: 64, align: 64)
!82 = !DILocalVariable(name: "argc", arg: 1, scope: !78, file: !1, line: 33, type: !31)
!83 = !DILocation(line: 33, column: 10, scope: !78)
!84 = !DILocalVariable(name: "argv", arg: 2, scope: !78, file: !1, line: 33, type: !81)
!85 = !DILocation(line: 33, column: 23, scope: !78)
!86 = !DILocation(line: 35, column: 15, scope: !78)
!87 = !DILocation(line: 35, column: 2, scope: !88)
!88 = !DILexicalBlockFile(scope: !78, file: !1, discriminator: 1)
!89 = !DILocation(line: 36, column: 2, scope: !78)
