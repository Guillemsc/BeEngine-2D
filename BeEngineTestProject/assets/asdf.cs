using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class asdf : ComponentScript 
{
	public override void Start () 
	{
        game_object.Name = "Pene";

        game_object.GetComponent<ComponentText>().Text = "This is a text";

        Debug.Log(game_object.GetComponent<ComponentText>().Text);

        game_object.GetComponent<ComponentText>().TextColour = new Colour(255, 255, 0);

        Debug.Log(game_object.GetComponent<ComponentText>().TextColour.ToString());

        comp = game_object.AddComponent<asdf>();

        new_go = Scene.CreateGameObject();

        new_go.Name = "Newua";
    }
	
	public override void Update () 
	{
        Scene.DestroyComponent(game_object.GetComponent<ComponentText>());
    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }

    GameObject new_go;

    asdf comp;
}