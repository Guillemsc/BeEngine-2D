using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.CompilerServices;

namespace BeEngine
{
    public class ComponentText : Component
    {
        public string Text
        {
            get
            {
                return GetText();
            }
            set
            {
                SetText(value);
            }
        }

        public Colour Colour
        {
            get
            {
                return GetTextColour();
            }
            set
            {
                SetTextColour(value);
            }
        }

        // -----------------------------------------------------

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string GetText();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string SetText(string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Colour GetTextColour();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string SetTextColour(Colour colour);

        // -----------------------------------------------------

    }
}
