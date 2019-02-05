using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineObject
    {
        public void SetPointerRef(char[] pointer_ref)
        {
            this.pointer_ref = pointer_ref;
        }

        public char[] GetPointerRef()
        {
            return pointer_ref;
        }

        private char[] pointer_ref = null;
    }
}
