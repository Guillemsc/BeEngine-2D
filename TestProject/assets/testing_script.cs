using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class testing_script : ComponentScript 
{
	public override void Start () 
	{
		
	}
	
	public override void Update () 
	{
        game_object.Transform.Position = new float2(i, 0);
        i += 0.1f;
	}

    float i = 0;
}