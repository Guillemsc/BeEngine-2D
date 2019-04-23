using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class Application
    {
        public static void Quit()
        {
            ApplicationQuit();
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplicationQuit();

        // -----------------------------------------------------
    }
}