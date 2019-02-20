using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class ComponentTransform : Component
    {
        public float2 Position
        {
            get
            {
                return GetPosition();
            }
            set
            {
                SetPosition(value);
            }
        }

        // -----------------------------------------------------

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetPosition(float2 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float2 GetPosition();

        // -----------------------------------------------------
    }
}
