using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class asdf : ComponentScript 
{
	public override void Start () 
	{
        button = game_object.AddComponent<ComponentButton>();

        if (button != null)
            button.SuscribeToOnClick(OnClick);

        game_object.Transform.Position = new float2(pos_x, pos_z);

        if(go != null)
            Debug.LOG(go.Name);


	}
	
	public override void Update () 
	{


        //if (go != null)
        //    Debug.LOG(go.Transform.RotationDegrees.ToString());

        float speed = Time.DeltaTime * 100.1f;

        if (Input.KeyRepeat("a"))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x - speed, game_object.Transform.Position.y);
        }

        if (Input.KeyRepeat("d"))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x + speed, game_object.Transform.Position.y);
        }

        if (Input.KeyRepeat("s"))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, game_object.Transform.Position.y - speed);
        }

        if (Input.KeyRepeat("w"))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, game_object.Transform.Position.y + speed);
        }
    }

    public void OnClick(ComponentButton but)
    {
        Debug.LOG("Click");
    }


    public int int_val = 0;
    public float float_val = 0;
    public string string_val = "";
    public bool bool_val = false;

    public float pos_x = 15;
    public float pos_z = 16;

    public GameObject go = null;
    private ComponentButton button;
}