using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Collision
    {
        public GameObject go_collided
        {
            get; private set;
        }

        // Internal functions ----------------------------------

        private void SetCollisionData(GameObject go_collided)
        {
            this.go_collided = go_collided;
        }

        // -----------------------------------------------------
    }
}