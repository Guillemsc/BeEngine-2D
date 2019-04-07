using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class ComponentTransform : Component
    {
        public Float2 Position
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

        public Float2 Scale
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
        private extern void SetPosition(Float2 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Float2 GetPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetRotationDegrees(float value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern float GetRotationDegrees();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetScale(Float2 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Float2 GetScale();

        // -----------------------------------------------------
    }
}
