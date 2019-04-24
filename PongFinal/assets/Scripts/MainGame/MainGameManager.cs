using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class MainGameManager : ComponentScript 
{
	public override void Start () 
	{
        queue_event = QueueEventManager.Instance.CreateContext();

        if(foreground_go != null)
        {
            foreground = foreground_go.GetComponent<ComponentSpriteRenderer>();

            Colour curr_col = foreground.Colour;
            foreground.Colour = new Colour(curr_col.r, curr_col.g, curr_col.b, 255);

            Debug.Log("Hi");
        }

        PlayFadeInAnimation();
    }
	
	public override void Update () 
	{
		
	}

    private void PlayFadeInAnimation()
    {
        if (foreground != null)
        {
            queue_event.PushEvent(new QueueEventSpriteFade(foreground, 255,
             0, 5.3f, EasingFunctionsType.QUAD_IN));
        }
    }

    private void PlayFadeOutAnimation()
    {
        if (foreground != null)
        {
            queue_event.PushEvent(new QueueEventSpriteFade(foreground, 0,
                255, 1.3f, EasingFunctionsType.QUAD_OUT));
        }
    }

    [ShowOnInspector]
    private GameObject foreground_go = null;

    private ComponentSpriteRenderer foreground = null;

    private QueueEventContext queue_event = null;
}