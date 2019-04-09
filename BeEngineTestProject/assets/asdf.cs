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

        game_object.AddComponent<asdf>();

        new_go = Scene.CreateGameObject();

        other_go = new_go;

        new_go.Name = "New";
    }
	
	public override void Update () 
	{
        Scene.DestroyGameObject(ref new_go);

        Scene.DestroyGameObject(ref other_go);
    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }

    GameObject new_go;

    GameObject other_go;
}