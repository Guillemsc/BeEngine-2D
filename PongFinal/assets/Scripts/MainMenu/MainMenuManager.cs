using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class MainMenuManager : ComponentScript 
{
	public override void Start () 
	{
        queue_event = QueueEventManager.Instance.CreateContext();

		if(play_button_go != null)
        {
            ComponentButton but = play_button_go.GetComponent<ComponentButton>();
            but.SuscribeToOnClick(OnPlayButtonPressed);
        }

        if(credits_button_go != null)
        {
            ComponentButton but = credits_button_go.GetComponent<ComponentButton>();
            but.SuscribeToOnClick(OnCreditsButtonPressed);
        }

        if(credits_back_button_go != null)
        {
            ComponentButton but = credits_back_button_go.GetComponent<ComponentButton>();
            but.SuscribeToOnClick(OnCreditsBackButtonPressed);
        }

        if(camera_go != null)
        {
            camera = camera_go.GetComponent<ComponentCamera>();
        }

        intro_parent.Transform.Position = enter_pos.Transform.Position;

        credits_parent.Transform.Position = enter_pos.Transform.Position;
        credits_parent.Active = false;
        credits_back_button_go.Active = false;

        main_menu_parent.Transform.Position = enter_pos.Transform.Position;
        main_menu_parent.Active = false;

        PlayIntro();
    }

    private void PlayIntro()
    {
        queue_event.PushEvent(new QueueEventWaitTime(1));

        PlayEnterAnimation(intro_parent);

        queue_event.PushEvent(new QueueEventWaitTime(3));

        PlayOutAnimation(intro_parent);

        queue_event.PushEvent(new QueueEventWaitTime(0.2f));

        PlayEnterAnimation(main_menu_parent);
    }

    private void OnPlayButtonPressed(ComponentButton button)
    {
        PlayOutAnimation(main_menu_parent);

        queue_event.LastPushedEventOnFinish(OnPlayButtonAnimationFinished);
    }

    private void OnCreditsButtonPressed(ComponentButton button)
    {
        PlayOutAnimation(main_menu_parent);

        PlayEnterAnimation(credits_parent);

        credits_back_button_go.Active = true;
    }

    private void OnCreditsBackButtonPressed(ComponentButton button)
    {
        PlayOutAnimation(credits_parent);

        PlayEnterAnimation(main_menu_parent);

        credits_back_button_go.Active = false;
    }

    private void PlayEnterAnimation(GameObject go)
    {
        if (go != null)
        {
            queue_event.PushEvent(new QueueEventActive(go, true));

            queue_event.PushEvent(new QueueEventPosition(go, enter_pos.Transform.Position,
                camera_go.Transform.Position, 1.3f, EasingFunctionsType.QUAD_IN_OUT));
        }
    }

    private void PlayOutAnimation(GameObject go)
    {
        if (go != null)
        {
            queue_event.PushEvent(new QueueEventPosition(go, camera_go.Transform.Position,
                out_pos.Transform.Position, 0.5f, EasingFunctionsType.QUAD_IN_OUT));

            queue_event.PushEvent(new QueueEventActive(go, false));
        }
    }

    private void OnPlayButtonAnimationFinished(QueueEvent ev)
    {
        Scene.LoadScene(game_scene_to_load);
    }

    [ShowOnInspector]
    private GameObject enter_pos;

    [ShowOnInspector]
    private GameObject out_pos;

    [ShowOnInspector]
    private GameObject camera_go;

    [ShowOnInspector]
    private GameObject play_button_go;

    [ShowOnInspector]
    private GameObject credits_button_go;

    [ShowOnInspector]
    private GameObject credits_back_button_go;

    [ShowOnInspector]
    private ResourceScene game_scene_to_load;

    [ShowOnInspector]
    private GameObject main_menu_parent;

    [ShowOnInspector]
    private GameObject credits_parent;

    [ShowOnInspector]
    private GameObject intro_parent;

    private ComponentCamera camera = null;

    private QueueEventContext queue_event = null;
}