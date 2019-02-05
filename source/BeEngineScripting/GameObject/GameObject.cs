using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class GameObject : BeEngineObject
    {
        /// <summary>
        ///   <para>Sets or gets the GameObject name</para>
        /// </summary>
        public string Name
        {
            get
            {
                return GetName();
            }
            set
            {
                SetName(value);
            }
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetName(string value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string GetName();

        // -----------------------------------------------------
    }
}
