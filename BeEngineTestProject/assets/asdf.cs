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

        //game_object.GetComponent<ComponentText>().Text =;
    }
	
	public override void Update () 
	{
        for (int i = 0; i < 160; ++i)
        {
            game_object.GetComponent<ComponentText>();

            //Debug.Log("Clicka");
        }
     
    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }
}