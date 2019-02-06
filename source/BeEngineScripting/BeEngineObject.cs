using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineObject
    {
        private void SetPointerRef(int[] pointer_ref)
        {
            this.pointer_ref = pointer_ref;
        }

        private int[] GetPointerRef()
        {
            return pointer_ref;
        }

        private int[] pointer_ref = null;
    }
}
