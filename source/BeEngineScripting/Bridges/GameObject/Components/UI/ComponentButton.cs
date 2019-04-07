using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public delegate void ButtonClickDelegate(ComponentButton button);

    public class ComponentButton : Component
    {
        public void SuscribeToOnClick(ButtonClickDelegate callback)
        {
            on_click += callback;
        }

        public void UnSuscribeFromOnClick(ButtonClickDelegate callback)
        {
            on_click -= callback;
        }

        private ButtonClickDelegate on_click;

        // Internal functions ----------------------------------

        private void CallOnClick()
        {
            if (on_click != null)
                on_click(this);
        }

        // -----------------------------------------------------
    }
}
