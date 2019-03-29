using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class test_script_fields : ComponentScript 
{
	public override void Start () 
	{
        game_object.Transform.Position = new float2(pos_x, pos_z);

        if(go != null)
            Debug.LOG(go.Name);
	}
	
	public override void Update () 
	{
		
	}

    public int int_val = 0;
    public float float_val = 0;
    public string string_val = "";
    public bool bool_val = false;

    public float pos_x = 15;
    public float pos_z = 16;

    public GameObject go = null;
}