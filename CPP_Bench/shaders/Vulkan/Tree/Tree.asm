; SPIR-V
; Version: 1.0
; Generator: Khronos SPIR-V Tools Linker; 0
; Bound: 367
; Schema: 0
               OpCapability Shader
          %1 = OpExtInstImport "GLSL.std.450"
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %Init "Init"
               OpEntryPoint GLCompute %Add "Add"
               OpExecutionMode %Init LocalSize 1 1 1
               OpExecutionMode %Add LocalSize 1 1 1
               OpSource GLSL 450
               OpSourceExtension "GL_ARB_separate_shader_objects"
               OpSourceExtension "GL_GOOGLE_cpp_style_line_directive"
               OpSourceExtension "GL_GOOGLE_include_directive"
               OpSource GLSL 450
               OpSourceExtension "GL_ARB_separate_shader_objects"
               OpSourceExtension "GL_GOOGLE_cpp_style_line_directive"
               OpSourceExtension "GL_GOOGLE_include_directive"
               OpName %Init "Init"
               OpName %SetNodeCount_i1_ "SetNodeCount(i1;"
               OpName %cnt "cnt"
               OpName %SetLeafCount_i1_ "SetLeafCount(i1;"
               OpName %cnt_0 "cnt"
               OpName %Set_Metanode_Parent_i1_i1_ "Set_Metanode_Parent(i1;i1;"
               OpName %index "index"
               OpName %value "value"
               OpName %Set_Metanode_IndexInParent_i1_i1_ "Set_Metanode_IndexInParent(i1;i1;"
               OpName %index_0 "index"
               OpName %value_0 "value"
               OpName %g_counts_InOut "g_counts_InOut"
               OpMemberName %g_counts_InOut 0 "g_counts"
               OpName %_ ""
               OpName %__0 ""
               OpName %param "param"
               OpName %param_0 "param"
               OpName %param_1 "param"
               OpName %param_2 "param"
               OpName %param_3 "param"
               OpName %param_4 "param"
               OpName %g_leaves_InOut "g_leaves_InOut"
               OpMemberName %g_leaves_InOut 0 "g_leaves"
               OpName %__1 ""
               OpName %g_NodeChild_Min_InOut "g_NodeChild_Min_InOut"
               OpMemberName %g_NodeChild_Min_InOut 0 "g_NodeChild_Min"
               OpName %__2 ""
               OpName %__3 ""
               OpName %__4 ""
               OpName %__5 ""
               OpName %Add "Add"
               OpName %Node_GetChildIndex_i1_i1_ "Node_GetChildIndex(i1;i1;"
               OpName %index_1 "index"
               OpName %child "child"
               OpName %Encode_i1_ "Encode(i1;"
               OpName %index_2 "index"
               OpName %Leaf_New_u1_u1_ "Leaf_New(u1;u1;"
               OpName %nodeIndex "nodeIndex"
               OpName %childIndex "childIndex"
               OpName %GetNodeCount_ "GetNodeCount("
               OpName %GetLeafCount_ "GetLeafCount("
               OpName %SetLeafCount_i1__0 "SetLeafCount(i1;"
               OpName %cnt_1 "cnt"
               OpName %SetLeaf_i1_u1_ "SetLeaf(i1;u1;"
               OpName %index_3 "index"
               OpName %leaf "leaf"
               OpName %Set_NodeChild_Min_i1_vf4_ "Set_NodeChild_Min(i1;vf4;"
               OpName %c_index "c_index"
               OpName %val "val"
               OpName %Set_NodeChild_Max_i1_vf4_ "Set_NodeChild_Max(i1;vf4;"
               OpName %c_index_0 "c_index"
               OpName %val_0 "val"
               OpName %Set_NodeChild_Index_i1_i1_ "Set_NodeChild_Index(i1;i1;"
               OpName %c_index_1 "c_index"
               OpName %val_1 "val"
               OpName %Set_NodeChild_LeafCount_i1_i1_ "Set_NodeChild_LeafCount(i1;i1;"
               OpName %c_index_2 "c_index"
               OpName %val_2 "val"
               OpName %AddLeaf_i1_i1_ "AddLeaf(i1;i1;"
               OpName %nodeIndex_0 "nodeIndex"
               OpName %childIndex_0 "childIndex"
               OpName %BoundingBox "BoundingBox"
               OpMemberName %BoundingBox 0 "Min"
               OpMemberName %BoundingBox 1 "Max"
               OpName %InsertLeafIntoEmptySlot_struct_BoundingBox_vf4_vf41_i1_i1_ "InsertLeafIntoEmptySlot(struct-BoundingBox-vf4-vf41;i1;i1;"
               OpName %leafBox "leafBox"
               OpName %nodeIndex_1 "nodeIndex"
               OpName %childIndex_1 "childIndex"
               OpName %ComputeBoundsMetric_2_vf4_vf4_ "ComputeBoundsMetric_2(vf4;vf4;"
               OpName %min "min"
               OpName %max "max"
               OpName %ComputeBoundsMetric_1_struct_BoundingBox_vf4_vf41_ "ComputeBoundsMetric_1(struct-BoundingBox-vf4-vf41;"
               OpName %bounds "bounds"
               OpName %SetResult_i1_i1_ "SetResult(i1;i1;"
               OpName %index_4 "index"
               OpName %val_3 "val"
               OpName %DoAdd_struct_BoundingBox_vf4_vf41_ "DoAdd(struct-BoundingBox-vf4-vf41;"
               OpName %bounds_0 "bounds"
               OpName %pck_index "pck_index"
               OpName %pck_child "pck_child"
               OpName %__6 ""
               OpName %__7 ""
               OpName %__8 ""
               OpName %__9 ""
               OpName %__10 ""
               OpName %__11 ""
               OpName %count "count"
               OpName %param_5 "param"
               OpName %param_6 "param"
               OpName %param_7 "param"
               OpName %param_8 "param"
               OpName %param_9 "param"
               OpName %leafIndex "leafIndex"
               OpName %param_10 "param"
               OpName %param_11 "param"
               OpName %child_0 "child"
               OpName %param_12 "param"
               OpName %param_13 "param"
               OpName %param_14 "param"
               OpName %param_15 "param"
               OpName %param_16 "param"
               OpName %param_17 "param"
               OpName %param_18 "param"
               OpName %param_19 "param"
               OpName %param_20 "param"
               OpName %param_21 "param"
               OpName %param_22 "param"
               OpName %offset "offset"
               OpName %param_23 "param"
               OpName %param_24 "param"
               OpName %__12 ""
               OpName %LeafCount "LeafCount"
               OpName %NodeCount "NodeCount"
               OpName %res "res"
               OpName %param_25 "param"
               OpName %param_26 "param"
               OpName %param_27 "param"
               OpName %nodeIndex_2 "nodeIndex"
               OpName %newLeafCost "newLeafCost"
               OpName %param_28 "param"
               OpName %depth "depth"
               OpName %boundsCount "boundsCount"
               OpName %numBounds_In "numBounds_In"
               OpMemberName %numBounds_In 0 "g_numBounds"
               OpName %__13 ""
               OpName %i "i"
               OpName %res_0 "res"
               OpName %BoundingBox_0 "BoundingBox"
               OpMemberName %BoundingBox_0 0 "Min"
               OpMemberName %BoundingBox_0 1 "Max"
               OpName %bounds_arr_In "bounds_arr_In"
               OpMemberName %bounds_arr_In 0 "g_bounds_arr"
               OpName %__14 ""
               OpName %param_29 "param"
               OpName %param_30 "param"
               OpName %param_31 "param"
               OpName %__15 ""
               OpDecorate %_runtimearr_v4int ArrayStride 16
               OpMemberDecorate %g_counts_InOut 0 Offset 0
               OpDecorate %g_counts_InOut BufferBlock
               OpDecorate %_ DescriptorSet 0
               OpDecorate %_ Binding 0
               OpDecorate %__0 DescriptorSet 0
               OpDecorate %__0 Binding 1
               OpDecorate %_runtimearr_v4uint ArrayStride 16
               OpMemberDecorate %g_leaves_InOut 0 Offset 0
               OpDecorate %g_leaves_InOut BufferBlock
               OpDecorate %__1 DescriptorSet 0
               OpDecorate %__1 Binding 2
               OpDecorate %_runtimearr_v4float ArrayStride 16
               OpMemberDecorate %g_NodeChild_Min_InOut 0 Offset 0
               OpDecorate %g_NodeChild_Min_InOut BufferBlock
               OpDecorate %__2 DescriptorSet 0
               OpDecorate %__2 Binding 3
               OpDecorate %__3 DescriptorSet 0
               OpDecorate %__3 Binding 4
               OpDecorate %__4 DescriptorSet 0
               OpDecorate %__4 Binding 5
               OpDecorate %__5 DescriptorSet 0
               OpDecorate %__5 Binding 6
               OpDecorate %gl_WorkGroupSize BuiltIn WorkgroupSize
               OpDecorate %__6 DescriptorSet 0
               OpDecorate %__6 Binding 0
               OpDecorate %__7 DescriptorSet 0
               OpDecorate %__7 Binding 2
               OpDecorate %__8 DescriptorSet 0
               OpDecorate %__8 Binding 3
               OpDecorate %__9 DescriptorSet 0
               OpDecorate %__9 Binding 4
               OpDecorate %__10 DescriptorSet 0
               OpDecorate %__10 Binding 5
               OpDecorate %__11 DescriptorSet 0
               OpDecorate %__11 Binding 6
               OpDecorate %__12 DescriptorSet 0
               OpDecorate %__12 Binding 9
               OpMemberDecorate %numBounds_In 0 NonWritable
               OpMemberDecorate %numBounds_In 0 Offset 0
               OpDecorate %numBounds_In BufferBlock
               OpDecorate %__13 DescriptorSet 0
               OpDecorate %__13 Binding 7
               OpMemberDecorate %BoundingBox_0 0 Offset 0
               OpMemberDecorate %BoundingBox_0 1 Offset 16
               OpDecorate %_runtimearr_BoundingBox_0 ArrayStride 32
               OpMemberDecorate %bounds_arr_In 0 NonWritable
               OpMemberDecorate %bounds_arr_In 0 Offset 0
               OpDecorate %bounds_arr_In BufferBlock
               OpDecorate %__14 DescriptorSet 0
               OpDecorate %__14 Binding 8
               OpDecorate %__15 DescriptorSet 0
               OpDecorate %__15 Binding 1
               OpDecorate %gl_WorkGroupSize_0 BuiltIn WorkgroupSize
       %void = OpTypeVoid
        %137 = OpTypeFunction %void
        %int = OpTypeInt 32 1
%_ptr_Function_int = OpTypePointer Function %int
        %140 = OpTypeFunction %void %_ptr_Function_int
        %141 = OpTypeFunction %void %_ptr_Function_int %_ptr_Function_int
      %v4int = OpTypeVector %int 4
%_runtimearr_v4int = OpTypeRuntimeArray %v4int
%g_counts_InOut = OpTypeStruct %_runtimearr_v4int
%_ptr_Uniform_g_counts_InOut = OpTypePointer Uniform %g_counts_InOut
          %_ = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
      %int_0 = OpConstant %int 0
       %uint = OpTypeInt 32 0
     %uint_0 = OpConstant %uint 0
%_ptr_Uniform_int = OpTypePointer Uniform %int
     %uint_1 = OpConstant %uint 1
        %__0 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
      %int_1 = OpConstant %int 1
     %int_n1 = OpConstant %int -1
     %v4uint = OpTypeVector %uint 4
%_runtimearr_v4uint = OpTypeRuntimeArray %v4uint
%g_leaves_InOut = OpTypeStruct %_runtimearr_v4uint
%_ptr_Uniform_g_leaves_InOut = OpTypePointer Uniform %g_leaves_InOut
        %__1 = OpVariable %_ptr_Uniform_g_leaves_InOut Uniform
      %float = OpTypeFloat 32
    %v4float = OpTypeVector %float 4
%_runtimearr_v4float = OpTypeRuntimeArray %v4float
%g_NodeChild_Min_InOut = OpTypeStruct %_runtimearr_v4float
%_ptr_Uniform_g_NodeChild_Min_InOut = OpTypePointer Uniform %g_NodeChild_Min_InOut
        %__2 = OpVariable %_ptr_Uniform_g_NodeChild_Min_InOut Uniform
        %__3 = OpVariable %_ptr_Uniform_g_NodeChild_Min_InOut Uniform
        %__4 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
        %__5 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
     %v3uint = OpTypeVector %uint 3
%gl_WorkGroupSize = OpConstantComposite %v3uint %uint_1 %uint_1 %uint_1
        %157 = OpTypeFunction %int %_ptr_Function_int %_ptr_Function_int
        %158 = OpTypeFunction %int %_ptr_Function_int
%_ptr_Function_uint = OpTypePointer Function %uint
        %160 = OpTypeFunction %uint %_ptr_Function_uint %_ptr_Function_uint
        %161 = OpTypeFunction %int
        %162 = OpTypeFunction %void %_ptr_Function_int %_ptr_Function_uint
%_ptr_Function_v4float = OpTypePointer Function %v4float
        %164 = OpTypeFunction %void %_ptr_Function_int %_ptr_Function_v4float
%BoundingBox = OpTypeStruct %v4float %v4float
%_ptr_Function_BoundingBox = OpTypePointer Function %BoundingBox
        %166 = OpTypeFunction %int %_ptr_Function_BoundingBox %_ptr_Function_int %_ptr_Function_int
        %167 = OpTypeFunction %float %_ptr_Function_v4float %_ptr_Function_v4float
        %168 = OpTypeFunction %float %_ptr_Function_BoundingBox
        %169 = OpTypeFunction %int %_ptr_Function_BoundingBox
      %int_2 = OpConstant %int 2
   %int_n1_0 = OpConstant %int -1
%uint_2147483647 = OpConstant %uint 2147483647
     %int_31 = OpConstant %int 31
        %__6 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
    %int_0_0 = OpConstant %int 0
   %uint_0_0 = OpConstant %uint 0
   %uint_1_0 = OpConstant %uint 1
        %__7 = OpVariable %_ptr_Uniform_g_leaves_InOut Uniform
      %int_4 = OpConstant %int 4
%_ptr_Uniform_uint = OpTypePointer Uniform %uint
        %__8 = OpVariable %_ptr_Uniform_g_NodeChild_Min_InOut Uniform
%_ptr_Uniform_v4float = OpTypePointer Uniform %v4float
        %__9 = OpVariable %_ptr_Uniform_g_NodeChild_Min_InOut Uniform
       %__10 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
       %__11 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
    %int_1_0 = OpConstant %int 1
%_ptr_Function_float = OpTypePointer Function %float
     %uint_2 = OpConstant %uint 2
       %__12 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
       %bool = OpTypeBool
%numBounds_In = OpTypeStruct %_runtimearr_v4int
%_ptr_Uniform_numBounds_In = OpTypePointer Uniform %numBounds_In
       %__13 = OpVariable %_ptr_Uniform_numBounds_In Uniform
%BoundingBox_0 = OpTypeStruct %v4float %v4float
%_runtimearr_BoundingBox_0 = OpTypeRuntimeArray %BoundingBox_0
%bounds_arr_In = OpTypeStruct %_runtimearr_BoundingBox_0
%_ptr_Uniform_bounds_arr_In = OpTypePointer Uniform %bounds_arr_In
       %__14 = OpVariable %_ptr_Uniform_bounds_arr_In Uniform
%_ptr_Uniform_BoundingBox_0 = OpTypePointer Uniform %BoundingBox_0
       %__15 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
%gl_WorkGroupSize_0 = OpConstantComposite %v3uint %uint_1_0 %uint_1_0 %uint_1_0
       %Init = OpFunction %void None %137
        %187 = OpLabel
      %param = OpVariable %_ptr_Function_int Function
    %param_0 = OpVariable %_ptr_Function_int Function
    %param_1 = OpVariable %_ptr_Function_int Function
    %param_2 = OpVariable %_ptr_Function_int Function
    %param_3 = OpVariable %_ptr_Function_int Function
    %param_4 = OpVariable %_ptr_Function_int Function
               OpStore %param %int_1
        %188 = OpFunctionCall %void %SetNodeCount_i1_ %param
               OpStore %param_0 %int_0
        %189 = OpFunctionCall %void %SetLeafCount_i1_ %param_0
               OpStore %param_1 %int_0
               OpStore %param_2 %int_n1
        %190 = OpFunctionCall %void %Set_Metanode_Parent_i1_i1_ %param_1 %param_2
               OpStore %param_3 %int_0
               OpStore %param_4 %int_n1
        %191 = OpFunctionCall %void %Set_Metanode_IndexInParent_i1_i1_ %param_3 %param_4
               OpReturn
               OpFunctionEnd
%SetNodeCount_i1_ = OpFunction %void None %140
        %cnt = OpFunctionParameter %_ptr_Function_int
        %192 = OpLabel
        %193 = OpLoad %int %cnt
        %194 = OpAccessChain %_ptr_Uniform_int %_ %int_0 %int_0 %uint_0
               OpStore %194 %193
               OpReturn
               OpFunctionEnd
%SetLeafCount_i1_ = OpFunction %void None %140
      %cnt_0 = OpFunctionParameter %_ptr_Function_int
        %195 = OpLabel
        %196 = OpLoad %int %cnt_0
        %197 = OpAccessChain %_ptr_Uniform_int %_ %int_0 %int_0 %uint_1
               OpStore %197 %196
               OpReturn
               OpFunctionEnd
%Set_Metanode_Parent_i1_i1_ = OpFunction %void None %141
      %index = OpFunctionParameter %_ptr_Function_int
      %value = OpFunctionParameter %_ptr_Function_int
        %198 = OpLabel
        %199 = OpLoad %int %index
        %200 = OpLoad %int %value
        %201 = OpAccessChain %_ptr_Uniform_int %__0 %int_0 %199 %uint_0
               OpStore %201 %200
               OpReturn
               OpFunctionEnd
%Set_Metanode_IndexInParent_i1_i1_ = OpFunction %void None %141
    %index_0 = OpFunctionParameter %_ptr_Function_int
    %value_0 = OpFunctionParameter %_ptr_Function_int
        %202 = OpLabel
        %203 = OpLoad %int %index_0
        %204 = OpLoad %int %value_0
        %205 = OpAccessChain %_ptr_Uniform_int %__0 %int_0 %203 %uint_1
               OpStore %205 %204
               OpReturn
               OpFunctionEnd
        %Add = OpFunction %void None %137
        %206 = OpLabel
%boundsCount = OpVariable %_ptr_Function_int Function
          %i = OpVariable %_ptr_Function_int Function
      %res_0 = OpVariable %_ptr_Function_int Function
   %param_29 = OpVariable %_ptr_Function_BoundingBox Function
   %param_30 = OpVariable %_ptr_Function_int Function
   %param_31 = OpVariable %_ptr_Function_int Function
        %207 = OpAccessChain %_ptr_Uniform_int %__13 %int_0_0 %int_0_0 %uint_0_0
        %208 = OpLoad %int %207
               OpStore %boundsCount %208
               OpStore %i %int_0_0
               OpBranch %209
        %209 = OpLabel
               OpLoopMerge %210 %211 None
               OpBranch %212
        %212 = OpLabel
        %213 = OpLoad %int %i
        %214 = OpLoad %int %boundsCount
        %215 = OpSLessThan %bool %213 %214
               OpBranchConditional %215 %216 %210
        %216 = OpLabel
        %217 = OpLoad %int %i
        %218 = OpAccessChain %_ptr_Uniform_BoundingBox_0 %__14 %int_0_0 %217
        %219 = OpLoad %BoundingBox_0 %218
        %220 = OpCompositeExtract %v4float %219 0
        %221 = OpAccessChain %_ptr_Function_v4float %param_29 %int_0_0
               OpStore %221 %220
        %222 = OpCompositeExtract %v4float %219 1
        %223 = OpAccessChain %_ptr_Function_v4float %param_29 %int_1_0
               OpStore %223 %222
        %224 = OpFunctionCall %int %DoAdd_struct_BoundingBox_vf4_vf41_ %param_29
               OpStore %res_0 %224
        %225 = OpLoad %int %i
               OpStore %param_30 %225
        %226 = OpLoad %int %res_0
               OpStore %param_31 %226
        %227 = OpFunctionCall %void %SetResult_i1_i1_ %param_30 %param_31
               OpBranch %211
        %211 = OpLabel
        %228 = OpLoad %int %i
        %229 = OpIAdd %int %228 %int_1_0
               OpStore %i %229
               OpBranch %209
        %210 = OpLabel
               OpReturn
               OpFunctionEnd
%Node_GetChildIndex_i1_i1_ = OpFunction %int None %157
    %index_1 = OpFunctionParameter %_ptr_Function_int
      %child = OpFunctionParameter %_ptr_Function_int
        %230 = OpLabel
        %231 = OpLoad %int %index_1
        %232 = OpIMul %int %231 %int_2
        %233 = OpLoad %int %child
        %234 = OpIAdd %int %232 %233
               OpReturnValue %234
               OpFunctionEnd
 %Encode_i1_ = OpFunction %int None %158
    %index_2 = OpFunctionParameter %_ptr_Function_int
        %235 = OpLabel
        %236 = OpLoad %int %index_2
        %237 = OpISub %int %int_n1_0 %236
               OpReturnValue %237
               OpFunctionEnd
%Leaf_New_u1_u1_ = OpFunction %uint None %160
  %nodeIndex = OpFunctionParameter %_ptr_Function_uint
 %childIndex = OpFunctionParameter %_ptr_Function_uint
        %238 = OpLabel
  %pck_index = OpVariable %_ptr_Function_uint Function
  %pck_child = OpVariable %_ptr_Function_uint Function
        %239 = OpLoad %uint %nodeIndex
        %240 = OpBitwiseAnd %uint %239 %uint_2147483647
               OpStore %pck_index %240
        %241 = OpLoad %uint %childIndex
        %242 = OpShiftLeftLogical %uint %241 %int_31
               OpStore %pck_child %242
        %243 = OpLoad %uint %pck_index
        %244 = OpLoad %uint %pck_child
        %245 = OpBitwiseOr %uint %243 %244
               OpReturnValue %245
               OpFunctionEnd
%GetNodeCount_ = OpFunction %int None %161
        %246 = OpLabel
        %247 = OpAccessChain %_ptr_Uniform_int %__6 %int_0_0 %int_0_0 %uint_0_0
        %248 = OpLoad %int %247
               OpReturnValue %248
               OpFunctionEnd
%GetLeafCount_ = OpFunction %int None %161
        %249 = OpLabel
        %250 = OpAccessChain %_ptr_Uniform_int %__6 %int_0_0 %int_0_0 %uint_1_0
        %251 = OpLoad %int %250
               OpReturnValue %251
               OpFunctionEnd
%SetLeafCount_i1__0 = OpFunction %void None %140
      %cnt_1 = OpFunctionParameter %_ptr_Function_int
        %252 = OpLabel
        %253 = OpLoad %int %cnt_1
        %254 = OpAccessChain %_ptr_Uniform_int %__6 %int_0_0 %int_0_0 %uint_1_0
               OpStore %254 %253
               OpReturn
               OpFunctionEnd
%SetLeaf_i1_u1_ = OpFunction %void None %162
    %index_3 = OpFunctionParameter %_ptr_Function_int
       %leaf = OpFunctionParameter %_ptr_Function_uint
        %255 = OpLabel
        %256 = OpLoad %int %index_3
        %257 = OpSDiv %int %256 %int_4
        %258 = OpLoad %int %index_3
        %259 = OpSMod %int %258 %int_4
        %260 = OpLoad %uint %leaf
        %261 = OpAccessChain %_ptr_Uniform_uint %__7 %int_0_0 %257 %259
               OpStore %261 %260
               OpReturn
               OpFunctionEnd
%Set_NodeChild_Min_i1_vf4_ = OpFunction %void None %164
    %c_index = OpFunctionParameter %_ptr_Function_int
        %val = OpFunctionParameter %_ptr_Function_v4float
        %262 = OpLabel
        %263 = OpLoad %int %c_index
        %264 = OpLoad %v4float %val
        %265 = OpAccessChain %_ptr_Uniform_v4float %__8 %int_0_0 %263
               OpStore %265 %264
               OpReturn
               OpFunctionEnd
%Set_NodeChild_Max_i1_vf4_ = OpFunction %void None %164
  %c_index_0 = OpFunctionParameter %_ptr_Function_int
      %val_0 = OpFunctionParameter %_ptr_Function_v4float
        %266 = OpLabel
        %267 = OpLoad %int %c_index_0
        %268 = OpLoad %v4float %val_0
        %269 = OpAccessChain %_ptr_Uniform_v4float %__9 %int_0_0 %267
               OpStore %269 %268
               OpReturn
               OpFunctionEnd
%Set_NodeChild_Index_i1_i1_ = OpFunction %void None %141
  %c_index_1 = OpFunctionParameter %_ptr_Function_int
      %val_1 = OpFunctionParameter %_ptr_Function_int
        %270 = OpLabel
        %271 = OpLoad %int %c_index_1
        %272 = OpSDiv %int %271 %int_4
        %273 = OpLoad %int %c_index_1
        %274 = OpSMod %int %273 %int_4
        %275 = OpLoad %int %val_1
        %276 = OpAccessChain %_ptr_Uniform_int %__10 %int_0_0 %272 %274
               OpStore %276 %275
               OpReturn
               OpFunctionEnd
%Set_NodeChild_LeafCount_i1_i1_ = OpFunction %void None %141
  %c_index_2 = OpFunctionParameter %_ptr_Function_int
      %val_2 = OpFunctionParameter %_ptr_Function_int
        %277 = OpLabel
        %278 = OpLoad %int %c_index_2
        %279 = OpSDiv %int %278 %int_4
        %280 = OpLoad %int %c_index_2
        %281 = OpSMod %int %280 %int_4
        %282 = OpLoad %int %val_2
        %283 = OpAccessChain %_ptr_Uniform_int %__11 %int_0_0 %279 %281
               OpStore %283 %282
               OpReturn
               OpFunctionEnd
%AddLeaf_i1_i1_ = OpFunction %int None %157
%nodeIndex_0 = OpFunctionParameter %_ptr_Function_int
%childIndex_0 = OpFunctionParameter %_ptr_Function_int
        %284 = OpLabel
      %count = OpVariable %_ptr_Function_int Function
    %param_5 = OpVariable %_ptr_Function_uint Function
    %param_6 = OpVariable %_ptr_Function_uint Function
    %param_7 = OpVariable %_ptr_Function_int Function
    %param_8 = OpVariable %_ptr_Function_uint Function
    %param_9 = OpVariable %_ptr_Function_int Function
        %285 = OpFunctionCall %int %GetLeafCount_
               OpStore %count %285
        %286 = OpLoad %int %nodeIndex_0
        %287 = OpBitcast %uint %286
        %288 = OpLoad %int %childIndex_0
        %289 = OpBitcast %uint %288
               OpStore %param_5 %287
               OpStore %param_6 %289
        %290 = OpFunctionCall %uint %Leaf_New_u1_u1_ %param_5 %param_6
        %291 = OpLoad %int %count
               OpStore %param_7 %291
               OpStore %param_8 %290
        %292 = OpFunctionCall %void %SetLeaf_i1_u1_ %param_7 %param_8
        %293 = OpLoad %int %count
        %294 = OpIAdd %int %293 %int_1_0
               OpStore %param_9 %294
        %295 = OpFunctionCall %void %SetLeafCount_i1__0 %param_9
        %296 = OpLoad %int %count
               OpReturnValue %296
               OpFunctionEnd
%InsertLeafIntoEmptySlot_struct_BoundingBox_vf4_vf41_i1_i1_ = OpFunction %int None %166
    %leafBox = OpFunctionParameter %_ptr_Function_BoundingBox
%nodeIndex_1 = OpFunctionParameter %_ptr_Function_int
%childIndex_1 = OpFunctionParameter %_ptr_Function_int
        %297 = OpLabel
  %leafIndex = OpVariable %_ptr_Function_int Function
   %param_10 = OpVariable %_ptr_Function_int Function
   %param_11 = OpVariable %_ptr_Function_int Function
    %child_0 = OpVariable %_ptr_Function_int Function
   %param_12 = OpVariable %_ptr_Function_int Function
   %param_13 = OpVariable %_ptr_Function_int Function
   %param_14 = OpVariable %_ptr_Function_int Function
   %param_15 = OpVariable %_ptr_Function_v4float Function
   %param_16 = OpVariable %_ptr_Function_int Function
   %param_17 = OpVariable %_ptr_Function_v4float Function
   %param_18 = OpVariable %_ptr_Function_int Function
   %param_19 = OpVariable %_ptr_Function_int Function
   %param_20 = OpVariable %_ptr_Function_int Function
   %param_21 = OpVariable %_ptr_Function_int Function
   %param_22 = OpVariable %_ptr_Function_int Function
        %298 = OpLoad %int %nodeIndex_1
               OpStore %param_10 %298
        %299 = OpLoad %int %childIndex_1
               OpStore %param_11 %299
        %300 = OpFunctionCall %int %AddLeaf_i1_i1_ %param_10 %param_11
               OpStore %leafIndex %300
        %301 = OpLoad %int %nodeIndex_1
               OpStore %param_12 %301
        %302 = OpLoad %int %childIndex_1
               OpStore %param_13 %302
        %303 = OpFunctionCall %int %Node_GetChildIndex_i1_i1_ %param_12 %param_13
               OpStore %child_0 %303
        %304 = OpLoad %int %child_0
               OpStore %param_14 %304
        %305 = OpAccessChain %_ptr_Function_v4float %leafBox %int_0_0
        %306 = OpLoad %v4float %305
               OpStore %param_15 %306
        %307 = OpFunctionCall %void %Set_NodeChild_Min_i1_vf4_ %param_14 %param_15
        %308 = OpLoad %int %child_0
               OpStore %param_16 %308
        %309 = OpAccessChain %_ptr_Function_v4float %leafBox %int_1_0
        %310 = OpLoad %v4float %309
               OpStore %param_17 %310
        %311 = OpFunctionCall %void %Set_NodeChild_Max_i1_vf4_ %param_16 %param_17
        %312 = OpLoad %int %leafIndex
               OpStore %param_18 %312
        %313 = OpFunctionCall %int %Encode_i1_ %param_18
        %314 = OpLoad %int %child_0
               OpStore %param_19 %314
               OpStore %param_20 %313
        %315 = OpFunctionCall %void %Set_NodeChild_Index_i1_i1_ %param_19 %param_20
        %316 = OpLoad %int %child_0
               OpStore %param_21 %316
               OpStore %param_22 %int_1_0
        %317 = OpFunctionCall %void %Set_NodeChild_LeafCount_i1_i1_ %param_21 %param_22
        %318 = OpLoad %int %leafIndex
               OpReturnValue %318
               OpFunctionEnd
%ComputeBoundsMetric_2_vf4_vf4_ = OpFunction %float None %167
        %min = OpFunctionParameter %_ptr_Function_v4float
        %max = OpFunctionParameter %_ptr_Function_v4float
        %319 = OpLabel
     %offset = OpVariable %_ptr_Function_v4float Function
        %320 = OpLoad %v4float %max
        %321 = OpLoad %v4float %min
        %322 = OpFSub %v4float %320 %321
               OpStore %offset %322
        %323 = OpAccessChain %_ptr_Function_float %offset %uint_0_0
        %324 = OpLoad %float %323
        %325 = OpAccessChain %_ptr_Function_float %offset %uint_1_0
        %326 = OpLoad %float %325
        %327 = OpFMul %float %324 %326
        %328 = OpAccessChain %_ptr_Function_float %offset %uint_1_0
        %329 = OpLoad %float %328
        %330 = OpAccessChain %_ptr_Function_float %offset %uint_2
        %331 = OpLoad %float %330
        %332 = OpFMul %float %329 %331
        %333 = OpFAdd %float %327 %332
        %334 = OpAccessChain %_ptr_Function_float %offset %uint_0_0
        %335 = OpLoad %float %334
        %336 = OpAccessChain %_ptr_Function_float %offset %uint_2
        %337 = OpLoad %float %336
        %338 = OpFMul %float %335 %337
        %339 = OpFAdd %float %333 %338
               OpReturnValue %339
               OpFunctionEnd
%ComputeBoundsMetric_1_struct_BoundingBox_vf4_vf41_ = OpFunction %float None %168
     %bounds = OpFunctionParameter %_ptr_Function_BoundingBox
        %340 = OpLabel
   %param_23 = OpVariable %_ptr_Function_v4float Function
   %param_24 = OpVariable %_ptr_Function_v4float Function
        %341 = OpAccessChain %_ptr_Function_v4float %bounds %int_0_0
        %342 = OpLoad %v4float %341
               OpStore %param_23 %342
        %343 = OpAccessChain %_ptr_Function_v4float %bounds %int_1_0
        %344 = OpLoad %v4float %343
               OpStore %param_24 %344
        %345 = OpFunctionCall %float %ComputeBoundsMetric_2_vf4_vf4_ %param_23 %param_24
               OpReturnValue %345
               OpFunctionEnd
%SetResult_i1_i1_ = OpFunction %void None %141
    %index_4 = OpFunctionParameter %_ptr_Function_int
      %val_3 = OpFunctionParameter %_ptr_Function_int
        %346 = OpLabel
        %347 = OpLoad %int %index_4
        %348 = OpSDiv %int %347 %int_4
        %349 = OpLoad %int %index_4
        %350 = OpSMod %int %349 %int_4
        %351 = OpLoad %int %val_3
        %352 = OpAccessChain %_ptr_Uniform_int %__12 %int_0_0 %348 %350
               OpStore %352 %351
               OpReturn
               OpFunctionEnd
%DoAdd_struct_BoundingBox_vf4_vf41_ = OpFunction %int None %169
   %bounds_0 = OpFunctionParameter %_ptr_Function_BoundingBox
        %353 = OpLabel
  %LeafCount = OpVariable %_ptr_Function_int Function
  %NodeCount = OpVariable %_ptr_Function_int Function
        %res = OpVariable %_ptr_Function_int Function
   %param_25 = OpVariable %_ptr_Function_BoundingBox Function
   %param_26 = OpVariable %_ptr_Function_int Function
   %param_27 = OpVariable %_ptr_Function_int Function
%nodeIndex_2 = OpVariable %_ptr_Function_int Function
%newLeafCost = OpVariable %_ptr_Function_float Function
   %param_28 = OpVariable %_ptr_Function_BoundingBox Function
      %depth = OpVariable %_ptr_Function_int Function
        %354 = OpFunctionCall %int %GetLeafCount_
               OpStore %LeafCount %354
        %355 = OpFunctionCall %int %GetNodeCount_
               OpStore %NodeCount %355
               OpStore %res %int_n1_0
        %356 = OpLoad %int %LeafCount
        %357 = OpSLessThan %bool %356 %int_2
               OpSelectionMerge %358 None
               OpBranchConditional %357 %359 %358
        %359 = OpLabel
        %360 = OpLoad %BoundingBox %bounds_0
               OpStore %param_25 %360
               OpStore %param_26 %int_0_0
        %361 = OpLoad %int %LeafCount
               OpStore %param_27 %361
        %362 = OpFunctionCall %int %InsertLeafIntoEmptySlot_struct_BoundingBox_vf4_vf41_i1_i1_ %param_25 %param_26 %param_27
               OpStore %res %362
        %363 = OpLoad %int %res
               OpReturnValue %363
        %358 = OpLabel
               OpStore %nodeIndex_2 %int_0_0
        %364 = OpLoad %BoundingBox %bounds_0
               OpStore %param_28 %364
        %365 = OpFunctionCall %float %ComputeBoundsMetric_1_struct_BoundingBox_vf4_vf41_ %param_28
               OpStore %newLeafCost %365
               OpStore %depth %int_0_0
        %366 = OpLoad %int %res
               OpReturnValue %366
               OpFunctionEnd
