; ModuleID = 'NSDL001.bc'
source_filename = "NSDL001.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.addrinfo = type { i32, i32, i32, i32, i32, %struct.sockaddr*, i8*, %struct.addrinfo* }
%struct.sockaddr = type { i16, [14 x i8] }

@.str = private unnamed_addr constant [15 x i8] c"enter passwd: \00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"http\00", align 1

; Function Attrs: nounwind uwtable
define void @leaks_passwd() #0 !dbg !18 {
  %1 = alloca i8*, align 8
  %2 = alloca %struct.addrinfo, align 8
  %3 = alloca %struct.addrinfo*, align 8
  call void @llvm.dbg.declare(metadata i8** %1, metadata !22, metadata !25), !dbg !26
  call void @llvm.dbg.declare(metadata %struct.addrinfo* %2, metadata !27, metadata !25), !dbg !58
  call void @llvm.dbg.declare(metadata %struct.addrinfo** %3, metadata !59, metadata !25), !dbg !60
  %4 = call i8* @getpass(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i32 0, i32 0)), !dbg !61
  store i8* %4, i8** %1, align 8, !dbg !62
  %5 = bitcast %struct.addrinfo* %2 to i8*, !dbg !63
  call void @llvm.memset.p0i8.i64(i8* %5, i8 0, i64 48, i32 8, i1 false), !dbg !63
  %6 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 1, !dbg !64
  store i32 0, i32* %6, align 4, !dbg !65
  %7 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 2, !dbg !66
  store i32 2, i32* %7, align 8, !dbg !67
  %8 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 0, !dbg !68
  store i32 0, i32* %8, align 8, !dbg !69
  %9 = getelementptr inbounds %struct.addrinfo, %struct.addrinfo* %2, i32 0, i32 3, !dbg !70
  store i32 0, i32* %9, align 4, !dbg !71
  %10 = load i8*, i8** %1, align 8, !dbg !72
  %11 = call i32 @getaddrinfo(i8* %10, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i32 0, i32 0), %struct.addrinfo* %2, %struct.addrinfo** %3), !dbg !73
  ret void, !dbg !74
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i8* @getpass(i8*) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #3

declare i32 @getaddrinfo(i8*, i8*, %struct.addrinfo*, %struct.addrinfo**) #2

; Function Attrs: nounwind uwtable
define i32 @main(i32, i8**) #0 !dbg !75 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  call void @llvm.dbg.declare(metadata i32* %4, metadata !79, metadata !25), !dbg !80
  store i8** %1, i8*** %5, align 8
  call void @llvm.dbg.declare(metadata i8*** %5, metadata !81, metadata !25), !dbg !82
  call void @leaks_passwd(), !dbg !83
  ret i32 0, !dbg !84
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!15, !16}
!llvm.ident = !{!17}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 3.9.0-1ubuntu1 (tags/RELEASE_390/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "NSDL001.c", directory: "/home/hoser/opcdematerial/code/comminute/tests")
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
!18 = distinct !DISubprogram(name: "leaks_passwd", scope: !1, file: !1, line: 15, type: !19, isLocal: false, isDefinition: true, scopeLine: 16, isOptimized: false, unit: !0, variables: !21)
!19 = !DISubroutineType(types: !20)
!20 = !{null}
!21 = !{}
!22 = !DILocalVariable(name: "p", scope: !18, file: !1, line: 17, type: !23)
!23 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !24, size: 64, align: 64)
!24 = !DIBasicType(name: "char", size: 8, align: 8, encoding: DW_ATE_signed_char)
!25 = !DIExpression()
!26 = !DILocation(line: 17, column: 8, scope: !18)
!27 = !DILocalVariable(name: "hints", scope: !18, file: !1, line: 18, type: !28)
!28 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "addrinfo", file: !29, line: 567, size: 384, align: 64, elements: !30)
!29 = !DIFile(filename: "/usr/include/netdb.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!30 = !{!31, !33, !34, !35, !36, !42, !55, !56}
!31 = !DIDerivedType(tag: DW_TAG_member, name: "ai_flags", scope: !28, file: !29, line: 569, baseType: !32, size: 32, align: 32)
!32 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "ai_family", scope: !28, file: !29, line: 570, baseType: !32, size: 32, align: 32, offset: 32)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "ai_socktype", scope: !28, file: !29, line: 571, baseType: !32, size: 32, align: 32, offset: 64)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "ai_protocol", scope: !28, file: !29, line: 572, baseType: !32, size: 32, align: 32, offset: 96)
!36 = !DIDerivedType(tag: DW_TAG_member, name: "ai_addrlen", scope: !28, file: !29, line: 573, baseType: !37, size: 32, align: 32, offset: 128)
!37 = !DIDerivedType(tag: DW_TAG_typedef, name: "socklen_t", file: !38, line: 277, baseType: !39)
!38 = !DIFile(filename: "/usr/include/unistd.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!39 = !DIDerivedType(tag: DW_TAG_typedef, name: "__socklen_t", file: !40, line: 189, baseType: !41)
!40 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!41 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!42 = !DIDerivedType(tag: DW_TAG_member, name: "ai_addr", scope: !28, file: !29, line: 574, baseType: !43, size: 64, align: 64, offset: 192)
!43 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 64, align: 64)
!44 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sockaddr", file: !45, line: 170, size: 128, align: 16, elements: !46)
!45 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/socket.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!46 = !{!47, !51}
!47 = !DIDerivedType(tag: DW_TAG_member, name: "sa_family", scope: !44, file: !45, line: 172, baseType: !48, size: 16, align: 16)
!48 = !DIDerivedType(tag: DW_TAG_typedef, name: "sa_family_t", file: !49, line: 28, baseType: !50)
!49 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/sockaddr.h", directory: "/home/hoser/opcdematerial/code/comminute/tests")
!50 = !DIBasicType(name: "unsigned short", size: 16, align: 16, encoding: DW_ATE_unsigned)
!51 = !DIDerivedType(tag: DW_TAG_member, name: "sa_data", scope: !44, file: !45, line: 173, baseType: !52, size: 112, align: 8, offset: 16)
!52 = !DICompositeType(tag: DW_TAG_array_type, baseType: !24, size: 112, align: 8, elements: !53)
!53 = !{!54}
!54 = !DISubrange(count: 14)
!55 = !DIDerivedType(tag: DW_TAG_member, name: "ai_canonname", scope: !28, file: !29, line: 575, baseType: !23, size: 64, align: 64, offset: 256)
!56 = !DIDerivedType(tag: DW_TAG_member, name: "ai_next", scope: !28, file: !29, line: 576, baseType: !57, size: 64, align: 64, offset: 320)
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64, align: 64)
!58 = !DILocation(line: 18, column: 18, scope: !18)
!59 = !DILocalVariable(name: "result", scope: !18, file: !1, line: 18, type: !57)
!60 = !DILocation(line: 18, column: 26, scope: !18)
!61 = !DILocation(line: 20, column: 6, scope: !18)
!62 = !DILocation(line: 20, column: 4, scope: !18)
!63 = !DILocation(line: 22, column: 2, scope: !18)
!64 = !DILocation(line: 23, column: 8, scope: !18)
!65 = !DILocation(line: 23, column: 18, scope: !18)
!66 = !DILocation(line: 24, column: 8, scope: !18)
!67 = !DILocation(line: 24, column: 20, scope: !18)
!68 = !DILocation(line: 25, column: 8, scope: !18)
!69 = !DILocation(line: 25, column: 17, scope: !18)
!70 = !DILocation(line: 26, column: 8, scope: !18)
!71 = !DILocation(line: 26, column: 20, scope: !18)
!72 = !DILocation(line: 28, column: 20, scope: !18)
!73 = !DILocation(line: 28, column: 8, scope: !18)
!74 = !DILocation(line: 29, column: 1, scope: !18)
!75 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 32, type: !76, isLocal: false, isDefinition: true, scopeLine: 33, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !21)
!76 = !DISubroutineType(types: !77)
!77 = !{!32, !32, !78}
!78 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !23, size: 64, align: 64)
!79 = !DILocalVariable(name: "argc", arg: 1, scope: !75, file: !1, line: 32, type: !32)
!80 = !DILocation(line: 32, column: 10, scope: !75)
!81 = !DILocalVariable(name: "argv", arg: 2, scope: !75, file: !1, line: 32, type: !78)
!82 = !DILocation(line: 32, column: 23, scope: !75)
!83 = !DILocation(line: 34, column: 2, scope: !75)
!84 = !DILocation(line: 35, column: 2, scope: !75)
