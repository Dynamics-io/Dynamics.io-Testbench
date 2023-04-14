glslangValidator -V -e Init --source-entrypoint main Tree_Init.comp -o Tree_Init.spv
glslangValidator -V -e Add --source-entrypoint main Tree_Add.comp -o Tree_Add.spv

spirv-link --target-env vulkan1.3 -o Tree.spv Tree_Init.spv Tree_Add.spv

spirv-dis Tree.spv -o Tree.asm

pause