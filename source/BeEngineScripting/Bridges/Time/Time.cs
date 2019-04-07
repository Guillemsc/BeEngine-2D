using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class Time
    {
        public static float DeltaTime
        {
            get
            {
                return GetDeltaTime();
            }
        }

        public static float TimeSinceStart
        {
            get
            {
                return GetTimeSinceStart();
            }
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetTimeSinceStart();

        // -----------------------------------------------------
    }
}
