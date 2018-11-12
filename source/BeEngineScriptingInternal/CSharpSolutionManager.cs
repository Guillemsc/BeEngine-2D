using System;
using System.Collections.Generic;
using System.Xml.Linq;

namespace BeEngine
{
    class CSharpSolutionManager
    {
        public bool Init(string project_path)
        {
            bool ret = true;

            try
            {
                doc = XDocument.Load(project_path);
            }
            catch(Exception ex)
            {
                ret = false;
            }

            if(ret)
            {

            }

            return ret;
        }

        private XDocument doc = null;
    }
}
