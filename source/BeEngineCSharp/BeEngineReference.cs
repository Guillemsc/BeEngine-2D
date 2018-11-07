using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BeEngine
{
    class BeEngineReference
    {   
        public IntPtr SetPtr(IntPtr set)
        {
            AssemblyFactory factory = new AssemblyFactory();

            ptr = set;

            return set;
        }

        public IntPtr GetPtr()
        {
            return ptr;
        }

        private IntPtr ptr;
    }
}
