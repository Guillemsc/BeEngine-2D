using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Collision
    {
        private GameObject _go_collided = null;

        public GameObject GoCollided
        {
            get
            {
                return _go_collided;
            }
        }
    }
}