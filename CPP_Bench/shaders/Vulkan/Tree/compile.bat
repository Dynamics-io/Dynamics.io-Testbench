mkdir obj
mkdir bin

glslangValidator -V -e Init --source-entrypoint main -I"C:\Users\jdrurka1\source\repos\Dynamics-io\Dynamics.io-Testbench\CPP_Bench\shaders\include" Tree_Init.comp -o obj/Tree_Init.spv
glslangValidator -V -e Add --source-entrypoint main -I"C:\Users\jdrurka1\source\repos\Dynamics-io\Dynamics.io-Testbench\CPP_Bench\shaders\include" Tree_Add.comp -o obj/Tree_Add.spv

spirv-link --target-env vulkan1.3 -o Tree.spv obj/Tree_Init.spv obj/Tree_Add.spv

spirv-dis Tree.spv -o Tree.asm

pause