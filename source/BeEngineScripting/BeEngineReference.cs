using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineReference
    {   
        public IntPtr SetPtr(IntPtr set)
        {
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
