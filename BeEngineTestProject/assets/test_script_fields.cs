using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class test_script_fields : ComponentScript 
{
	public override void Start () 
	{
        button = game_object.AddComponent<ComponentButton>();
        game_object.AddComponent<ComponentCanvas>();
        game_object.AddComponent<ComponentPhysicsBody>();
        game_object.AddComponent<ComponentCamera>();
        game_object.AddComponent<ComponentPolygonCollider>();
        game_object.AddComponent<ComponentSpriteRenderer>();

        if (button != null)
            button.SuscribeToOnClick(OnClick);

        game_object.Transform.Position = new float2(pos_x, pos_z);

        if(go != null)
            Debug.LOG(go.Name);
	}
	
	public override void Update () 
	{
		
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