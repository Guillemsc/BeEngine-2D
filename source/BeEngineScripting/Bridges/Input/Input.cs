using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.CompilerServices;

namespace BeEngine
{
    public class Input
    {
        public static bool KeyDown(string key)
        {
            return GetKeyDown(key);
        }

        public static bool KeyRepeat(string key)
        {
            return GetKeyRepeat(key);
        }

        public static bool KeyUp(string key)
        {
            return GetKeyUp(key);
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetKeyDown(string key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetKeyRepeat(string key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetKeyUp(string key);

        // -----------------------------------------------------
    }
}
