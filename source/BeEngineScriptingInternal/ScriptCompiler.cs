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

            public bool AddScript(string add)
            {
                bool ret = false;

                if (File.Exists(add) && !scripts.Contains(add))
                    scripts.Add(add);

                ret = true;
                
                return ret;
            }

            public bool RemoveScript(string remove)
            {
                bool ret = false;

                scripts.Remove(remove);

                return ret;
            }

            public string[] CompileScripts(string dll_output_path)
            {
                List<string> ret = new List<string>();

                if (scripts.Count > 0)
                {
                    if (compile_parameters != null)
                    {
                        compile_parameters.OutputAssembly = dll_output_path;

                        CSharpCodeProvider code_provider = new CSharpCodeProvider();

                        CompilerResults results = code_provider.CompileAssemblyFromFile(compile_parameters, scripts.ToArray());

                        for (int i = 0; i < results.Errors.Count; ++i)
                        {
                            CompilerError curr_error = results.Errors[i];

                            string error = curr_error.FileName + " at (" + curr_error.Line + "," +
                                curr_error.Column + ")," + (curr_error.IsWarning ? "Warning: " : " Error: ") +
                                curr_error.ErrorNumber + ": " + curr_error.ErrorText + ".";

                            ret.Add(error);
                        }

                        code_provider.Dispose();
                    }
                }

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
            List<string> scripts = new List<string>();

            CompilerParameters compile_parameters = new CompilerParameters();
        }
    }
}
