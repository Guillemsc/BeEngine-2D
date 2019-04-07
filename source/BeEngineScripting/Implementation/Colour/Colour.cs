using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class Colour
    {
        public Colour()
        {
            r = 0;
            g = 0;
            b = 0;
            a = 0;
        }

        public Colour(float r, float g, float b, float a)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }

        public Colour(float r, float g, float b)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = 255;
        }

        public float r
        {
            get { return _r; }
            set { _r = value; }
        }

        public float g
        {
            get { return _g; }
            set { _g = value; }
        }

        public float b
        {
            get { return _b; }
            set { _b = value; }
        }

        public float a
        {
            get { return _a; }
            set { _a = value; }
        }

        public override string ToString()
        {
            return "r: " + r + " g: " + g + " b: " + b + " a: " + a;
        }

        private float _r = 0;
        private float _g = 0;
        private float _b = 0;
        private float _a = 0;
    }
}
