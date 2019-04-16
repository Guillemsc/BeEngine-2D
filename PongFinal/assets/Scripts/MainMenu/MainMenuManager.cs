using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class MainMenuManager : ComponentScript 
{
	public override void Start () 
	{
		if(play_button_go != null)
        {
            ComponentButton but = play_button_go.GetComponent<ComponentButton>();
            but.SuscribeToOnClick(OnPlayButtonPressed);
        }
	}

    private void OnPlayButtonPressed(ComponentButton button)
    {
        Scene.LoadScene(game_scene_to_load);
    }

    [ShowOnInspector]
    private GameObject play_button_go;

    [ShowOnInspector]
    private ResourceScene game_scene_to_load;
}