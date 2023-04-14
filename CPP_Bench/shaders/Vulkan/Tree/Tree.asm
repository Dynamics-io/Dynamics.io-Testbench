; SPIR-V
; Version: 1.0
; Generator: Khronos SPIR-V Tools Linker; 0
; Bound: 80
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
               OpName %__6 ""
               OpName %__7 ""
               OpName %__8 ""
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
               OpDecorate %__7 Binding 1
               OpDecorate %__8 DescriptorSet 0
               OpDecorate %__8 Binding 2
               OpDecorate %gl_WorkGroupSize_0 BuiltIn WorkgroupSize
       %void = OpTypeVoid
         %39 = OpTypeFunction %void
        %int = OpTypeInt 32 1
%_ptr_Function_int = OpTypePointer Function %int
         %42 = OpTypeFunction %void %_ptr_Function_int
         %43 = OpTypeFunction %void %_ptr_Function_int %_ptr_Function_int
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
        %__6 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
        %__7 = OpVariable %_ptr_Uniform_g_counts_InOut Uniform
        %__8 = OpVariable %_ptr_Uniform_g_leaves_InOut Uniform
   %uint_1_0 = OpConstant %uint 1
%gl_WorkGroupSize_0 = OpConstantComposite %v3uint %uint_1_0 %uint_1_0 %uint_1_0
       %Init = OpFunction %void None %39
         %60 = OpLabel
      %param = OpVariable %_ptr_Function_int Function
    %param_0 = OpVariable %_ptr_Function_int Function
    %param_1 = OpVariable %_ptr_Function_int Function
    %param_2 = OpVariable %_ptr_Function_int Function
    %param_3 = OpVariable %_ptr_Function_int Function
    %param_4 = OpVariable %_ptr_Function_int Function
               OpStore %param %int_1
         %61 = OpFunctionCall %void %SetNodeCount_i1_ %param
               OpStore %param_0 %int_0
         %62 = OpFunctionCall %void %SetLeafCount_i1_ %param_0
               OpStore %param_1 %int_0
               OpStore %param_2 %int_n1
         %63 = OpFunctionCall %void %Set_Metanode_Parent_i1_i1_ %param_1 %param_2
               OpStore %param_3 %int_0
               OpStore %param_4 %int_n1
         %64 = OpFunctionCall %void %Set_Metanode_IndexInParent_i1_i1_ %param_3 %param_4
               OpReturn
               OpFunctionEnd
%SetNodeCount_i1_ = OpFunction %void None %42
        %cnt = OpFunctionParameter %_ptr_Function_int
         %65 = OpLabel
         %66 = OpLoad %int %cnt
         %67 = OpAccessChain %_ptr_Uniform_int %_ %int_0 %int_0 %uint_0
               OpStore %67 %66
               OpReturn
               OpFunctionEnd
%SetLeafCount_i1_ = OpFunction %void None %42
      %cnt_0 = OpFunctionParameter %_ptr_Function_int
         %68 = OpLabel
         %69 = OpLoad %int %cnt_0
         %70 = OpAccessChain %_ptr_Uniform_int %_ %int_0 %int_0 %uint_1
               OpStore %70 %69
               OpReturn
               OpFunctionEnd
%Set_Metanode_Parent_i1_i1_ = OpFunction %void None %43
      %index = OpFunctionParameter %_ptr_Function_int
      %value = OpFunctionParameter %_ptr_Function_int
         %71 = OpLabel
         %72 = OpLoad %int %index
         %73 = OpLoad %int %value
         %74 = OpAccessChain %_ptr_Uniform_int %__0 %int_0 %72 %uint_0
               OpStore %74 %73
               OpReturn
               OpFunctionEnd
%Set_Metanode_IndexInParent_i1_i1_ = OpFunction %void None %43
    %index_0 = OpFunctionParameter %_ptr_Function_int
    %value_0 = OpFunctionParameter %_ptr_Function_int
         %75 = OpLabel
         %76 = OpLoad %int %index_0
         %77 = OpLoad %int %value_0
         %78 = OpAccessChain %_ptr_Uniform_int %__0 %int_0 %76 %uint_1
               OpStore %78 %77
               OpReturn
               OpFunctionEnd
        %Add = OpFunction %void None %39
         %79 = OpLabel
               OpReturn
               OpFunctionEnd
