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


        public float RotationDegrees
        {
            get
            {
                return GetRotationDegrees();
            }
            set
            {
                SetRotationDegrees(value);
            }
        }

        public float2 Scale
        {
            get
            {
                return GetScale();
            }
            set
            {
                SetScale(value);
            }
        }

        // -----------------------------------------------------

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetPosition(float2 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float2 GetPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetRotationDegrees(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetRotationDegrees();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetScale(float2 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float2 GetScale();

        // -----------------------------------------------------
    }
}
