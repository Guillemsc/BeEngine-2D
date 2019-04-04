using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class GameManager : ComponentScript 
{
	public override void Start () 
	{
		
	}
	
	public override void Update () 
	{
		if(Input.KeyDown("R"))
        {
            ball.Transform.Position = new float2(0, 0);
        }
	}

    public GameObject ball;
}