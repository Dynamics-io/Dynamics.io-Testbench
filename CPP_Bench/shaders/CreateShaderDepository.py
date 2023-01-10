import glob, os

path = '..\\source\\library\\Compute\\';
new_line_char = '\\n';


depo_cpp_source = """
#include "ShaderDepository.h"

using namespace FlexRigLib::Compute;

void ShaderDepository::Load()
{
@inner_source@
}


""";
inner_source = "";

for file in glob.glob("*.cl"):
    cl_file = open(file, "r");
    
    cl_source = cl_file.read().replace('\n', new_line_char);
    
    if '#exclude' in cl_source:
        cl_file.close();
        continue;
    
    file_name = file.split('.')[0];
    file_name_parts = file_name.split('_');
    s_type = file_name_parts.pop(0);
    s_name = '_'.join(file_name_parts);
    
    methodCall = '';
    match s_type:
        case 'kernel':
            methodCall = 'AddKernel'
        case 'function':
            methodCall = 'AddFunction'
        case 'source':
            methodCall = 'AddSource'
    inner_source += f"    {methodCall}(\"{s_name}\", \"{cl_source}\");\n";
    
    cl_file.close();
    
depo_cpp_source = depo_cpp_source.replace('@inner_source@', inner_source);

cpp_file = open(path + 'ShaderDepository.cpp', 'w');
cpp_file.write(depo_cpp_source);
cpp_file.close();



    