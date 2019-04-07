using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Collision
    {
        private GameObject _go_collided = null;

        public GameObject GameObjectCollidedWith
        {
            get
            {
                return _go_collided;
            }
        }
    }
}