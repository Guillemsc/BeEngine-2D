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
                    compile_parameters.IncludeDebugInformation = false;
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

            bool CreateCSScriptFile(string save_filepath, string code)
            {
                bool ret = false;

                if (!File.Exists(save_filepath))
                {
                    using (File.Create(save_filepath)) {}

                    using (TextWriter tw = new StreamWriter(save_filepath))
                    {
                        if (tw != null)
                        {
                            tw.Write(code);

                            ret = true;
                        }
                    }
                }

                return ret;
            }

            string ReadCSScriptFile(string filepath)
            {
                string ret = "";

                if (File.Exists(filepath))
                {
                    using (StreamReader tw = new StreamReader(filepath))
                    {
                        if (tw != null)
                        {
                            ret = tw.ReadToEnd();
                        }
                    }
                }

                return ret;
            }

            bool WriteCSScriptFile(string filepath, string code)
            {
                bool ret = false;

                if (File.Exists(filepath))
                {
                    using (TextWriter tw = new StreamWriter(filepath))
                    {
                        if (tw != null)
                        {
                            tw.Write("");
                            tw.Write(code);

                            ret = true;
                        }
                    }
                }

                return ret;
            }

            private List<string> ref_assemblies = new List<string>();
            CompilerParameters compile_parameters = new CompilerParameters();
        }
    }
}
