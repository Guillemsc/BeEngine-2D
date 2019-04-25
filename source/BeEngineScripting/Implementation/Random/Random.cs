using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public class Random
    {
        public static int Range(int min, int max)
        {
            return new System.Random().Next(0, max + 1);
        }

        public static float Range(float min, float max)
        {
            return (float)new System.Random().NextDouble() * (max - min) + min;
        }

        public static double Range(double min, double max)
        {
            return new System.Random().NextDouble() * (max - min) + min;
        }

        // Internal Calls --------------------------------------

        // -----------------------------------------------------
    }
}