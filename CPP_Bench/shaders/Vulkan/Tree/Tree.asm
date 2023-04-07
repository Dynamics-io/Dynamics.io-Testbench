; ModuleID = 'Tree.cl'
source_filename = "Tree.cl"
target datalayout = "e-p:32:32-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "spir-unknown-unknown"

%struct.tree_counts = type { i32, i32 }
%struct.metanode = type { i32, i32, i32, float }
%struct.BoundingBox = type { <4 x float>, <4 x float> }
%struct.InsertionChoice = type { i32, %struct.BoundingBox, float, [12 x i8] }

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @Node_GetChildIndex(i32 %index, i32 %child) #0 {
entry:
  %index.addr = alloca i32, align 4
  %child.addr = alloca i32, align 4
  store i32 %index, ptr %index.addr, align 4
  store i32 %child, ptr %child.addr, align 4
  %0 = load i32, ptr %index.addr, align 4
  %mul = mul nsw i32 %0, 2
  %1 = load i32, ptr %child.addr, align 4
  %add = add nsw i32 %mul, %1
  ret i32 %add
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @Encode(i32 %index) #0 {
entry:
  %index.addr = alloca i32, align 4
  store i32 %index, ptr %index.addr, align 4
  %0 = load i32, ptr %index.addr, align 4
  %sub = sub nsw i32 -1, %0
  ret i32 %sub
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @Leaf_New(i32 %nodeIndex, i32 %childIndex) #0 {
entry:
  %nodeIndex.addr = alloca i32, align 4
  %childIndex.addr = alloca i32, align 4
  store i32 %nodeIndex, ptr %nodeIndex.addr, align 4
  store i32 %childIndex, ptr %childIndex.addr, align 4
  %0 = load i32, ptr %nodeIndex.addr, align 4
  %and = and i32 %0, 2147483647
  %1 = load i32, ptr %childIndex.addr, align 4
  %shl = shl i32 %1, 31
  %or = or i32 %and, %shl
  ret i32 %or
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @Leaf_NodeIndex(i32 %leaf) #0 {
entry:
  %leaf.addr = alloca i32, align 4
  store i32 %leaf, ptr %leaf.addr, align 4
  %0 = load i32, ptr %leaf.addr, align 4
  %and = and i32 %0, 2147483647
  ret i32 %and
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @Leaf_ChildIndex(i32 %leaf) #0 {
entry:
  %leaf.addr = alloca i32, align 4
  store i32 %leaf, ptr %leaf.addr, align 4
  %0 = load i32, ptr %leaf.addr, align 4
  %and = and i32 %0, -2147483648
  %shr = lshr i32 %and, 31
  ret i32 %shr
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @AllocateNode(ptr addrspace(1) %counts) #0 {
entry:
  %counts.addr = alloca ptr addrspace(1), align 4
  %tmp = alloca %struct.tree_counts, align 4
  %count = alloca i32, align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  %0 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %0, i32 0
  call void @llvm.memcpy.p0.p1.i32(ptr align 4 %tmp, ptr addrspace(1) align 4 %arrayidx, i32 8, i1 false)
  %NodeCount = getelementptr inbounds %struct.tree_counts, ptr %tmp, i32 0, i32 0
  %1 = load i32, ptr %NodeCount, align 4
  store i32 %1, ptr %count, align 4
  %2 = load i32, ptr %count, align 4
  %add = add nsw i32 %2, 1
  %NodeCount1 = getelementptr inbounds %struct.tree_counts, ptr %tmp, i32 0, i32 0
  store i32 %add, ptr %NodeCount1, align 4
  %3 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx2 = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %3, i32 0
  call void @llvm.memcpy.p1.p0.i32(ptr addrspace(1) align 4 %arrayidx2, ptr align 4 %tmp, i32 8, i1 false)
  %4 = load i32, ptr %count, align 4
  ret i32 %4
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p1.i32(ptr noalias nocapture writeonly, ptr addrspace(1) noalias nocapture readonly, i32, i1 immarg) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p1.p0.i32(ptr addrspace(1) noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #1

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @IncrementLeaves(ptr addrspace(1) %counts) #0 {
entry:
  %counts.addr = alloca ptr addrspace(1), align 4
  %cnts = alloca %struct.tree_counts, align 4
  %count = alloca i32, align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  %0 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %0, i32 0
  call void @llvm.memcpy.p0.p1.i32(ptr align 4 %cnts, ptr addrspace(1) align 4 %arrayidx, i32 8, i1 false)
  %LeafCount = getelementptr inbounds %struct.tree_counts, ptr %cnts, i32 0, i32 1
  %1 = load i32, ptr %LeafCount, align 4
  store i32 %1, ptr %count, align 4
  %2 = load i32, ptr %count, align 4
  %add = add nsw i32 %2, 1
  %LeafCount1 = getelementptr inbounds %struct.tree_counts, ptr %cnts, i32 0, i32 1
  store i32 %add, ptr %LeafCount1, align 4
  %3 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx2 = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %3, i32 0
  call void @llvm.memcpy.p1.p0.i32(ptr addrspace(1) align 4 %arrayidx2, ptr align 4 %cnts, i32 8, i1 false)
  %4 = load i32, ptr %count, align 4
  ret i32 %4
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @AddLeaf(i32 %nodeIndex, i32 %childIndex, ptr addrspace(1) %counts, ptr addrspace(1) %leaves) #0 {
entry:
  %nodeIndex.addr = alloca i32, align 4
  %childIndex.addr = alloca i32, align 4
  %counts.addr = alloca ptr addrspace(1), align 4
  %leaves.addr = alloca ptr addrspace(1), align 4
  %cnts = alloca %struct.tree_counts, align 4
  %count = alloca i32, align 4
  store i32 %nodeIndex, ptr %nodeIndex.addr, align 4
  store i32 %childIndex, ptr %childIndex.addr, align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  store ptr addrspace(1) %leaves, ptr %leaves.addr, align 4
  %0 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %0, i32 0
  call void @llvm.memcpy.p0.p1.i32(ptr align 4 %cnts, ptr addrspace(1) align 4 %arrayidx, i32 8, i1 false)
  %LeafCount = getelementptr inbounds %struct.tree_counts, ptr %cnts, i32 0, i32 1
  %1 = load i32, ptr %LeafCount, align 4
  store i32 %1, ptr %count, align 4
  %2 = load i32, ptr %nodeIndex.addr, align 4
  %3 = load i32, ptr %childIndex.addr, align 4
  %call = call spir_func i32 @Leaf_New(i32 %2, i32 %3) #5
  %4 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %5 = load i32, ptr %count, align 4
  %arrayidx1 = getelementptr inbounds i32, ptr addrspace(1) %4, i32 %5
  store i32 %call, ptr addrspace(1) %arrayidx1, align 4
  %6 = load i32, ptr %count, align 4
  %add = add nsw i32 %6, 1
  %LeafCount2 = getelementptr inbounds %struct.tree_counts, ptr %cnts, i32 0, i32 1
  store i32 %add, ptr %LeafCount2, align 4
  %7 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx3 = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %7, i32 0
  call void @llvm.memcpy.p1.p0.i32(ptr addrspace(1) align 4 %arrayidx3, ptr align 4 %cnts, i32 8, i1 false)
  %8 = load i32, ptr %count, align 4
  ret i32 %8
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_kernel void @Init(ptr addrspace(1) align 4 %counts, ptr addrspace(1) align 4 %metanodes) #2 !kernel_arg_addr_space !3 !kernel_arg_access_qual !4 !kernel_arg_type !5 !kernel_arg_base_type !5 !kernel_arg_type_qual !6 !reqd_work_group_size !7 {
entry:
  %counts.addr = alloca ptr addrspace(1), align 4
  %metanodes.addr = alloca ptr addrspace(1), align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  store ptr addrspace(1) %metanodes, ptr %metanodes.addr, align 4
  %0 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %0, i32 0
  %NodeCount = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %arrayidx, i32 0, i32 0
  store i32 1, ptr addrspace(1) %NodeCount, align 4
  %1 = load ptr addrspace(1), ptr %counts.addr, align 4
  %arrayidx1 = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %1, i32 0
  %LeafCount = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %arrayidx1, i32 0, i32 1
  store i32 0, ptr addrspace(1) %LeafCount, align 4
  %2 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %arrayidx2 = getelementptr inbounds %struct.metanode, ptr addrspace(1) %2, i32 0
  %Parent = getelementptr inbounds %struct.metanode, ptr addrspace(1) %arrayidx2, i32 0, i32 0
  store i32 -1, ptr addrspace(1) %Parent, align 4
  %3 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %arrayidx3 = getelementptr inbounds %struct.metanode, ptr addrspace(1) %3, i32 0
  %IndexInParent = getelementptr inbounds %struct.metanode, ptr addrspace(1) %arrayidx3, i32 0, i32 1
  store i32 -1, ptr addrspace(1) %IndexInParent, align 4
  ret void
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @InsertLeafIntoEmptySlot(ptr byval(%struct.BoundingBox) align 16 %leafBox, i32 %nodeIndex, i32 %childIndex, ptr addrspace(1) %NodeChild_Min, ptr addrspace(1) %NodeChild_Max, ptr addrspace(1) %NodeChild_Index, ptr addrspace(1) %NodeChild_LeafCount, ptr addrspace(1) %counts, ptr addrspace(1) %leaves) #0 {
entry:
  %nodeIndex.addr = alloca i32, align 4
  %childIndex.addr = alloca i32, align 4
  %NodeChild_Min.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Max.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Index.addr = alloca ptr addrspace(1), align 4
  %NodeChild_LeafCount.addr = alloca ptr addrspace(1), align 4
  %counts.addr = alloca ptr addrspace(1), align 4
  %leaves.addr = alloca ptr addrspace(1), align 4
  %leafIndex = alloca i32, align 4
  %child = alloca i32, align 4
  store i32 %nodeIndex, ptr %nodeIndex.addr, align 4
  store i32 %childIndex, ptr %childIndex.addr, align 4
  store ptr addrspace(1) %NodeChild_Min, ptr %NodeChild_Min.addr, align 4
  store ptr addrspace(1) %NodeChild_Max, ptr %NodeChild_Max.addr, align 4
  store ptr addrspace(1) %NodeChild_Index, ptr %NodeChild_Index.addr, align 4
  store ptr addrspace(1) %NodeChild_LeafCount, ptr %NodeChild_LeafCount.addr, align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  store ptr addrspace(1) %leaves, ptr %leaves.addr, align 4
  %0 = load i32, ptr %nodeIndex.addr, align 4
  %1 = load i32, ptr %childIndex.addr, align 4
  %2 = load ptr addrspace(1), ptr %counts.addr, align 4
  %3 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %call = call spir_func i32 @AddLeaf(i32 %0, i32 %1, ptr addrspace(1) %2, ptr addrspace(1) %3) #5
  store i32 %call, ptr %leafIndex, align 4
  %4 = load i32, ptr %nodeIndex.addr, align 4
  %5 = load i32, ptr %childIndex.addr, align 4
  %call1 = call spir_func i32 @Node_GetChildIndex(i32 %4, i32 %5) #5
  store i32 %call1, ptr %child, align 4
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %leafBox, i32 0, i32 0
  %6 = load <4 x float>, ptr %Min, align 16
  %7 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %8 = load i32, ptr %child, align 4
  %arrayidx = getelementptr inbounds <4 x float>, ptr addrspace(1) %7, i32 %8
  store <4 x float> %6, ptr addrspace(1) %arrayidx, align 16
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %leafBox, i32 0, i32 1
  %9 = load <4 x float>, ptr %Max, align 16
  %10 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %11 = load i32, ptr %child, align 4
  %arrayidx2 = getelementptr inbounds <4 x float>, ptr addrspace(1) %10, i32 %11
  store <4 x float> %9, ptr addrspace(1) %arrayidx2, align 16
  %12 = load i32, ptr %leafIndex, align 4
  %call3 = call spir_func i32 @Encode(i32 %12) #5
  %13 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %14 = load i32, ptr %child, align 4
  %arrayidx4 = getelementptr inbounds i32, ptr addrspace(1) %13, i32 %14
  store i32 %call3, ptr addrspace(1) %arrayidx4, align 4
  %15 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %16 = load i32, ptr %child, align 4
  %arrayidx5 = getelementptr inbounds i32, ptr addrspace(1) %15, i32 %16
  store i32 1, ptr addrspace(1) %arrayidx5, align 4
  %17 = load i32, ptr %leafIndex, align 4
  ret i32 %17
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func float @ComputeBoundsMetric_2(<4 x float> %min, <4 x float> %max) #0 {
entry:
  %min.addr = alloca <4 x float>, align 16
  %max.addr = alloca <4 x float>, align 16
  %offset = alloca <4 x float>, align 16
  store <4 x float> %min, ptr %min.addr, align 16
  store <4 x float> %max, ptr %max.addr, align 16
  %0 = load <4 x float>, ptr %max.addr, align 16
  %1 = load <4 x float>, ptr %min.addr, align 16
  %sub = fsub <4 x float> %0, %1
  store <4 x float> %sub, ptr %offset, align 16
  %2 = load <4 x float>, ptr %offset, align 16
  %3 = extractelement <4 x float> %2, i32 0
  %4 = load <4 x float>, ptr %offset, align 16
  %5 = extractelement <4 x float> %4, i32 1
  %6 = load <4 x float>, ptr %offset, align 16
  %7 = extractelement <4 x float> %6, i32 1
  %8 = load <4 x float>, ptr %offset, align 16
  %9 = extractelement <4 x float> %8, i32 2
  %mul1 = fmul float %7, %9
  %10 = call float @llvm.fmuladd.f32(float %3, float %5, float %mul1)
  %11 = load <4 x float>, ptr %offset, align 16
  %12 = extractelement <4 x float> %11, i32 0
  %13 = load <4 x float>, ptr %offset, align 16
  %14 = extractelement <4 x float> %13, i32 2
  %15 = call float @llvm.fmuladd.f32(float %12, float %14, float %10)
  ret float %15
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #3

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func float @ComputeBoundsMetric_1(ptr byval(%struct.BoundingBox) align 16 %bounds) #0 {
entry:
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %bounds, i32 0, i32 0
  %0 = load <4 x float>, ptr %Min, align 16
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %bounds, i32 0, i32 1
  %1 = load <4 x float>, ptr %Max, align 16
  %call = call spir_func float @ComputeBoundsMetric_2(<4 x float> %0, <4 x float> %1) #5
  ret float %call
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func void @CreateMerged(ptr noalias sret(%struct.BoundingBox) align 16 %agg.result, <4 x float> %minA, <4 x float> %maxA, <4 x float> %minB, <4 x float> %maxB) #0 {
entry:
  %minA.addr = alloca <4 x float>, align 16
  %maxA.addr = alloca <4 x float>, align 16
  %minB.addr = alloca <4 x float>, align 16
  %maxB.addr = alloca <4 x float>, align 16
  store <4 x float> %minA, ptr %minA.addr, align 16
  store <4 x float> %maxA, ptr %maxA.addr, align 16
  store <4 x float> %minB, ptr %minB.addr, align 16
  store <4 x float> %maxB, ptr %maxB.addr, align 16
  %0 = load <4 x float>, ptr %minA.addr, align 16
  %1 = load <4 x float>, ptr %minB.addr, align 16
  %call = call spir_func <4 x float> @_Z3minDv4_fS_(<4 x float> %0, <4 x float> %1) #6
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %agg.result, i32 0, i32 0
  store <4 x float> %call, ptr %Min, align 16
  %2 = load <4 x float>, ptr %maxA.addr, align 16
  %3 = load <4 x float>, ptr %maxB.addr, align 16
  %call1 = call spir_func <4 x float> @_Z3maxDv4_fS_(<4 x float> %2, <4 x float> %3) #6
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %agg.result, i32 0, i32 1
  store <4 x float> %call1, ptr %Max, align 16
  ret void
}

; Function Attrs: convergent nounwind willreturn memory(none)
declare spir_func <4 x float> @_Z3minDv4_fS_(<4 x float>, <4 x float>) #4

; Function Attrs: convergent nounwind willreturn memory(none)
declare spir_func <4 x float> @_Z3maxDv4_fS_(<4 x float>, <4 x float>) #4

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @LeadingZeros(i32 %x) #0 {
entry:
  %x.addr = alloca i32, align 4
  %numIntBits = alloca i32, align 4
  store i32 %x, ptr %x.addr, align 4
  store i32 32, ptr %numIntBits, align 4
  %0 = load i32, ptr %x.addr, align 4
  %shr = ashr i32 %0, 1
  %1 = load i32, ptr %x.addr, align 4
  %or = or i32 %1, %shr
  store i32 %or, ptr %x.addr, align 4
  %2 = load i32, ptr %x.addr, align 4
  %shr1 = ashr i32 %2, 2
  %3 = load i32, ptr %x.addr, align 4
  %or2 = or i32 %3, %shr1
  store i32 %or2, ptr %x.addr, align 4
  %4 = load i32, ptr %x.addr, align 4
  %shr3 = ashr i32 %4, 4
  %5 = load i32, ptr %x.addr, align 4
  %or4 = or i32 %5, %shr3
  store i32 %or4, ptr %x.addr, align 4
  %6 = load i32, ptr %x.addr, align 4
  %shr5 = ashr i32 %6, 8
  %7 = load i32, ptr %x.addr, align 4
  %or6 = or i32 %7, %shr5
  store i32 %or6, ptr %x.addr, align 4
  %8 = load i32, ptr %x.addr, align 4
  %shr7 = ashr i32 %8, 16
  %9 = load i32, ptr %x.addr, align 4
  %or8 = or i32 %9, %shr7
  store i32 %or8, ptr %x.addr, align 4
  %10 = load i32, ptr %x.addr, align 4
  %shr9 = ashr i32 %10, 1
  %and = and i32 %shr9, 1431655765
  %11 = load i32, ptr %x.addr, align 4
  %sub = sub nsw i32 %11, %and
  store i32 %sub, ptr %x.addr, align 4
  %12 = load i32, ptr %x.addr, align 4
  %shr10 = ashr i32 %12, 2
  %and11 = and i32 %shr10, 858993459
  %13 = load i32, ptr %x.addr, align 4
  %and12 = and i32 %13, 858993459
  %add = add nsw i32 %and11, %and12
  store i32 %add, ptr %x.addr, align 4
  %14 = load i32, ptr %x.addr, align 4
  %shr13 = ashr i32 %14, 4
  %15 = load i32, ptr %x.addr, align 4
  %add14 = add nsw i32 %shr13, %15
  %and15 = and i32 %add14, 252645135
  store i32 %and15, ptr %x.addr, align 4
  %16 = load i32, ptr %x.addr, align 4
  %shr16 = ashr i32 %16, 8
  %17 = load i32, ptr %x.addr, align 4
  %add17 = add nsw i32 %17, %shr16
  store i32 %add17, ptr %x.addr, align 4
  %18 = load i32, ptr %x.addr, align 4
  %shr18 = ashr i32 %18, 16
  %19 = load i32, ptr %x.addr, align 4
  %add19 = add nsw i32 %19, %shr18
  store i32 %add19, ptr %x.addr, align 4
  %20 = load i32, ptr %x.addr, align 4
  %and20 = and i32 %20, 63
  %sub21 = sub nsw i32 32, %and20
  ret i32 %sub21
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @GetContainingPowerOf2(i32 %i) #0 {
entry:
  %i.addr = alloca i32, align 4
  %signed_ = alloca i32, align 4
  store i32 %i, ptr %i.addr, align 4
  %0 = load i32, ptr %i.addr, align 4
  %cmp = icmp eq i32 %0, 0
  br i1 %cmp, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  br label %cond.end

cond.false:                                       ; preds = %entry
  %1 = load i32, ptr %i.addr, align 4
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ 1, %cond.true ], [ %1, %cond.false ]
  store i32 %cond, ptr %signed_, align 4
  %2 = load i32, ptr %signed_, align 4
  %sub = sub nsw i32 %2, 1
  %call = call spir_func i32 @LeadingZeros(i32 %sub) #5
  %sub1 = sub nsw i32 32, %call
  ret i32 %sub1
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func void @ComputeBestInsertionChoice(ptr noalias sret(%struct.InsertionChoice) align 16 %agg.result, ptr byval(%struct.BoundingBox) align 16 %bounds, float %newLeafCost, <4 x float> %NodeChild_Min, <4 x float> %NodeChild_Max, i32 %NodeChild_Index, i32 %NodeChild_LeafCount) #0 {
entry:
  %newLeafCost.addr = alloca float, align 4
  %NodeChild_Min.addr = alloca <4 x float>, align 16
  %NodeChild_Max.addr = alloca <4 x float>, align 16
  %NodeChild_Index.addr = alloca i32, align 4
  %NodeChild_LeafCount.addr = alloca i32, align 4
  %tmp = alloca %struct.BoundingBox, align 16
  %newCost = alloca float, align 4
  store float %newLeafCost, ptr %newLeafCost.addr, align 4
  store <4 x float> %NodeChild_Min, ptr %NodeChild_Min.addr, align 16
  store <4 x float> %NodeChild_Max, ptr %NodeChild_Max.addr, align 16
  store i32 %NodeChild_Index, ptr %NodeChild_Index.addr, align 4
  store i32 %NodeChild_LeafCount, ptr %NodeChild_LeafCount.addr, align 4
  %MergedCandidate = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 1
  %0 = load <4 x float>, ptr %NodeChild_Min.addr, align 16
  %1 = load <4 x float>, ptr %NodeChild_Max.addr, align 16
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %bounds, i32 0, i32 0
  %2 = load <4 x float>, ptr %Min, align 16
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %bounds, i32 0, i32 1
  %3 = load <4 x float>, ptr %Max, align 16
  call spir_func void @CreateMerged(ptr sret(%struct.BoundingBox) align 16 %tmp, <4 x float> %0, <4 x float> %1, <4 x float> %2, <4 x float> %3) #5
  call void @llvm.memcpy.p0.p0.i32(ptr align 16 %MergedCandidate, ptr align 16 %tmp, i32 32, i1 false)
  %MergedCandidate1 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 1
  %call = call spir_func float @ComputeBoundsMetric_1(ptr byval(%struct.BoundingBox) align 16 %MergedCandidate1) #5
  store float %call, ptr %newCost, align 4
  %4 = load i32, ptr %NodeChild_Index.addr, align 4
  %cmp = icmp sge i32 %4, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %5 = load float, ptr %newCost, align 4
  %6 = load <4 x float>, ptr %NodeChild_Min.addr, align 16
  %7 = load <4 x float>, ptr %NodeChild_Max.addr, align 16
  %call2 = call spir_func float @ComputeBoundsMetric_2(<4 x float> %6, <4 x float> %7) #5
  %sub = fsub float %5, %call2
  %CostChange = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 2
  store float %sub, ptr %CostChange, align 16
  %CostChange3 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 2
  %8 = load float, ptr %CostChange3, align 16
  %9 = load i32, ptr %NodeChild_LeafCount.addr, align 4
  %call4 = call spir_func i32 @GetContainingPowerOf2(i32 %9) #5
  %conv = sitofp i32 %call4 to float
  %10 = load float, ptr %newLeafCost.addr, align 4
  %CostChange5 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 2
  %11 = load float, ptr %CostChange5, align 16
  %call6 = call spir_func float @_Z3maxff(float %10, float %11) #6
  %12 = call float @llvm.fmuladd.f32(float %conv, float %call6, float %8)
  %CostChange7 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 2
  store float %12, ptr %CostChange7, align 16
  %Choice = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 0
  store i32 1, ptr %Choice, align 16
  br label %if.end

if.else:                                          ; preds = %entry
  %13 = load float, ptr %newCost, align 4
  %CostChange8 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 2
  store float %13, ptr %CostChange8, align 16
  %Choice9 = getelementptr inbounds %struct.InsertionChoice, ptr %agg.result, i32 0, i32 0
  store i32 0, ptr %Choice9, align 16
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #1

; Function Attrs: convergent nounwind willreturn memory(none)
declare spir_func float @_Z3maxff(float, float) #4

; Function Attrs: convergent norecurse nounwind
define dso_local spir_func i32 @MergeLeafNodes(ptr byval(%struct.BoundingBox) align 16 %newLeafBounds, i32 %parentIndex, i32 %indexInParent, ptr byval(%struct.BoundingBox) align 16 %merged, ptr addrspace(1) %counts, ptr addrspace(1) %metanodes, ptr addrspace(1) %NodeChild_Min, ptr addrspace(1) %NodeChild_Max, ptr addrspace(1) %NodeChild_Index, ptr addrspace(1) %NodeChild_LeafCount, ptr addrspace(1) %leaves) #0 {
entry:
  %parentIndex.addr = alloca i32, align 4
  %indexInParent.addr = alloca i32, align 4
  %counts.addr = alloca ptr addrspace(1), align 4
  %metanodes.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Min.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Max.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Index.addr = alloca ptr addrspace(1), align 4
  %NodeChild_LeafCount.addr = alloca ptr addrspace(1), align 4
  %leaves.addr = alloca ptr addrspace(1), align 4
  %newNodeIndex = alloca i32, align 4
  %newNode = alloca i32, align 4
  %parentNode = alloca i32, align 4
  %childInParent = alloca i32, align 4
  %b = alloca i32, align 4
  %leafIndex = alloca i32, align 4
  %new_a = alloca i32, align 4
  %oldLeafIndex = alloca i32, align 4
  store i32 %parentIndex, ptr %parentIndex.addr, align 4
  store i32 %indexInParent, ptr %indexInParent.addr, align 4
  store ptr addrspace(1) %counts, ptr %counts.addr, align 4
  store ptr addrspace(1) %metanodes, ptr %metanodes.addr, align 4
  store ptr addrspace(1) %NodeChild_Min, ptr %NodeChild_Min.addr, align 4
  store ptr addrspace(1) %NodeChild_Max, ptr %NodeChild_Max.addr, align 4
  store ptr addrspace(1) %NodeChild_Index, ptr %NodeChild_Index.addr, align 4
  store ptr addrspace(1) %NodeChild_LeafCount, ptr %NodeChild_LeafCount.addr, align 4
  store ptr addrspace(1) %leaves, ptr %leaves.addr, align 4
  %0 = load ptr addrspace(1), ptr %counts.addr, align 4
  %call = call spir_func i32 @AllocateNode(ptr addrspace(1) %0) #5
  store i32 %call, ptr %newNodeIndex, align 4
  %1 = load i32, ptr %newNodeIndex, align 4
  store i32 %1, ptr %newNode, align 4
  %2 = load i32, ptr %parentIndex.addr, align 4
  %3 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %4 = load i32, ptr %newNodeIndex, align 4
  %arrayidx = getelementptr inbounds %struct.metanode, ptr addrspace(1) %3, i32 %4
  %Parent = getelementptr inbounds %struct.metanode, ptr addrspace(1) %arrayidx, i32 0, i32 0
  store i32 %2, ptr addrspace(1) %Parent, align 4
  %5 = load i32, ptr %indexInParent.addr, align 4
  %6 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %7 = load i32, ptr %newNodeIndex, align 4
  %arrayidx1 = getelementptr inbounds %struct.metanode, ptr addrspace(1) %6, i32 %7
  %IndexInParent = getelementptr inbounds %struct.metanode, ptr addrspace(1) %arrayidx1, i32 0, i32 1
  store i32 %5, ptr addrspace(1) %IndexInParent, align 4
  %8 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %9 = load i32, ptr %newNodeIndex, align 4
  %arrayidx2 = getelementptr inbounds %struct.metanode, ptr addrspace(1) %8, i32 %9
  %RefineFlag = getelementptr inbounds %struct.metanode, ptr addrspace(1) %arrayidx2, i32 0, i32 2
  store i32 0, ptr addrspace(1) %RefineFlag, align 4
  %10 = load i32, ptr %parentIndex.addr, align 4
  store i32 %10, ptr %parentNode, align 4
  %11 = load i32, ptr %parentNode, align 4
  %12 = load i32, ptr %indexInParent.addr, align 4
  %call3 = call spir_func i32 @Node_GetChildIndex(i32 %11, i32 %12) #5
  store i32 %call3, ptr %childInParent, align 4
  %13 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %14 = load i32, ptr %childInParent, align 4
  %arrayidx4 = getelementptr inbounds <4 x float>, ptr addrspace(1) %13, i32 %14
  %15 = load <4 x float>, ptr addrspace(1) %arrayidx4, align 16
  %16 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %17 = load i32, ptr %newNode, align 4
  %arrayidx5 = getelementptr inbounds <4 x float>, ptr addrspace(1) %16, i32 %17
  store <4 x float> %15, ptr addrspace(1) %arrayidx5, align 16
  %18 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %19 = load i32, ptr %childInParent, align 4
  %arrayidx6 = getelementptr inbounds <4 x float>, ptr addrspace(1) %18, i32 %19
  %20 = load <4 x float>, ptr addrspace(1) %arrayidx6, align 16
  %21 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %22 = load i32, ptr %newNode, align 4
  %arrayidx7 = getelementptr inbounds <4 x float>, ptr addrspace(1) %21, i32 %22
  store <4 x float> %20, ptr addrspace(1) %arrayidx7, align 16
  %23 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %24 = load i32, ptr %childInParent, align 4
  %arrayidx8 = getelementptr inbounds i32, ptr addrspace(1) %23, i32 %24
  %25 = load i32, ptr addrspace(1) %arrayidx8, align 4
  %26 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %27 = load i32, ptr %newNode, align 4
  %arrayidx9 = getelementptr inbounds i32, ptr addrspace(1) %26, i32 %27
  store i32 %25, ptr addrspace(1) %arrayidx9, align 4
  %28 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %29 = load i32, ptr %childInParent, align 4
  %arrayidx10 = getelementptr inbounds i32, ptr addrspace(1) %28, i32 %29
  %30 = load i32, ptr addrspace(1) %arrayidx10, align 4
  %31 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %32 = load i32, ptr %newNode, align 4
  %arrayidx11 = getelementptr inbounds i32, ptr addrspace(1) %31, i32 %32
  store i32 %30, ptr addrspace(1) %arrayidx11, align 4
  %33 = load i32, ptr %newNode, align 4
  %call12 = call spir_func i32 @Node_GetChildIndex(i32 %33, i32 1) #5
  store i32 %call12, ptr %b, align 4
  %34 = load i32, ptr %newNodeIndex, align 4
  %35 = load ptr addrspace(1), ptr %counts.addr, align 4
  %36 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %call13 = call spir_func i32 @AddLeaf(i32 %34, i32 1, ptr addrspace(1) %35, ptr addrspace(1) %36) #5
  store i32 %call13, ptr %leafIndex, align 4
  %37 = load i32, ptr %leafIndex, align 4
  %call14 = call spir_func i32 @Encode(i32 %37) #5
  %38 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %39 = load i32, ptr %b, align 4
  %arrayidx15 = getelementptr inbounds i32, ptr addrspace(1) %38, i32 %39
  store i32 %call14, ptr addrspace(1) %arrayidx15, align 4
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %newLeafBounds, i32 0, i32 0
  %40 = load <4 x float>, ptr %Min, align 16
  %41 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %42 = load i32, ptr %b, align 4
  %arrayidx16 = getelementptr inbounds <4 x float>, ptr addrspace(1) %41, i32 %42
  store <4 x float> %40, ptr addrspace(1) %arrayidx16, align 16
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %newLeafBounds, i32 0, i32 1
  %43 = load <4 x float>, ptr %Max, align 16
  %44 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %45 = load i32, ptr %b, align 4
  %arrayidx17 = getelementptr inbounds <4 x float>, ptr addrspace(1) %44, i32 %45
  store <4 x float> %43, ptr addrspace(1) %arrayidx17, align 16
  %46 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %47 = load i32, ptr %b, align 4
  %arrayidx18 = getelementptr inbounds i32, ptr addrspace(1) %46, i32 %47
  store i32 1, ptr addrspace(1) %arrayidx18, align 4
  %48 = load i32, ptr %newNode, align 4
  %call19 = call spir_func i32 @Node_GetChildIndex(i32 %48, i32 0) #5
  store i32 %call19, ptr %new_a, align 4
  %49 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %50 = load i32, ptr %new_a, align 4
  %arrayidx20 = getelementptr inbounds i32, ptr addrspace(1) %49, i32 %50
  %51 = load i32, ptr addrspace(1) %arrayidx20, align 4
  %call21 = call spir_func i32 @Encode(i32 %51) #5
  store i32 %call21, ptr %oldLeafIndex, align 4
  %52 = load i32, ptr %newNodeIndex, align 4
  %call22 = call spir_func i32 @Leaf_New(i32 %52, i32 0) #5
  %53 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %54 = load i32, ptr %oldLeafIndex, align 4
  %arrayidx23 = getelementptr inbounds i32, ptr addrspace(1) %53, i32 %54
  store i32 %call22, ptr addrspace(1) %arrayidx23, align 4
  %55 = load i32, ptr %newNodeIndex, align 4
  %56 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %57 = load i32, ptr %childInParent, align 4
  %arrayidx24 = getelementptr inbounds i32, ptr addrspace(1) %56, i32 %57
  store i32 %55, ptr addrspace(1) %arrayidx24, align 4
  %Min25 = getelementptr inbounds %struct.BoundingBox, ptr %merged, i32 0, i32 0
  %58 = load <4 x float>, ptr %Min25, align 16
  %59 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %60 = load i32, ptr %childInParent, align 4
  %arrayidx26 = getelementptr inbounds <4 x float>, ptr addrspace(1) %59, i32 %60
  store <4 x float> %58, ptr addrspace(1) %arrayidx26, align 16
  %Max27 = getelementptr inbounds %struct.BoundingBox, ptr %merged, i32 0, i32 1
  %61 = load <4 x float>, ptr %Max27, align 16
  %62 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %63 = load i32, ptr %childInParent, align 4
  %arrayidx28 = getelementptr inbounds <4 x float>, ptr addrspace(1) %62, i32 %63
  store <4 x float> %61, ptr addrspace(1) %arrayidx28, align 16
  %64 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %65 = load i32, ptr %childInParent, align 4
  %arrayidx29 = getelementptr inbounds i32, ptr addrspace(1) %64, i32 %65
  store i32 2, ptr addrspace(1) %arrayidx29, align 4
  %66 = load i32, ptr %leafIndex, align 4
  ret i32 %66
}

; Function Attrs: convergent norecurse nounwind
define dso_local spir_kernel void @Add(ptr addrspace(1) align 4 %counts_ref, ptr addrspace(1) align 16 %bounds_arr, ptr addrspace(1) align 4 %numBounds, ptr addrspace(1) align 16 %NodeChild_Min, ptr addrspace(1) align 16 %NodeChild_Max, ptr addrspace(1) align 4 %NodeChild_Index, ptr addrspace(1) align 4 %NodeChild_LeafCount, ptr addrspace(1) align 4 %leaves, ptr addrspace(1) align 4 %metanodes, ptr addrspace(1) align 4 %result) #2 !kernel_arg_addr_space !8 !kernel_arg_access_qual !9 !kernel_arg_type !10 !kernel_arg_base_type !11 !kernel_arg_type_qual !12 !reqd_work_group_size !7 {
entry:
  %counts_ref.addr = alloca ptr addrspace(1), align 4
  %bounds_arr.addr = alloca ptr addrspace(1), align 4
  %numBounds.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Min.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Max.addr = alloca ptr addrspace(1), align 4
  %NodeChild_Index.addr = alloca ptr addrspace(1), align 4
  %NodeChild_LeafCount.addr = alloca ptr addrspace(1), align 4
  %leaves.addr = alloca ptr addrspace(1), align 4
  %metanodes.addr = alloca ptr addrspace(1), align 4
  %result.addr = alloca ptr addrspace(1), align 4
  %boundsCount = alloca i32, align 4
  %i = alloca i32, align 4
  %counts = alloca %struct.tree_counts, align 4
  %LeafCount = alloca i32, align 4
  %NodeCount = alloca i32, align 4
  %bounds = alloca %struct.BoundingBox, align 16
  %nodeIndex = alloca i32, align 4
  %newLeafCost = alloca float, align 4
  %child_a = alloca i32, align 4
  %child_b = alloca i32, align 4
  %choiceA = alloca %struct.InsertionChoice, align 16
  %choiceB = alloca %struct.InsertionChoice, align 16
  store ptr addrspace(1) %counts_ref, ptr %counts_ref.addr, align 4
  store ptr addrspace(1) %bounds_arr, ptr %bounds_arr.addr, align 4
  store ptr addrspace(1) %numBounds, ptr %numBounds.addr, align 4
  store ptr addrspace(1) %NodeChild_Min, ptr %NodeChild_Min.addr, align 4
  store ptr addrspace(1) %NodeChild_Max, ptr %NodeChild_Max.addr, align 4
  store ptr addrspace(1) %NodeChild_Index, ptr %NodeChild_Index.addr, align 4
  store ptr addrspace(1) %NodeChild_LeafCount, ptr %NodeChild_LeafCount.addr, align 4
  store ptr addrspace(1) %leaves, ptr %leaves.addr, align 4
  store ptr addrspace(1) %metanodes, ptr %metanodes.addr, align 4
  store ptr addrspace(1) %result, ptr %result.addr, align 4
  %0 = load ptr addrspace(1), ptr %numBounds.addr, align 4
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %0, i32 0
  %1 = load i32, ptr addrspace(1) %arrayidx, align 4
  store i32 %1, ptr %boundsCount, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load i32, ptr %i, align 4
  %3 = load i32, ptr %boundsCount, align 4
  %cmp = icmp slt i32 %2, %3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %4 = load ptr addrspace(1), ptr %counts_ref.addr, align 4
  %arrayidx1 = getelementptr inbounds %struct.tree_counts, ptr addrspace(1) %4, i32 0
  call void @llvm.memcpy.p0.p1.i32(ptr align 4 %counts, ptr addrspace(1) align 4 %arrayidx1, i32 8, i1 false)
  %LeafCount2 = getelementptr inbounds %struct.tree_counts, ptr %counts, i32 0, i32 1
  %5 = load i32, ptr %LeafCount2, align 4
  store i32 %5, ptr %LeafCount, align 4
  %NodeCount3 = getelementptr inbounds %struct.tree_counts, ptr %counts, i32 0, i32 0
  %6 = load i32, ptr %NodeCount3, align 4
  store i32 %6, ptr %NodeCount, align 4
  %7 = load ptr addrspace(1), ptr %bounds_arr.addr, align 4
  %8 = load i32, ptr %i, align 4
  %arrayidx4 = getelementptr inbounds %struct.BoundingBox, ptr addrspace(1) %7, i32 %8
  call void @llvm.memcpy.p0.p1.i32(ptr align 16 %bounds, ptr addrspace(1) align 16 %arrayidx4, i32 32, i1 false)
  %9 = load i32, ptr %LeafCount, align 4
  %cmp5 = icmp slt i32 %9, 2
  br i1 %cmp5, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %10 = load i32, ptr %LeafCount, align 4
  %11 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %12 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %13 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %14 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %15 = load ptr addrspace(1), ptr %counts_ref.addr, align 4
  %16 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %call = call spir_func i32 @InsertLeafIntoEmptySlot(ptr byval(%struct.BoundingBox) align 16 %bounds, i32 0, i32 %10, ptr addrspace(1) %11, ptr addrspace(1) %12, ptr addrspace(1) %13, ptr addrspace(1) %14, ptr addrspace(1) %15, ptr addrspace(1) %16) #5
  %17 = load ptr addrspace(1), ptr %result.addr, align 4
  %18 = load i32, ptr %i, align 4
  %arrayidx6 = getelementptr inbounds i32, ptr addrspace(1) %17, i32 %18
  store i32 %call, ptr addrspace(1) %arrayidx6, align 4
  br label %for.inc

if.end:                                           ; preds = %for.body
  store i32 0, ptr %nodeIndex, align 4
  %call7 = call spir_func float @ComputeBoundsMetric_1(ptr byval(%struct.BoundingBox) align 16 %bounds) #5
  store float %call7, ptr %newLeafCost, align 4
  br label %while.body

while.body:                                       ; preds = %if.end, %if.end52
  %19 = load i32, ptr %nodeIndex, align 4
  %call8 = call spir_func i32 @Node_GetChildIndex(i32 %19, i32 0) #5
  store i32 %call8, ptr %child_a, align 4
  %20 = load i32, ptr %nodeIndex, align 4
  %call9 = call spir_func i32 @Node_GetChildIndex(i32 %20, i32 1) #5
  store i32 %call9, ptr %child_b, align 4
  %21 = load float, ptr %newLeafCost, align 4
  %22 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %23 = load i32, ptr %child_a, align 4
  %arrayidx10 = getelementptr inbounds <4 x float>, ptr addrspace(1) %22, i32 %23
  %24 = load <4 x float>, ptr addrspace(1) %arrayidx10, align 16
  %25 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %26 = load i32, ptr %child_a, align 4
  %arrayidx11 = getelementptr inbounds <4 x float>, ptr addrspace(1) %25, i32 %26
  %27 = load <4 x float>, ptr addrspace(1) %arrayidx11, align 16
  %28 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %29 = load i32, ptr %child_a, align 4
  %arrayidx12 = getelementptr inbounds i32, ptr addrspace(1) %28, i32 %29
  %30 = load i32, ptr addrspace(1) %arrayidx12, align 4
  %31 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %32 = load i32, ptr %child_a, align 4
  %arrayidx13 = getelementptr inbounds i32, ptr addrspace(1) %31, i32 %32
  %33 = load i32, ptr addrspace(1) %arrayidx13, align 4
  call spir_func void @ComputeBestInsertionChoice(ptr sret(%struct.InsertionChoice) align 16 %choiceA, ptr byval(%struct.BoundingBox) align 16 %bounds, float %21, <4 x float> %24, <4 x float> %27, i32 %30, i32 %33) #5
  %34 = load float, ptr %newLeafCost, align 4
  %35 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %36 = load i32, ptr %child_b, align 4
  %arrayidx14 = getelementptr inbounds <4 x float>, ptr addrspace(1) %35, i32 %36
  %37 = load <4 x float>, ptr addrspace(1) %arrayidx14, align 16
  %38 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %39 = load i32, ptr %child_b, align 4
  %arrayidx15 = getelementptr inbounds <4 x float>, ptr addrspace(1) %38, i32 %39
  %40 = load <4 x float>, ptr addrspace(1) %arrayidx15, align 16
  %41 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %42 = load i32, ptr %child_b, align 4
  %arrayidx16 = getelementptr inbounds i32, ptr addrspace(1) %41, i32 %42
  %43 = load i32, ptr addrspace(1) %arrayidx16, align 4
  %44 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %45 = load i32, ptr %child_b, align 4
  %arrayidx17 = getelementptr inbounds i32, ptr addrspace(1) %44, i32 %45
  %46 = load i32, ptr addrspace(1) %arrayidx17, align 4
  call spir_func void @ComputeBestInsertionChoice(ptr sret(%struct.InsertionChoice) align 16 %choiceB, ptr byval(%struct.BoundingBox) align 16 %bounds, float %34, <4 x float> %37, <4 x float> %40, i32 %43, i32 %46) #5
  %CostChange = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 2
  %47 = load float, ptr %CostChange, align 16
  %CostChange18 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceB, i32 0, i32 2
  %48 = load float, ptr %CostChange18, align 16
  %cmp19 = fcmp ole float %47, %48
  br i1 %cmp19, label %if.then20, label %if.else33

if.then20:                                        ; preds = %while.body
  %Choice = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 0
  %49 = load i32, ptr %Choice, align 16
  %cmp21 = icmp eq i32 %49, 0
  br i1 %cmp21, label %if.then22, label %if.else

if.then22:                                        ; preds = %if.then20
  %50 = load i32, ptr %nodeIndex, align 4
  %MergedCandidate = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 1
  %51 = load ptr addrspace(1), ptr %counts_ref.addr, align 4
  %52 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %53 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %54 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %55 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %56 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %57 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %call23 = call spir_func i32 @MergeLeafNodes(ptr byval(%struct.BoundingBox) align 16 %bounds, i32 %50, i32 0, ptr byval(%struct.BoundingBox) align 16 %MergedCandidate, ptr addrspace(1) %51, ptr addrspace(1) %52, ptr addrspace(1) %53, ptr addrspace(1) %54, ptr addrspace(1) %55, ptr addrspace(1) %56, ptr addrspace(1) %57) #5
  %58 = load ptr addrspace(1), ptr %result.addr, align 4
  %59 = load i32, ptr %i, align 4
  %arrayidx24 = getelementptr inbounds i32, ptr addrspace(1) %58, i32 %59
  store i32 %call23, ptr addrspace(1) %arrayidx24, align 4
  br label %while.end

if.else:                                          ; preds = %if.then20
  %MergedCandidate25 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 1
  %Min = getelementptr inbounds %struct.BoundingBox, ptr %MergedCandidate25, i32 0, i32 0
  %60 = load <4 x float>, ptr %Min, align 16
  %61 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %62 = load i32, ptr %child_a, align 4
  %arrayidx26 = getelementptr inbounds <4 x float>, ptr addrspace(1) %61, i32 %62
  store <4 x float> %60, ptr addrspace(1) %arrayidx26, align 16
  %MergedCandidate27 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 1
  %Max = getelementptr inbounds %struct.BoundingBox, ptr %MergedCandidate27, i32 0, i32 1
  %63 = load <4 x float>, ptr %Max, align 16
  %64 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %65 = load i32, ptr %child_a, align 4
  %arrayidx28 = getelementptr inbounds <4 x float>, ptr addrspace(1) %64, i32 %65
  store <4 x float> %63, ptr addrspace(1) %arrayidx28, align 16
  %66 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %67 = load i32, ptr %child_a, align 4
  %arrayidx29 = getelementptr inbounds i32, ptr addrspace(1) %66, i32 %67
  %68 = load i32, ptr addrspace(1) %arrayidx29, align 4
  store i32 %68, ptr %nodeIndex, align 4
  %69 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %70 = load i32, ptr %child_a, align 4
  %arrayidx30 = getelementptr inbounds i32, ptr addrspace(1) %69, i32 %70
  %71 = load i32, ptr addrspace(1) %arrayidx30, align 4
  %add = add nsw i32 %71, 1
  %72 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %73 = load i32, ptr %child_a, align 4
  %arrayidx31 = getelementptr inbounds i32, ptr addrspace(1) %72, i32 %73
  store i32 %add, ptr addrspace(1) %arrayidx31, align 4
  br label %if.end32

if.end32:                                         ; preds = %if.else
  br label %if.end52

if.else33:                                        ; preds = %while.body
  %Choice34 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceA, i32 0, i32 0
  %74 = load i32, ptr %Choice34, align 16
  %cmp35 = icmp eq i32 %74, 0
  br i1 %cmp35, label %if.then36, label %if.else40

if.then36:                                        ; preds = %if.else33
  %75 = load i32, ptr %nodeIndex, align 4
  %MergedCandidate37 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceB, i32 0, i32 1
  %76 = load ptr addrspace(1), ptr %counts_ref.addr, align 4
  %77 = load ptr addrspace(1), ptr %metanodes.addr, align 4
  %78 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %79 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %80 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %81 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %82 = load ptr addrspace(1), ptr %leaves.addr, align 4
  %call38 = call spir_func i32 @MergeLeafNodes(ptr byval(%struct.BoundingBox) align 16 %bounds, i32 %75, i32 1, ptr byval(%struct.BoundingBox) align 16 %MergedCandidate37, ptr addrspace(1) %76, ptr addrspace(1) %77, ptr addrspace(1) %78, ptr addrspace(1) %79, ptr addrspace(1) %80, ptr addrspace(1) %81, ptr addrspace(1) %82) #5
  %83 = load ptr addrspace(1), ptr %result.addr, align 4
  %84 = load i32, ptr %i, align 4
  %arrayidx39 = getelementptr inbounds i32, ptr addrspace(1) %83, i32 %84
  store i32 %call38, ptr addrspace(1) %arrayidx39, align 4
  br label %while.end

if.else40:                                        ; preds = %if.else33
  %MergedCandidate41 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceB, i32 0, i32 1
  %Min42 = getelementptr inbounds %struct.BoundingBox, ptr %MergedCandidate41, i32 0, i32 0
  %85 = load <4 x float>, ptr %Min42, align 16
  %86 = load ptr addrspace(1), ptr %NodeChild_Min.addr, align 4
  %87 = load i32, ptr %child_b, align 4
  %arrayidx43 = getelementptr inbounds <4 x float>, ptr addrspace(1) %86, i32 %87
  store <4 x float> %85, ptr addrspace(1) %arrayidx43, align 16
  %MergedCandidate44 = getelementptr inbounds %struct.InsertionChoice, ptr %choiceB, i32 0, i32 1
  %Max45 = getelementptr inbounds %struct.BoundingBox, ptr %MergedCandidate44, i32 0, i32 1
  %88 = load <4 x float>, ptr %Max45, align 16
  %89 = load ptr addrspace(1), ptr %NodeChild_Max.addr, align 4
  %90 = load i32, ptr %child_b, align 4
  %arrayidx46 = getelementptr inbounds <4 x float>, ptr addrspace(1) %89, i32 %90
  store <4 x float> %88, ptr addrspace(1) %arrayidx46, align 16
  %91 = load ptr addrspace(1), ptr %NodeChild_Index.addr, align 4
  %92 = load i32, ptr %child_b, align 4
  %arrayidx47 = getelementptr inbounds i32, ptr addrspace(1) %91, i32 %92
  %93 = load i32, ptr addrspace(1) %arrayidx47, align 4
  store i32 %93, ptr %nodeIndex, align 4
  %94 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %95 = load i32, ptr %child_b, align 4
  %arrayidx48 = getelementptr inbounds i32, ptr addrspace(1) %94, i32 %95
  %96 = load i32, ptr addrspace(1) %arrayidx48, align 4
  %add49 = add nsw i32 %96, 1
  %97 = load ptr addrspace(1), ptr %NodeChild_LeafCount.addr, align 4
  %98 = load i32, ptr %child_b, align 4
  %arrayidx50 = getelementptr inbounds i32, ptr addrspace(1) %97, i32 %98
  store i32 %add49, ptr addrspace(1) %arrayidx50, align 4
  br label %if.end51

if.end51:                                         ; preds = %if.else40
  br label %if.end52

if.end52:                                         ; preds = %if.end51, %if.end32
  br label %while.body

while.end:                                        ; preds = %if.then36, %if.then22
  br label %for.inc

for.inc:                                          ; preds = %while.end, %if.then
  %99 = load i32, ptr %i, align 4
  %inc = add nsw i32 %99, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

attributes #0 = { convergent norecurse nounwind "no-builtins" "no-trapping-math"="true" "stack-protector-buffer-size"="0" "stackrealign" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { convergent norecurse nounwind "no-builtins" "no-trapping-math"="true" "stack-protector-buffer-size"="0" "stackrealign" "uniform-work-group-size"="true" }
attributes #3 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #4 = { convergent nounwind willreturn memory(none) "no-builtins" "no-trapping-math"="true" "stack-protector-buffer-size"="0" "stackrealign" }
attributes #5 = { convergent nobuiltin nounwind "no-builtins" }
attributes #6 = { convergent nobuiltin nounwind willreturn memory(none) "no-builtins" }

!llvm.module.flags = !{!0}
!opencl.ocl.version = !{!1}
!opencl.spir.version = !{!1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, i32 2}
!2 = !{!"clang version 17.0.0 (https://github.com/llvm/llvm-project fc78ebad051ac3e7564efc1a38a5e1faa8f30bf1)"}
!3 = !{i32 1, i32 1}
!4 = !{!"none", !"none"}
!5 = !{!"struct tree_counts*", !"struct metanode*"}
!6 = !{!"", !""}
!7 = !{i32 1, i32 1, i32 1}
!8 = !{i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1}
!9 = !{!"none", !"none", !"none", !"none", !"none", !"none", !"none", !"none", !"none", !"none"}
!10 = !{!"struct tree_counts*", !"struct BoundingBox*", !"int*", !"float4*", !"float4*", !"int*", !"int*", !"uint*", !"struct metanode*", !"int*"}
!11 = !{!"struct tree_counts*", !"struct BoundingBox*", !"int*", !"float __attribute__((ext_vector_type(4)))*", !"float __attribute__((ext_vector_type(4)))*", !"int*", !"int*", !"uint*", !"struct metanode*", !"int*"}
!12 = !{!"", !"", !"", !"", !"", !"", !"", !"", !"", !""}
