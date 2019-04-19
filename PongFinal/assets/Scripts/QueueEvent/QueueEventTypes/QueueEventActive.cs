
using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class QueueEventActive : QueueEvent
{
    public QueueEventActive(GameObject element, bool set_active) : base("set_active")
    {
        active = set_active;
        affected_element = element;
    }

    public override void OnStart()
    {
        if (affected_element != null)
        {
            affected_element.Active = active;

            Finish();
        }
    }

    private bool active = false;
    private GameObject affected_element = null;
}