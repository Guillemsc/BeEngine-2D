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

                    for (int i = 0; i < ref_assemblies.Count; ++i)
                    {
                        if(File.Exists(ref_assemblies[i]))
                            compile_parameters.ReferencedAssemblies.Add(ref_assemblies[i]);
                    }

                    ret = true;
                }

                return ret;
            }

            public bool AddScript(string add)
            {
                bool ret = false;

                if (File.Exists(add))
                {
                    if(!scripts.Contains(add))
                        scripts.Add(add);

                    ret = true;
                }
                
                return ret;
            }

            public bool RemoveScript(string remove)
            {
                bool ret = false;

                ret = scripts.Remove(remove);

                return ret;
            }

            public string[] GetScripts()
            {
                return scripts.ToArray();
            }

            public bool CompileScripts(string dll_output_path)
            {
                bool ret = true;

                if (scripts.Count > 0)
                {
                    if (compile_parameters != null)
                    {
                        if (File.Exists(dll_output_path))
                            File.Delete(dll_output_path);

                        compile_parameters.OutputAssembly = dll_output_path;

                        CSharpCodeProvider code_provider = new CSharpCodeProvider();

                        CompilerResults results = code_provider.CompileAssemblyFromFile(compile_parameters, scripts.ToArray());

                        compile_errors.Clear();
                        compile_warnings.Clear();

                        for (int i = 0; i < results.Errors.Count; ++i)
                        {
                            CompilerError curr_error = results.Errors[i];

                            string file_name = Path.GetFileNameWithoutExtension(curr_error.FileName);
                            string error = "Script: " + file_name + " at (" + curr_error.Line + "," +
                                curr_error.Column + ")," + (curr_error.IsWarning ? " Warning: " : " Error: ") +
                                ": " + curr_error.ErrorText + ". (" + curr_error.ErrorNumber + ")";

                            if (!curr_error.IsWarning)
                            {
                                compile_errors.Add(error);
                                ret = false;
                            }
                            else
                            {
                                compile_warnings.Add(error);
                            }
                        }

                        code_provider.Dispose();
                    }
                }

                return ret;
            }

            public string[] GetCompileErrors()
            {
                return compile_errors.ToArray();
            }

            public string[] GetCompileWarnings()
            {
                return compile_warnings.ToArray();
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

            bool ClassIsSubclassOf(string class_to_check, string parent_class)
            {
                bool ret = false;

                Type t_class_to_check = Type.GetType(class_to_check);
                Type t_parent_class = Type.GetType(parent_class);

                if(t_class_to_check != null && t_parent_class != null)
                    ret = t_class_to_check.IsSubclassOf(t_parent_class) || t_class_to_check == t_parent_class;

                return ret;
            }

            private List<string> ref_assemblies = new List<string>();
            List<string> scripts = new List<string>();
            List<string> compile_errors = new List<string>();
            List<string> compile_warnings = new List<string>();

            CompilerParameters compile_parameters = new CompilerParameters();
        }
    }
}
