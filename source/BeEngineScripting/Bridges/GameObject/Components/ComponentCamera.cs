using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.CompilerServices;

namespace BeEngine
{
    public class ComponentCamera : Component
    {
        public float ViewUpPosition()
        {
            return GetViewUpPosition();
        }

        public float ViewDownPosition()
        {
            return GetViewDownPosition();
        }

        public float ViewLeftPosition()
        {
            return GetViewLeftPosition();
        }

        public float ViewRightPosition()
        {
            return GetViewRightPosition();
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetViewUpPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetViewDownPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetViewLeftPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetViewRightPosition();

        // -----------------------------------------------------

    }
}

