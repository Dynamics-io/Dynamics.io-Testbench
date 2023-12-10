#pragma once

#include "stdafx.h"
#include <boost/algorithm/string/replace.hpp>



namespace DynamicCompute {
    namespace Compute {
        namespace OCL {

            class ShaderDepository
            {
                friend class ComputeController;
                friend class ComputeProgram_OCL;

            private:

                std::string GetCode(std::string name) { return m_objects[name]; }

                std::string GetFunctionDef(std::string name) {
                    std::string code = GetCode(name);
                    boost::replace_all(code, "\n", "");
                    size_t pos = code.find_first_of('{');
                    return code.substr(0, pos) + ";";
                }

                std::vector<std::string> GetFunctionsDefinitions()
                {
                    std::vector<std::string> f_defs;
                    for (auto f_name : m_functions)
                    {
                        f_defs.push_back(GetFunctionDef(f_name));
                    }

                    return f_defs;
                }

                void Load();

                void Clear()
                {
                    //m_kernels.clear();
                    //m_functions.clear();
                    //m_source.clear();
                    m_objects.clear();
                };

                void AddKernel(std::string name, std::string source) {
                    m_objects[name] = source;
                    m_kernels.push_back(name);
                }

                void AddFunction(std::string name, std::string source) {
                    m_objects[name] = source;
                    m_functions.push_back(name);
                }

                void AddSource(std::string name, std::string source) {
                    m_objects[name] = source;
                    m_source.push_back(name);
                }

                std::vector<std::string> m_kernels;
                std::vector<std::string> m_functions;
                std::vector<std::string> m_source;
                std::map<std::string, std::string> m_objects;
            };

        }
    }
}