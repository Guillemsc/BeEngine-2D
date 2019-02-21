using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class float2
    {
        private float _x = 0;
        private float _y = 0;

        public float2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public float x
        {
            get { return _x; }
            set { _x = value; }
        }

        public float y
        {
            get { return _y; }
            set { _y = value; }
        }
    }
}
