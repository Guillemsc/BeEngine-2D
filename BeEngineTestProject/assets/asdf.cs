using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class asdf : ComponentScript 
{
	public override void Start () 
	{
        game_object.GetComponent<ComponentText>().Text = "This is a text";

        Debug.Log(game_object.GetComponent<ComponentText>().Text);

        //game_object.GetComponent<ComponentText>().Text =;
    }
	
	public override void Update () 
	{

    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }
}