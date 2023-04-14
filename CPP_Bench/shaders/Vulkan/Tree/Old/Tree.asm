; SPIR-V
; Version: 1.0
; Generator: Google Clspv; 0
; Bound: 644
; Schema: 0
               OpCapability Shader
               OpCapability Int64
               OpExtension "SPV_KHR_storage_buffer_storage_class"
               OpExtension "SPV_KHR_non_semantic_info"
        %275 = OpExtInstImport "GLSL.std.450"
        %595 = OpExtInstImport "NonSemantic.ClspvReflection.5"
               OpMemoryModel Logical GLSL450
               OpEntryPoint GLCompute %40 "Init"
               OpEntryPoint GLCompute %50 "Add"
               OpExecutionMode %40 LocalSize 1 1 1
               OpExecutionMode %50 LocalSize 1 1 1
               OpSource OpenCL_C 120
        %596 = OpString "Init"
        %599 = OpString "counts"
        %602 = OpString "metanodes"
        %605 = OpString "Add"
        %609 = OpString "counts_ref"
        %612 = OpString "bounds_arr"
        %615 = OpString "numBounds"
        %618 = OpString "NodeChild_Min"
        %622 = OpString "NodeChild_Max"
        %625 = OpString "NodeChild_Index"
        %629 = OpString "NodeChild_LeafCount"
        %633 = OpString "leaves"
        %637 = OpString "metanodes"
        %640 = OpString "result"
               OpMemberDecorate %_struct_2 0 Offset 0
               OpMemberDecorate %_struct_2 1 Offset 4
               OpDecorate %_runtimearr__struct_2 ArrayStride 8
               OpMemberDecorate %_struct_4 0 Offset 0
               OpDecorate %_struct_4 Block
               OpMemberDecorate %_struct_8 0 Offset 0
               OpMemberDecorate %_struct_8 1 Offset 4
               OpMemberDecorate %_struct_8 2 Offset 8
               OpMemberDecorate %_struct_8 3 Offset 12
               OpDecorate %_runtimearr__struct_8 ArrayStride 16
               OpMemberDecorate %_struct_10 0 Offset 0
               OpDecorate %_struct_10 Block
               OpDecorate %_runtimearr_ulong ArrayStride 8
               OpMemberDecorate %_struct_15 0 Offset 0
               OpDecorate %_struct_15 Block
               OpMemberDecorate %_struct_19 0 Offset 0
               OpMemberDecorate %_struct_19 1 Offset 16
               OpDecorate %_runtimearr__struct_19 ArrayStride 32
               OpMemberDecorate %_struct_21 0 Offset 0
               OpDecorate %_struct_21 Block
               OpDecorate %_runtimearr_uint ArrayStride 4
               OpMemberDecorate %_struct_25 0 Offset 0
               OpDecorate %_struct_25 Block
               OpDecorate %_runtimearr_v4float ArrayStride 16
               OpMemberDecorate %_struct_29 0 Offset 0
               OpDecorate %_struct_29 Block
               OpDecorate %6 DescriptorSet 0
               OpDecorate %6 Binding 0
               OpDecorate %12 DescriptorSet 0
               OpDecorate %12 Binding 1
               OpDecorate %17 DescriptorSet 0
               OpDecorate %17 Binding 0
               OpDecorate %23 DescriptorSet 0
               OpDecorate %23 Binding 1
               OpDecorate %27 DescriptorSet 0
               OpDecorate %27 Binding 2
               OpDecorate %31 DescriptorSet 0
               OpDecorate %31 Binding 3
               OpDecorate %32 DescriptorSet 0
               OpDecorate %32 Binding 4
               OpDecorate %33 DescriptorSet 0
               OpDecorate %33 Binding 5
               OpDecorate %34 DescriptorSet 0
               OpDecorate %34 Binding 6
               OpDecorate %35 DescriptorSet 0
               OpDecorate %35 Binding 7
               OpDecorate %36 DescriptorSet 0
               OpDecorate %36 Binding 8
               OpDecorate %37 DescriptorSet 0
               OpDecorate %37 Binding 9
               OpDecorate %270 NoContraction
               OpDecorate %274 NoContraction
               OpDecorate %292 NoContraction
               OpDecorate %296 NoContraction
               OpDecorate %307 NoContraction
               OpDecorate %311 NoContraction
               OpDecorate %314 NoContraction
               OpDecorate %368 NoContraction
               OpDecorate %372 NoContraction
               OpDecorate %380 NoContraction
               OpDecorate %384 NoContraction
               OpDecorate %387 NoContraction
       %uint = OpTypeInt 32 0
  %_struct_2 = OpTypeStruct %uint %uint
%_runtimearr__struct_2 = OpTypeRuntimeArray %_struct_2
  %_struct_4 = OpTypeStruct %_runtimearr__struct_2
%_ptr_StorageBuffer__struct_4 = OpTypePointer StorageBuffer %_struct_4
      %float = OpTypeFloat 32
  %_struct_8 = OpTypeStruct %uint %uint %uint %float
%_runtimearr__struct_8 = OpTypeRuntimeArray %_struct_8
 %_struct_10 = OpTypeStruct %_runtimearr__struct_8
%_ptr_StorageBuffer__struct_10 = OpTypePointer StorageBuffer %_struct_10
      %ulong = OpTypeInt 64 0
%_runtimearr_ulong = OpTypeRuntimeArray %ulong
 %_struct_15 = OpTypeStruct %_runtimearr_ulong
%_ptr_StorageBuffer__struct_15 = OpTypePointer StorageBuffer %_struct_15
    %v4float = OpTypeVector %float 4
 %_struct_19 = OpTypeStruct %v4float %v4float
%_runtimearr__struct_19 = OpTypeRuntimeArray %_struct_19
 %_struct_21 = OpTypeStruct %_runtimearr__struct_19
%_ptr_StorageBuffer__struct_21 = OpTypePointer StorageBuffer %_struct_21
%_runtimearr_uint = OpTypeRuntimeArray %uint
 %_struct_25 = OpTypeStruct %_runtimearr_uint
%_ptr_StorageBuffer__struct_25 = OpTypePointer StorageBuffer %_struct_25
%_runtimearr_v4float = OpTypeRuntimeArray %v4float
 %_struct_29 = OpTypeStruct %_runtimearr_v4float
%_ptr_StorageBuffer__struct_29 = OpTypePointer StorageBuffer %_struct_29
       %void = OpTypeVoid
         %39 = OpTypeFunction %void
%_ptr_StorageBuffer_uint = OpTypePointer StorageBuffer %uint
     %uint_0 = OpConstant %uint 0
     %uint_1 = OpConstant %uint 1
%uint_4294967295 = OpConstant %uint 4294967295
%_ptr_StorageBuffer_ulong = OpTypePointer StorageBuffer %ulong
       %bool = OpTypeBool
   %ulong_32 = OpConstant %ulong 32
%_ptr_StorageBuffer_v4float = OpTypePointer StorageBuffer %v4float
%_ptr_StorageBuffer__struct_19 = OpTypePointer StorageBuffer %_struct_19
    %v4ulong = OpTypeVector %ulong 4
         %91 = OpUndef %v4ulong
    %v2ulong = OpTypeVector %ulong 2
      %false = OpConstantFalse %bool
     %v4uint = OpTypeVector %uint 4
     %uint_2 = OpConstant %uint 2
%_struct_315 = OpTypeStruct %uint %uint
     %uint_4 = OpConstant %uint 4
     %uint_8 = OpConstant %uint 8
    %uint_16 = OpConstant %uint 16
%uint_1431655765 = OpConstant %uint 1431655765
%uint_858993459 = OpConstant %uint 858993459
%uint_252645135 = OpConstant %uint 252645135
    %uint_63 = OpConstant %uint 63
%ulong_18446744069414584320 = OpConstant %ulong 18446744069414584320
%uint_2147483647 = OpConstant %uint 2147483647
%uint_2147483648 = OpConstant %uint 2147483648
%ulong_4294967295 = OpConstant %ulong 4294967295
    %uint_31 = OpConstant %uint 31
        %593 = OpUndef %uint
       %true = OpConstantTrue %bool
    %uint_10 = OpConstant %uint 10
     %uint_3 = OpConstant %uint 3
     %uint_5 = OpConstant %uint 5
     %uint_6 = OpConstant %uint 6
     %uint_7 = OpConstant %uint 7
     %uint_9 = OpConstant %uint 9
          %6 = OpVariable %_ptr_StorageBuffer__struct_4 StorageBuffer
         %12 = OpVariable %_ptr_StorageBuffer__struct_10 StorageBuffer
         %17 = OpVariable %_ptr_StorageBuffer__struct_15 StorageBuffer
         %23 = OpVariable %_ptr_StorageBuffer__struct_21 StorageBuffer
         %27 = OpVariable %_ptr_StorageBuffer__struct_25 StorageBuffer
         %31 = OpVariable %_ptr_StorageBuffer__struct_29 StorageBuffer
         %32 = OpVariable %_ptr_StorageBuffer__struct_29 StorageBuffer
         %33 = OpVariable %_ptr_StorageBuffer__struct_25 StorageBuffer
         %34 = OpVariable %_ptr_StorageBuffer__struct_25 StorageBuffer
         %35 = OpVariable %_ptr_StorageBuffer__struct_25 StorageBuffer
         %36 = OpVariable %_ptr_StorageBuffer__struct_10 StorageBuffer
         %37 = OpVariable %_ptr_StorageBuffer__struct_25 StorageBuffer
         %40 = OpFunction %void None %39
         %41 = OpLabel
         %44 = OpAccessChain %_ptr_StorageBuffer_uint %6 %uint_0 %uint_0 %uint_0
               OpStore %44 %uint_1
         %46 = OpAccessChain %_ptr_StorageBuffer_uint %6 %uint_0 %uint_0 %uint_1
               OpStore %46 %uint_0
         %47 = OpAccessChain %_ptr_StorageBuffer_uint %12 %uint_0 %uint_0 %uint_0
               OpStore %47 %uint_4294967295
         %49 = OpAccessChain %_ptr_StorageBuffer_uint %12 %uint_0 %uint_0 %uint_1
               OpStore %49 %uint_4294967295
               OpReturn
               OpFunctionEnd
         %50 = OpFunction %void None %39
         %51 = OpLabel
         %53 = OpAccessChain %_ptr_StorageBuffer_ulong %17 %uint_0 %uint_0
         %54 = OpAccessChain %_ptr_StorageBuffer_uint %27 %uint_0 %uint_0
         %55 = OpLoad %uint %54
         %57 = OpSGreaterThan %bool %55 %uint_0
               OpSelectionMerge %592 None
               OpBranchConditional %57 %60 %592
         %60 = OpLabel
         %61 = OpPhi %uint %587 %586 %uint_0 %51
         %62 = OpLoad %ulong %53
         %64 = OpShiftRightLogical %ulong %62 %ulong_32
         %65 = OpUConvert %uint %64
         %66 = OpIAdd %uint %61 %uint_0
         %68 = OpAccessChain %_ptr_StorageBuffer_v4float %23 %uint_0 %66 %uint_0
         %69 = OpLoad %v4float %68
         %70 = OpIAdd %uint %61 %uint_0
         %72 = OpAccessChain %_ptr_StorageBuffer__struct_19 %23 %uint_0 %70
         %73 = OpLoad %_struct_19 %72
         %74 = OpCompositeExtract %v4float %73 0
         %75 = OpCompositeExtract %float %74 0
         %76 = OpCompositeExtract %float %74 1
         %77 = OpCompositeExtract %float %74 2
         %78 = OpCompositeExtract %float %74 3
         %79 = OpCompositeExtract %v4float %73 1
         %80 = OpCompositeExtract %float %79 0
         %81 = OpCompositeExtract %float %79 1
         %82 = OpCompositeExtract %float %79 2
         %83 = OpCompositeExtract %float %79 3
         %84 = OpBitcast %uint %75
         %85 = OpUConvert %ulong %84
         %86 = OpBitcast %uint %76
         %87 = OpUConvert %ulong %86
         %88 = OpShiftLeftLogical %ulong %87 %ulong_32
         %89 = OpBitwiseOr %ulong %85 %88
         %92 = OpCompositeInsert %v4ulong %89 %91 0
         %93 = OpBitcast %uint %77
         %94 = OpUConvert %ulong %93
         %95 = OpBitcast %uint %78
         %96 = OpUConvert %ulong %95
         %97 = OpShiftLeftLogical %ulong %96 %ulong_32
         %98 = OpBitwiseOr %ulong %94 %97
         %99 = OpCompositeInsert %v4ulong %98 %92 1
        %100 = OpBitcast %uint %80
        %101 = OpUConvert %ulong %100
        %102 = OpBitcast %uint %81
        %103 = OpUConvert %ulong %102
        %104 = OpShiftLeftLogical %ulong %103 %ulong_32
        %105 = OpBitwiseOr %ulong %101 %104
        %106 = OpCompositeInsert %v4ulong %105 %99 2
        %107 = OpBitcast %uint %82
        %108 = OpUConvert %ulong %107
        %109 = OpBitcast %uint %83
        %110 = OpUConvert %ulong %109
        %111 = OpShiftLeftLogical %ulong %110 %ulong_32
        %112 = OpBitwiseOr %ulong %108 %111
        %113 = OpCompositeInsert %v4ulong %112 %106 3
        %115 = OpVectorShuffle %v2ulong %113 %91 0 1
        %116 = OpVectorShuffle %v2ulong %113 %91 2 3
        %118 = OpSelect %v2ulong %false %115 %116
        %120 = OpBitcast %v4uint %118
        %121 = OpCompositeExtract %uint %120 0
        %122 = OpIAdd %uint %61 %uint_0
        %123 = OpAccessChain %_ptr_StorageBuffer__struct_19 %23 %uint_0 %122
        %124 = OpLoad %_struct_19 %123
        %125 = OpCompositeExtract %v4float %124 0
        %126 = OpCompositeExtract %float %125 0
        %127 = OpCompositeExtract %float %125 1
        %128 = OpCompositeExtract %float %125 2
        %129 = OpCompositeExtract %float %125 3
        %130 = OpCompositeExtract %v4float %124 1
        %131 = OpCompositeExtract %float %130 0
        %132 = OpCompositeExtract %float %130 1
        %133 = OpCompositeExtract %float %130 2
        %134 = OpCompositeExtract %float %130 3
        %135 = OpBitcast %uint %126
        %136 = OpUConvert %ulong %135
        %137 = OpBitcast %uint %127
        %138 = OpUConvert %ulong %137
        %139 = OpShiftLeftLogical %ulong %138 %ulong_32
        %140 = OpBitwiseOr %ulong %136 %139
        %141 = OpCompositeInsert %v4ulong %140 %91 0
        %142 = OpBitcast %uint %128
        %143 = OpUConvert %ulong %142
        %144 = OpBitcast %uint %129
        %145 = OpUConvert %ulong %144
        %146 = OpShiftLeftLogical %ulong %145 %ulong_32
        %147 = OpBitwiseOr %ulong %143 %146
        %148 = OpCompositeInsert %v4ulong %147 %141 1
        %149 = OpBitcast %uint %131
        %150 = OpUConvert %ulong %149
        %151 = OpBitcast %uint %132
        %152 = OpUConvert %ulong %151
        %153 = OpShiftLeftLogical %ulong %152 %ulong_32
        %154 = OpBitwiseOr %ulong %150 %153
        %155 = OpCompositeInsert %v4ulong %154 %148 2
        %156 = OpBitcast %uint %133
        %157 = OpUConvert %ulong %156
        %158 = OpBitcast %uint %134
        %159 = OpUConvert %ulong %158
        %160 = OpShiftLeftLogical %ulong %159 %ulong_32
        %161 = OpBitwiseOr %ulong %157 %160
        %162 = OpCompositeInsert %v4ulong %161 %155 3
        %163 = OpVectorShuffle %v2ulong %162 %91 0 1
        %164 = OpVectorShuffle %v2ulong %162 %91 2 3
        %165 = OpSelect %v2ulong %false %163 %164
        %166 = OpBitcast %v4uint %165
        %167 = OpCompositeExtract %uint %166 1
        %168 = OpIAdd %uint %61 %uint_0
        %169 = OpAccessChain %_ptr_StorageBuffer__struct_19 %23 %uint_0 %168
        %170 = OpLoad %_struct_19 %169
        %171 = OpCompositeExtract %v4float %170 0
        %172 = OpCompositeExtract %float %171 0
        %173 = OpCompositeExtract %float %171 1
        %174 = OpCompositeExtract %float %171 2
        %175 = OpCompositeExtract %float %171 3
        %176 = OpCompositeExtract %v4float %170 1
        %177 = OpCompositeExtract %float %176 0
        %178 = OpCompositeExtract %float %176 1
        %179 = OpCompositeExtract %float %176 2
        %180 = OpCompositeExtract %float %176 3
        %181 = OpBitcast %uint %172
        %182 = OpUConvert %ulong %181
        %183 = OpBitcast %uint %173
        %184 = OpUConvert %ulong %183
        %185 = OpShiftLeftLogical %ulong %184 %ulong_32
        %186 = OpBitwiseOr %ulong %182 %185
        %187 = OpCompositeInsert %v4ulong %186 %91 0
        %188 = OpBitcast %uint %174
        %189 = OpUConvert %ulong %188
        %190 = OpBitcast %uint %175
        %191 = OpUConvert %ulong %190
        %192 = OpShiftLeftLogical %ulong %191 %ulong_32
        %193 = OpBitwiseOr %ulong %189 %192
        %194 = OpCompositeInsert %v4ulong %193 %187 1
        %195 = OpBitcast %uint %177
        %196 = OpUConvert %ulong %195
        %197 = OpBitcast %uint %178
        %198 = OpUConvert %ulong %197
        %199 = OpShiftLeftLogical %ulong %198 %ulong_32
        %200 = OpBitwiseOr %ulong %196 %199
        %201 = OpCompositeInsert %v4ulong %200 %194 2
        %202 = OpBitcast %uint %179
        %203 = OpUConvert %ulong %202
        %204 = OpBitcast %uint %180
        %205 = OpUConvert %ulong %204
        %206 = OpShiftLeftLogical %ulong %205 %ulong_32
        %207 = OpBitwiseOr %ulong %203 %206
        %208 = OpCompositeInsert %v4ulong %207 %201 3
        %209 = OpVectorShuffle %v2ulong %208 %91 0 1
        %210 = OpVectorShuffle %v2ulong %208 %91 2 3
        %211 = OpSelect %v2ulong %false %209 %210
        %212 = OpBitcast %v4uint %211
        %213 = OpCompositeExtract %uint %212 2
        %214 = OpIAdd %uint %61 %uint_0
        %215 = OpAccessChain %_ptr_StorageBuffer__struct_19 %23 %uint_0 %214
        %216 = OpLoad %_struct_19 %215
        %217 = OpCompositeExtract %v4float %216 0
        %218 = OpCompositeExtract %float %217 0
        %219 = OpCompositeExtract %float %217 1
        %220 = OpCompositeExtract %float %217 2
        %221 = OpCompositeExtract %float %217 3
        %222 = OpCompositeExtract %v4float %216 1
        %223 = OpCompositeExtract %float %222 0
        %224 = OpCompositeExtract %float %222 1
        %225 = OpCompositeExtract %float %222 2
        %226 = OpCompositeExtract %float %222 3
        %227 = OpBitcast %uint %218
        %228 = OpUConvert %ulong %227
        %229 = OpBitcast %uint %219
        %230 = OpUConvert %ulong %229
        %231 = OpShiftLeftLogical %ulong %230 %ulong_32
        %232 = OpBitwiseOr %ulong %228 %231
        %233 = OpCompositeInsert %v4ulong %232 %91 0
        %234 = OpBitcast %uint %220
        %235 = OpUConvert %ulong %234
        %236 = OpBitcast %uint %221
        %237 = OpUConvert %ulong %236
        %238 = OpShiftLeftLogical %ulong %237 %ulong_32
        %239 = OpBitwiseOr %ulong %235 %238
        %240 = OpCompositeInsert %v4ulong %239 %233 1
        %241 = OpBitcast %uint %223
        %242 = OpUConvert %ulong %241
        %243 = OpBitcast %uint %224
        %244 = OpUConvert %ulong %243
        %245 = OpShiftLeftLogical %ulong %244 %ulong_32
        %246 = OpBitwiseOr %ulong %242 %245
        %247 = OpCompositeInsert %v4ulong %246 %240 2
        %248 = OpBitcast %uint %225
        %249 = OpUConvert %ulong %248
        %250 = OpBitcast %uint %226
        %251 = OpUConvert %ulong %250
        %252 = OpShiftLeftLogical %ulong %251 %ulong_32
        %253 = OpBitwiseOr %ulong %249 %252
        %254 = OpCompositeInsert %v4ulong %253 %247 3
        %255 = OpVectorShuffle %v2ulong %254 %91 0 1
        %256 = OpVectorShuffle %v2ulong %254 %91 2 3
        %257 = OpSelect %v2ulong %false %255 %256
        %258 = OpBitcast %v4uint %257
        %259 = OpCompositeExtract %uint %258 3
        %260 = OpCompositeConstruct %v4uint %121 %167 %213 %259
        %261 = OpBitcast %v4float %260
        %263 = OpSGreaterThanEqual %bool %65 %uint_2
               OpLoopMerge %590 %586 None
               OpBranch %266
        %266 = OpLabel
               OpSelectionMerge %567 None
               OpBranchConditional %263 %269 %567
        %269 = OpLabel
        %270 = OpFSub %v4float %261 %69
        %271 = OpCompositeExtract %float %270 0
        %272 = OpCompositeExtract %float %270 1
        %273 = OpCompositeExtract %float %270 2
        %274 = OpFMul %float %272 %273
        %276 = OpExtInst %float %275 Fma %271 %272 %274
        %277 = OpExtInst %float %275 Fma %271 %273 %276
               OpBranch %279
        %279 = OpLabel
        %280 = OpPhi %uint %468 %467 %uint_0 %269
        %281 = OpShiftLeftLogical %uint %280 %uint_1
        %282 = OpBitwiseOr %uint %281 %uint_1
        %283 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %281
        %284 = OpLoad %v4float %283
        %285 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %281
        %286 = OpLoad %v4float %285
        %287 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %281
        %288 = OpLoad %uint %287
        %289 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %281
        %290 = OpExtInst %v4float %275 FMin %284 %69
        %291 = OpExtInst %v4float %275 FMax %286 %261
        %292 = OpFSub %v4float %291 %290
        %293 = OpCompositeExtract %float %292 0
        %294 = OpCompositeExtract %float %292 1
        %295 = OpCompositeExtract %float %292 2
        %296 = OpFMul %float %294 %295
        %297 = OpExtInst %float %275 Fma %293 %294 %296
        %298 = OpExtInst %float %275 Fma %293 %295 %297
        %299 = OpSGreaterThanEqual %bool %288 %uint_0
               OpLoopMerge %471 %467 None
               OpBranch %302
        %302 = OpLabel
               OpSelectionMerge %357 None
               OpBranchConditional %299 %305 %357
        %305 = OpLabel
        %306 = OpLoad %uint %289
        %307 = OpFSub %v4float %286 %284
        %308 = OpCompositeExtract %float %307 0
        %309 = OpCompositeExtract %float %307 1
        %310 = OpCompositeExtract %float %307 2
        %311 = OpFMul %float %309 %310
        %312 = OpExtInst %float %275 Fma %308 %309 %311
        %313 = OpExtInst %float %275 Fma %308 %310 %312
        %314 = OpFSub %float %298 %313
        %316 = OpISubBorrow %_struct_315 %306 %uint_1
        %317 = OpCompositeExtract %uint %316 0
        %318 = OpCompositeExtract %uint %316 1
        %319 = OpIEqual %bool %318 %uint_0
        %320 = OpSelect %uint %319 %317 %uint_0
        %321 = OpShiftRightArithmetic %uint %320 %uint_1
        %322 = OpBitwiseOr %uint %321 %320
        %323 = OpShiftRightArithmetic %uint %322 %uint_2
        %324 = OpBitwiseOr %uint %323 %322
        %326 = OpShiftRightArithmetic %uint %324 %uint_4
        %327 = OpBitwiseOr %uint %326 %324
        %329 = OpShiftRightArithmetic %uint %327 %uint_8
        %330 = OpBitwiseOr %uint %329 %327
        %332 = OpShiftRightArithmetic %uint %330 %uint_16
        %333 = OpBitwiseOr %uint %332 %330
        %334 = OpShiftRightLogical %uint %333 %uint_1
        %336 = OpBitwiseAnd %uint %334 %uint_1431655765
        %337 = OpISub %uint %333 %336
        %338 = OpShiftRightLogical %uint %337 %uint_2
        %340 = OpBitwiseAnd %uint %338 %uint_858993459
        %341 = OpBitwiseAnd %uint %337 %uint_858993459
        %342 = OpIAdd %uint %340 %341
        %343 = OpShiftRightLogical %uint %342 %uint_4
        %344 = OpIAdd %uint %343 %342
        %346 = OpBitwiseAnd %uint %344 %uint_252645135
        %347 = OpShiftRightLogical %uint %346 %uint_8
        %348 = OpIAdd %uint %347 %346
        %349 = OpShiftRightLogical %uint %348 %uint_16
        %350 = OpIAdd %uint %349 %348
        %352 = OpBitwiseAnd %uint %350 %uint_63
        %353 = OpConvertSToF %float %352
        %354 = OpExtInst %float %275 FMax %277 %314
        %355 = OpExtInst %float %275 Fma %353 %354 %314
               OpBranch %357
        %357 = OpLabel
        %358 = OpPhi %float %355 %305 %298 %302
        %359 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %282
        %360 = OpLoad %v4float %359
        %361 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %282
        %362 = OpLoad %v4float %361
        %363 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %282
        %364 = OpLoad %uint %363
        %365 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %282
        %366 = OpExtInst %v4float %275 FMin %360 %69
        %367 = OpExtInst %v4float %275 FMax %362 %261
        %368 = OpFSub %v4float %367 %366
        %369 = OpCompositeExtract %float %368 0
        %370 = OpCompositeExtract %float %368 1
        %371 = OpCompositeExtract %float %368 2
        %372 = OpFMul %float %370 %371
        %373 = OpExtInst %float %275 Fma %369 %370 %372
        %374 = OpExtInst %float %275 Fma %369 %371 %373
        %375 = OpSGreaterThan %bool %364 %uint_4294967295
               OpSelectionMerge %422 None
               OpBranchConditional %375 %378 %422
        %378 = OpLabel
        %379 = OpLoad %uint %365
        %380 = OpFSub %v4float %362 %360
        %381 = OpCompositeExtract %float %380 0
        %382 = OpCompositeExtract %float %380 1
        %383 = OpCompositeExtract %float %380 2
        %384 = OpFMul %float %382 %383
        %385 = OpExtInst %float %275 Fma %381 %382 %384
        %386 = OpExtInst %float %275 Fma %381 %383 %385
        %387 = OpFSub %float %374 %386
        %388 = OpISubBorrow %_struct_315 %379 %uint_1
        %389 = OpCompositeExtract %uint %388 0
        %390 = OpCompositeExtract %uint %388 1
        %391 = OpIEqual %bool %390 %uint_0
        %392 = OpSelect %uint %391 %389 %uint_0
        %393 = OpShiftRightArithmetic %uint %392 %uint_1
        %394 = OpBitwiseOr %uint %393 %392
        %395 = OpShiftRightArithmetic %uint %394 %uint_2
        %396 = OpBitwiseOr %uint %395 %394
        %397 = OpShiftRightArithmetic %uint %396 %uint_4
        %398 = OpBitwiseOr %uint %397 %396
        %399 = OpShiftRightArithmetic %uint %398 %uint_8
        %400 = OpBitwiseOr %uint %399 %398
        %401 = OpShiftRightArithmetic %uint %400 %uint_16
        %402 = OpBitwiseOr %uint %401 %400
        %403 = OpShiftRightLogical %uint %402 %uint_1
        %404 = OpBitwiseAnd %uint %403 %uint_1431655765
        %405 = OpISub %uint %402 %404
        %406 = OpShiftRightLogical %uint %405 %uint_2
        %407 = OpBitwiseAnd %uint %406 %uint_858993459
        %408 = OpBitwiseAnd %uint %405 %uint_858993459
        %409 = OpIAdd %uint %407 %408
        %410 = OpShiftRightLogical %uint %409 %uint_4
        %411 = OpIAdd %uint %410 %409
        %412 = OpBitwiseAnd %uint %411 %uint_252645135
        %413 = OpShiftRightLogical %uint %412 %uint_8
        %414 = OpIAdd %uint %413 %412
        %415 = OpShiftRightLogical %uint %414 %uint_16
        %416 = OpIAdd %uint %415 %414
        %417 = OpBitwiseAnd %uint %416 %uint_63
        %418 = OpConvertSToF %float %417
        %419 = OpExtInst %float %275 FMax %277 %387
        %420 = OpExtInst %float %275 Fma %418 %419 %387
               OpBranch %422
        %422 = OpLabel
        %423 = OpPhi %float %420 %378 %374 %357
        %424 = OpFOrdLessThanEqual %bool %358 %423
               OpSelectionMerge %440 None
               OpBranchConditional %424 %427 %440
        %427 = OpLabel
               OpSelectionMerge %435 None
               OpBranchConditional %299 %430 %435
        %430 = OpLabel
               OpStore %283 %290
               OpStore %285 %291
        %431 = OpLoad %uint %287
        %432 = OpLoad %uint %289
        %433 = OpIAdd %uint %432 %uint_1
               OpStore %289 %433
               OpBranch %435
        %435 = OpLabel
        %436 = OpPhi %uint %431 %430 %593 %427
        %437 = OpPhi %bool %false %430 %true %427
        %438 = OpPhi %bool %true %430 %false %427
               OpBranch %440
        %440 = OpLabel
        %441 = OpPhi %uint %436 %435 %593 %422
        %442 = OpPhi %bool %437 %435 %false %422
        %443 = OpPhi %bool %438 %435 %false %422
        %444 = OpPhi %bool %false %435 %true %422
               OpSelectionMerge %460 None
               OpBranchConditional %444 %447 %460
        %447 = OpLabel
               OpSelectionMerge %455 None
               OpBranchConditional %299 %450 %455
        %450 = OpLabel
               OpStore %359 %366
               OpStore %361 %367
        %451 = OpLoad %uint %363
        %452 = OpLoad %uint %365
        %453 = OpIAdd %uint %452 %uint_1
               OpStore %365 %453
               OpBranch %455
        %455 = OpLabel
        %456 = OpPhi %uint %451 %450 %593 %447
        %457 = OpPhi %bool %false %450 %true %447
        %458 = OpPhi %bool %true %450 %443 %447
               OpBranch %460
        %460 = OpLabel
        %461 = OpPhi %uint %456 %455 %441 %440
        %462 = OpPhi %bool %457 %455 %false %440
        %463 = OpPhi %bool %458 %455 %443 %440
               OpBranchConditional %463 %465 %467
        %465 = OpLabel
               OpBranch %467
        %467 = OpLabel
        %468 = OpPhi %uint %461 %465 %593 %460
        %469 = OpPhi %bool %false %465 %true %460
               OpBranchConditional %469 %471 %279
        %471 = OpLabel
               OpSelectionMerge %520 None
               OpBranchConditional %442 %474 %520
        %474 = OpLabel
        %475 = OpLoad %ulong %53
        %476 = OpUConvert %uint %475
        %478 = OpBitwiseAnd %ulong %475 %ulong_18446744069414584320
        %479 = OpIAdd %uint %476 %uint_1
        %480 = OpUConvert %ulong %479
        %481 = OpBitwiseOr %ulong %478 %480
               OpStore %53 %481
        %482 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %476 %uint_0
               OpStore %482 %280
        %483 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %476 %uint_1
               OpStore %483 %uint_0
        %484 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %476 %uint_2
               OpStore %484 %uint_0
        %485 = OpLoad %v4float %283
        %486 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %476
               OpStore %486 %485
        %487 = OpLoad %v4float %285
        %488 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %476
               OpStore %488 %487
        %489 = OpLoad %uint %287
        %490 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %476
               OpStore %490 %489
        %491 = OpLoad %uint %289
        %492 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %476
               OpStore %492 %491
        %493 = OpShiftLeftLogical %uint %476 %uint_1
        %494 = OpBitwiseOr %uint %493 %uint_1
        %495 = OpLoad %ulong %53
        %496 = OpShiftRightLogical %ulong %495 %ulong_32
        %497 = OpUConvert %uint %496
        %499 = OpBitwiseAnd %uint %476 %uint_2147483647
        %501 = OpBitwiseOr %uint %476 %uint_2147483648
        %502 = OpAccessChain %_ptr_StorageBuffer_uint %35 %uint_0 %497
               OpStore %502 %501
        %503 = OpIAdd %uint %497 %uint_1
        %504 = OpUConvert %ulong %503
        %505 = OpShiftLeftLogical %ulong %504 %ulong_32
        %507 = OpBitwiseAnd %ulong %495 %ulong_4294967295
        %508 = OpBitwiseOr %ulong %505 %507
               OpStore %53 %508
        %509 = OpBitwiseXor %uint %497 %uint_4294967295
        %510 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %494
               OpStore %510 %509
        %511 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %494
               OpStore %511 %69
        %512 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %494
               OpStore %512 %261
        %513 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %494
               OpStore %513 %uint_1
        %514 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %493
        %515 = OpLoad %uint %514
        %516 = OpBitwiseXor %uint %515 %uint_4294967295
        %517 = OpAccessChain %_ptr_StorageBuffer_uint %35 %uint_0 %516
               OpStore %517 %499
               OpStore %287 %476
               OpStore %283 %290
               OpStore %285 %291
               OpStore %289 %uint_2
        %518 = OpAccessChain %_ptr_StorageBuffer_uint %37 %uint_0 %61
               OpStore %518 %497
               OpBranch %520
        %520 = OpLabel
               OpSelectionMerge %565 None
               OpBranchConditional %462 %523 %565
        %523 = OpLabel
        %524 = OpLoad %ulong %53
        %525 = OpUConvert %uint %524
        %526 = OpBitwiseAnd %ulong %524 %ulong_18446744069414584320
        %527 = OpIAdd %uint %525 %uint_1
        %528 = OpUConvert %ulong %527
        %529 = OpBitwiseOr %ulong %526 %528
               OpStore %53 %529
        %530 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %525 %uint_0
               OpStore %530 %280
        %531 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %525 %uint_1
               OpStore %531 %uint_1
        %532 = OpAccessChain %_ptr_StorageBuffer_uint %36 %uint_0 %525 %uint_2
               OpStore %532 %uint_0
        %533 = OpLoad %v4float %359
        %534 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %525
               OpStore %534 %533
        %535 = OpLoad %v4float %361
        %536 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %525
               OpStore %536 %535
        %537 = OpLoad %uint %363
        %538 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %525
               OpStore %538 %537
        %539 = OpLoad %uint %365
        %540 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %525
               OpStore %540 %539
        %541 = OpShiftLeftLogical %uint %525 %uint_1
        %542 = OpBitwiseOr %uint %541 %uint_1
        %543 = OpLoad %ulong %53
        %544 = OpShiftRightLogical %ulong %543 %ulong_32
        %545 = OpUConvert %uint %544
        %546 = OpBitwiseAnd %uint %525 %uint_2147483647
        %547 = OpBitwiseOr %uint %525 %uint_2147483648
        %548 = OpAccessChain %_ptr_StorageBuffer_uint %35 %uint_0 %545
               OpStore %548 %547
        %549 = OpIAdd %uint %545 %uint_1
        %550 = OpUConvert %ulong %549
        %551 = OpShiftLeftLogical %ulong %550 %ulong_32
        %552 = OpBitwiseAnd %ulong %543 %ulong_4294967295
        %553 = OpBitwiseOr %ulong %551 %552
               OpStore %53 %553
        %554 = OpBitwiseXor %uint %545 %uint_4294967295
        %555 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %542
               OpStore %555 %554
        %556 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %542
               OpStore %556 %69
        %557 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %542
               OpStore %557 %261
        %558 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %542
               OpStore %558 %uint_1
        %559 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %541
        %560 = OpLoad %uint %559
        %561 = OpBitwiseXor %uint %560 %uint_4294967295
        %562 = OpAccessChain %_ptr_StorageBuffer_uint %35 %uint_0 %561
               OpStore %562 %546
               OpStore %363 %525
               OpStore %359 %366
               OpStore %361 %367
               OpStore %365 %uint_2
        %563 = OpAccessChain %_ptr_StorageBuffer_uint %37 %uint_0 %61
               OpStore %563 %545
               OpBranch %565
        %565 = OpLabel
               OpBranch %567
        %567 = OpLabel
        %568 = OpPhi %bool %false %565 %true %266
               OpBranchConditional %568 %570 %586
        %570 = OpLabel
        %572 = OpShiftLeftLogical %uint %65 %uint_31
        %573 = OpAccessChain %_ptr_StorageBuffer_uint %35 %uint_0 %65
               OpStore %573 %572
        %574 = OpIAdd %uint %65 %uint_1
        %575 = OpUConvert %ulong %574
        %576 = OpShiftLeftLogical %ulong %575 %ulong_32
        %577 = OpBitwiseAnd %ulong %62 %ulong_4294967295
        %578 = OpBitwiseOr %ulong %576 %577
               OpStore %53 %578
        %579 = OpAccessChain %_ptr_StorageBuffer_v4float %31 %uint_0 %65
               OpStore %579 %69
        %580 = OpAccessChain %_ptr_StorageBuffer_v4float %32 %uint_0 %65
               OpStore %580 %261
        %581 = OpBitwiseXor %uint %65 %uint_4294967295
        %582 = OpAccessChain %_ptr_StorageBuffer_uint %33 %uint_0 %65
               OpStore %582 %581
        %583 = OpAccessChain %_ptr_StorageBuffer_uint %34 %uint_0 %65
               OpStore %583 %uint_1
        %584 = OpAccessChain %_ptr_StorageBuffer_uint %37 %uint_0 %61
               OpStore %584 %65
               OpBranch %586
        %586 = OpLabel
        %587 = OpIAdd %uint %61 %uint_1
        %588 = OpSGreaterThanEqual %bool %587 %55
               OpBranchConditional %588 %590 %60
        %590 = OpLabel
               OpBranch %592
        %592 = OpLabel
               OpReturn
               OpFunctionEnd
        %597 = OpExtInst %void %595 Kernel %40 %596 %uint_2
        %598 = OpExtInst %void %595 PropertyRequiredWorkgroupSize %597 %uint_1 %uint_1 %uint_1
        %600 = OpExtInst %void %595 ArgumentInfo %599
        %601 = OpExtInst %void %595 ArgumentStorageBuffer %597 %uint_0 %uint_0 %uint_0 %600
        %603 = OpExtInst %void %595 ArgumentInfo %602
        %604 = OpExtInst %void %595 ArgumentStorageBuffer %597 %uint_1 %uint_0 %uint_1 %603
        %607 = OpExtInst %void %595 Kernel %50 %605 %uint_10
        %608 = OpExtInst %void %595 PropertyRequiredWorkgroupSize %607 %uint_1 %uint_1 %uint_1
        %610 = OpExtInst %void %595 ArgumentInfo %609
        %611 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_0 %uint_0 %uint_0 %610
        %613 = OpExtInst %void %595 ArgumentInfo %612
        %614 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_1 %uint_0 %uint_1 %613
        %616 = OpExtInst %void %595 ArgumentInfo %615
        %617 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_2 %uint_0 %uint_2 %616
        %619 = OpExtInst %void %595 ArgumentInfo %618
        %621 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_3 %uint_0 %uint_3 %619
        %623 = OpExtInst %void %595 ArgumentInfo %622
        %624 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_4 %uint_0 %uint_4 %623
        %626 = OpExtInst %void %595 ArgumentInfo %625
        %628 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_5 %uint_0 %uint_5 %626
        %630 = OpExtInst %void %595 ArgumentInfo %629
        %632 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_6 %uint_0 %uint_6 %630
        %634 = OpExtInst %void %595 ArgumentInfo %633
        %636 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_7 %uint_0 %uint_7 %634
        %638 = OpExtInst %void %595 ArgumentInfo %637
        %639 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_8 %uint_0 %uint_8 %638
        %641 = OpExtInst %void %595 ArgumentInfo %640
        %643 = OpExtInst %void %595 ArgumentStorageBuffer %607 %uint_9 %uint_0 %uint_9 %641
