using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class float2
    {
        public float2()
        {
            x = 0;
            y = 0;
        }

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

        public static float2 operator + (float2 val1, float2 val2)
        {
            return new float2(val1.x + val2.x, val1.y + val2.y);
        }

        public override string ToString()
        {
            return "x: " + x + " y: " + y;
        }
    }
}
