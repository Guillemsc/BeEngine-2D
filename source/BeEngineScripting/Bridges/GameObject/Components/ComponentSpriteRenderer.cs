using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.CompilerServices;

namespace BeEngine
{
    public class ComponentSpriteRenderer : Component
    {
        public Colour Colour
        {
            get
            {
                return GetSpriteColour();
            }
            set
            {
                SetSpriteColour(value);
            }
        }

        // -----------------------------------------------------

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Colour GetSpriteColour();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string SetSpriteColour(Colour colour);

        // -----------------------------------------------------
    }
}

