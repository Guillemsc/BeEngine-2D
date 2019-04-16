using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class Timer
{
    bool started = false;
    private float start_time = 0.0f;
    private float start_real_time = 0.0f;

    public void Start()
    {
        started = true;
        start_time = Time.TimeSinceStart;
    }

    public void Reset()
    {
        started = false;
        start_time = 0.0f;
        start_real_time = 0.0f;
    }

    public float ReadTime()
    {
        if (started)
            return Time.TimeSinceStart - start_time;
        else
            return 0.0f;
    }
}