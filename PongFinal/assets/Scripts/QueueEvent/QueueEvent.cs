
using System.Collections;
using System.Collections.Generic;

public delegate void DelQueueEvent(QueueEvent ev);

public class QueueEvent
{
    public QueueEvent(string event_type)
    {
        this.event_type = event_type;
    }

    public virtual void OnStart() { }
    public virtual void OnUpdate() { }
    public virtual void OnFinish() { }

    public string GetEventType()
    {
        return event_type;
    }

    public void PushWithLastEvent()
    {
        push_with_last_event = true;
    }

    public bool GetPushWithLastEvent()
    {
        return push_with_last_event;
    }

    public void Finish()
    {
        finished = true;
    }

    public bool GetFinished()
    {
        return finished;
    }

    public void SuscribeToOnEventStart(DelQueueEvent ev)
    {
        on_event_start += ev;
    }

    public void CallOnEventStart()
    {
        if (on_event_start != null)
            on_event_start(this);
    }

    public void SuscribeToOnEventFinish(DelQueueEvent ev)
    {
        on_event_finish += ev;
    }

    public void CallOnEventFinish()
    {
        if (on_event_finish != null)
            on_event_finish(this);
    }

    private string event_type = "";
    private bool finished = false;

    private bool push_with_last_event = false;

    private DelQueueEvent on_event_start = null;
    private DelQueueEvent on_event_finish = null;
}