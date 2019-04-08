using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineObject
    {
        private void SetPointerRef(string pointer_ref)
        {
            this.pointer_ref = pointer_ref;
        }

        private string GetPointerRef()
        {
            return pointer_ref;
        }

        private string pointer_ref = null;
    }
}
