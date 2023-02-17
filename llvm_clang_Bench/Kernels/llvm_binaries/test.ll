; ModuleID = 'test.clcpp'
source_filename = "test.clcpp"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.34.31937"

$"??_C@_04LEHALOHF@?$CFi?5?6?$AA@" = comdat any

@"??_C@_04LEHALOHF@?$CFi?5?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [5 x i8] c"%i \0A\00", comdat, align 1

; Function Attrs: convergent norecurse nounwind uwtable
define dso_local spir_kernel void @bar(ptr nocapture noundef writeonly align 4 %0, ptr noundef %1) local_unnamed_addr #0 !kernel_arg_addr_space !5 !kernel_arg_access_qual !6 !kernel_arg_type !7 !kernel_arg_base_type !7 !kernel_arg_type_qual !8 {
  %3 = icmp ne ptr %1, null
  tail call void @llvm.assume(i1 %3)
  store i32 6, ptr %1, align 4, !tbaa !9
  br label %4

4:                                                ; preds = %2, %4
  %5 = phi i32 [ %8, %4 ], [ 0, %2 ]
  %6 = tail call noundef i32 (ptr, ...) @"?printf@@YAHPEAU?$_ASCLconstant@$$CBD@__clang@@ZZ"(ptr noundef nonnull @"??_C@_04LEHALOHF@?$CFi?5?6?$AA@", i32 noundef %5) #4
  %7 = mul nsw i32 %5, %5
  %8 = add nuw nsw i32 %7, 1
  %9 = load i32, ptr %1, align 4, !tbaa !9
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %4, label %11, !llvm.loop !14

11:                                               ; preds = %4
  %12 = tail call noundef i64 @"?get_global_id@@YAKI@Z"(i32 noundef 0) #5
  %13 = getelementptr inbounds i32, ptr %0, i64 %12
  store i32 2, ptr %13, align 4, !tbaa !16
  ret void
}

; Function Attrs: convergent mustprogress nofree nounwind readnone willreturn
declare dso_local noundef i64 @"?get_global_id@@YAKI@Z"(i32 noundef) local_unnamed_addr #1

; Function Attrs: convergent
declare dso_local noundef i32 @"?printf@@YAHPEAU?$_ASCLconstant@$$CBD@__clang@@ZZ"(ptr noundef, ...) local_unnamed_addr #2

; Function Attrs: inaccessiblememonly nocallback nofree nosync nounwind willreturn
declare void @llvm.assume(i1 noundef) #3

attributes #0 = { convergent norecurse nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "uniform-work-group-size"="true" }
attributes #1 = { convergent mustprogress nofree nounwind readnone willreturn "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { convergent "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { inaccessiblememonly nocallback nofree nosync nounwind willreturn }
attributes #4 = { convergent nounwind }
attributes #5 = { convergent nounwind readnone willreturn }

!llvm.module.flags = !{!0, !1, !2}
!opencl.ocl.version = !{!3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 2, i32 0}
!4 = !{!"clang version 15.0.7"}
!5 = !{i32 1, i32 3}
!6 = !{!"none", !"none"}
!7 = !{!"int*", !"void*"}
!8 = !{!"", !""}
!9 = !{!10, !11, i64 0}
!10 = !{!"?AVB@@", !11, i64 0}
!11 = !{!"int", !12, i64 0}
!12 = !{!"omnipotent char", !13, i64 0}
!13 = !{!"Simple C++ TBAA"}
!14 = distinct !{!14, !15}
!15 = !{!"llvm.loop.mustprogress"}
!16 = !{!11, !11, i64 0}
