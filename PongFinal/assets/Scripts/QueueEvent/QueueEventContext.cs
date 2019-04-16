using System.Collections;
using System.Collections.Generic;

public class QueueEventContext
{
    public void PushEvent(QueueEvent ev, bool start_with_last = false)
    {
        if (ev != null)
        {
            if (start_with_last)
                ev.PushWithLastEvent();

            events_to_push.Add(ev);

            last_pushed = ev;
        }
    }

    public void PushEventForced(QueueEvent ev)
    {
        if (ev != null)
        {
            curr_events.Add(ev);

            ev.CallOnEventStart();

            ev.OnStart();

            last_pushed = ev;
        }
    }

    public void LastPushedEventOnStart(DelQueueEvent callback)
    {
        if (last_pushed != null)
        {
            last_pushed.SuscribeToOnEventStart(callback);
        }
    }

    public void LastPushedEventOnFinish(DelQueueEvent callback)
    {
        if (last_pushed != null)
        {
            last_pushed.SuscribeToOnEventFinish(callback);
        }
    }

    public void UpdateEvents()
    {
        if (curr_events.Count > 0)
        {
            for (int i = 0; i < curr_events.Count;)
            {
                QueueEvent curr_event = curr_events[i];

                if (!curr_event.GetFinished())
                {
                    curr_event.OnUpdate();

                    ++i;
                }
                else
                {
                    curr_event.OnFinish();

                    curr_event.CallOnEventFinish();

                    curr_events.RemoveAt(i);
                }
            }
        }
        else
        {
            if (events_to_push.Count > 0)
            {
                bool first = true;
                for (int i = 0; i < events_to_push.Count;)
                {
                    QueueEvent curr_event = events_to_push[i];

                    bool add = false;

                    if (first)
                    {
                        add = true;

                        first = false;
                    }
                    else if (curr_event.GetPushWithLastEvent())
                    {
                        add = true;
                    }

                    if (add)
                    {
                        curr_events.Add(curr_event);

                        events_to_push.RemoveAt(i);

                        curr_event.CallOnEventStart();

                        curr_event.OnStart();
                    }
                    else
                        break;
                }
            }
        }
    }

    private List<QueueEvent> events_to_push = new List<QueueEvent>();
    private List<QueueEvent> curr_events = new List<QueueEvent>();

    private QueueEvent last_pushed = null;
}