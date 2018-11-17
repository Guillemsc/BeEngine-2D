using System;
using System.Collections.Generic;
using System.Xml.Linq;

namespace BeEngine
{
    namespace Internal
    {
        class SolutionManager
        {
            public bool Init(string _project_path)
            {
                bool ret = true;

                try
                {
                    doc = XDocument.Load(_project_path);
                }
                catch (Exception ex)
                {
                    ret = false;
                    doc = null;
                }

                if (ret)
                {
                    project_path = _project_path;
                }

                return ret;
            }

            public bool AddAssembly(string path, string name_space)
            {
                bool ret = false;

                if (path != "" && name_space != "")
                {
                    if (doc != null)
                    {
                        XElement el = doc.Element("Project").Element("ItemGroup");

                        if (el != null)
                        {
                            XElement reference = new XElement("Reference");
                            reference.Add(new XAttribute("Include", name_space));
                            reference.Add(new XElement("HintPath", path));

                            el.Add(reference);

                            doc.Save(project_path);

                            ret = true;
                        }
                    }
                }

                return ret;
            }

            public void RemoveAllAssemblys()
            {
                if (doc != null)
                {
                    XElement el = doc.Element("Project");

                    if (el != null)
                    {
                        IEnumerable<XElement> elements = el.Elements("ItemGroup");

                        foreach (XElement childElement in elements)
                        {
                            childElement.Descendants("Reference").Remove();
                        }

                        doc.Save(project_path);
                    }
                }
            }

            public bool AddScript(string path)
            {
                bool ret = false;

                if (!HasScript(path))
                {
                    if (path != "")
                    {
                        if (doc != null)
                        {
                            XElement el = doc.Element("Project").Element("ItemGroup");

                            if (el != null)
                            {
                                XElement reference = new XElement("Compile");
                                reference.Add(new XAttribute("Include", path));

                                el.Add(reference);

                                doc.Save(project_path);

                                ret = true;
                            }
                        }
                    }
                }

                return ret;
            }

            public bool RemoveScript(string path)
            {
                bool ret = false;

                bool save = false;

                if (path != "")
                {
                    XElement el = doc.Element("Project");

                    if (el != null)
                    {
                        IEnumerable<XElement> elements = el.Elements("ItemGroup");

                        foreach (XElement childElement in elements)
                        {
                            IEnumerable<XElement> compiles = childElement.Elements("Compile");

                            foreach (XElement childCompiles in compiles)
                            {
                                XAttribute at = childCompiles.Attribute("Include");

                                if (at != null)
                                {
                                    if (at.Value == path)
                                    {
                                        childCompiles.Remove();

                                        save = true;
                                        ret = true;
                                    }
                                }
                                else
                                {
                                    childCompiles.Remove();

                                    save = true;
                                    ret = true;
                                }
                            }
                        }
                    }

                    if(save)
                        doc.Save(project_path);
                }

                return ret;
            }

            public bool HasScript(string path)
            {
                if (path != "")
                {
                    XElement el = doc.Element("Project");

                    if (el != null)
                    {
                        IEnumerable<XElement> elements = el.Elements("ItemGroup");

                        foreach (XElement childElement in elements)
                        {
                            IEnumerable<XElement> compiles = childElement.Elements("Compile");

                            foreach (XElement childCompiles in compiles)
                            {
                                XAttribute at = childCompiles.Attribute("Include");

                                if (at != null)
                                {
                                    if (at.Value == path)
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }

                return false;
            }

            private XDocument doc = null;
            private string project_path = "";
        }
    }
}
