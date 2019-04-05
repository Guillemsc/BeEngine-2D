using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class Debug
    {
        public static void Log(string log)
        {
            ConsoleLog(log);
        }

        // Internal Calls -------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ConsoleLog(string log);

        // -----------------------------------------------------
    }
}
