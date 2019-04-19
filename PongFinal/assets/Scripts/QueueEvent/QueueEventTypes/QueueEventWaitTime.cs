using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class QueueEventWaitTime : QueueEvent
{
    public QueueEventWaitTime(float time_seconds, bool real_time = false) : base("wait_time")
    {
        wait_seconds = time_seconds;
        this.real_time = real_time;
    }

    public override void OnStart()
    {
        timer.Start();
    }

    public override void OnUpdate()
    {
        if (!real_time)
        {
            if (timer.ReadTime() >= wait_seconds)
            {
                Finish();
            }
        }
        else
        {
            if (timer.ReadTime() >= wait_seconds)
            {
                Finish();
            }
        }
    }

    private float wait_seconds = 0.0f;
    private Timer timer = new Timer();
    private bool real_time = false;
}