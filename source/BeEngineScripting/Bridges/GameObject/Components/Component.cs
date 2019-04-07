using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Component : BeEngineObject
    {
        private void SetOwner(GameObject game_object)
        {
            this.game_object = game_object;
        }

        public GameObject game_object = null;
    }
}

