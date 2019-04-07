using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Float2
    {
        public Float2()
        {
            x = 0;
            y = 0;
        }

        public Float2(float x, float y)
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

        public static Float2 operator + (Float2 val1, Float2 val2)
        {
            return new Float2(val1.x + val2.x, val1.y + val2.y);
        }

        public override string ToString()
        {
            return "x: " + x + " y: " + y;
        }

        private float _x = 0;
        private float _y = 0;
    }
}
