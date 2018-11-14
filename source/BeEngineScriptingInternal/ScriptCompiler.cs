using Microsoft.CSharp;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace BeEngine
{
    namespace Internal
    {
        public class ScriptCompiler
        {
            public bool Init(string[] referenced_assemblies)
            {
                bool ret = false;

                if (referenced_assemblies != null)
                {
                    ref_assemblies = new List<string>(referenced_assemblies);

                    compile_parameters = new CompilerParameters();

                    compile_parameters.GenerateExecutable = false;
                    compile_parameters.IncludeDebugInformation = true;
                    compile_parameters.GenerateInMemory = true;

                    string a = referenced_assemblies[0];

                    for (int i = 0; i < ref_assemblies.Count; ++i)
                    {
                        compile_parameters.ReferencedAssemblies.Add(ref_assemblies[i]);
                    }

                    ret = true;
                }

                return ret;
            }

            public string[] CompileScript(string script_path, string dll_output_path)
            {
                List<string> ret = new List<string>();

                compile_parameters.OutputAssembly = dll_output_path;

                CSharpCodeProvider code_provider = new CSharpCodeProvider();

                CompilerResults results = code_provider.CompileAssemblyFromFile(compile_parameters, script_path);

                for (int i = 0; i < results.Errors.Count; ++i)
                {
                    CompilerError curr_error = results.Errors[i];

                    ret.Add(curr_error.ErrorText);
                }

                code_provider.Dispose();

                return ret.ToArray();
            }

            private List<string> ref_assemblies = new List<string>();
            CompilerParameters compile_parameters = new CompilerParameters();
        }
    }
}
