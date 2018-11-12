using Microsoft.CSharp;
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
            public bool Init(List<string> referenced_assemblies)
            {
                bool ret = false;

                if (referenced_assemblies != null)
                {
                    ref_assemblies = referenced_assemblies;

                    ret = true;
                }

                return ret;
            }

            public bool CompileScript(string script_path, string script_name, ref List<string> compile_errors)
            {
                CompilerParameters compile_parameters = new CompilerParameters();

                compile_parameters.GenerateExecutable = false;
                compile_parameters.OutputAssembly = script_name;
                compile_parameters.IncludeDebugInformation = true;
                compile_parameters.GenerateInMemory = true;
                //compile_parameters.ReferencedAssemblies.Add(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\System.dll");
                //compile_parameters.ReferencedAssemblies.Add(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\TheEngine.dll");

                CompilerResults results = code_provider.CompileAssemblyFromFile(compile_parameters, script_path);

                for (int i = 0; i < results.Errors.Count; ++i)
                {
                    CompilerError curr_error = results.Errors[i];

                    compile_errors.Add(curr_error.ErrorText);
                }

                return false;
            }

            private CSharpCodeProvider code_provider = new CSharpCodeProvider();
            private List<string> ref_assemblies = new List<string>();
        }
    }
}
