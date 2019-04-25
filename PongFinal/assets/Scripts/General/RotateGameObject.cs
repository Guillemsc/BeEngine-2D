using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class RotateGameObject : ComponentScript 
{
	public override void Start () 
	{
		
	}
	
	public override void Update () 
	{
        float speed = rotation_speed * Time.DeltaTime;

        game_object.Transform.RotationDegrees = game_object.Transform.RotationDegrees + speed;
    }

    [ShowOnInspector]
    private float rotation_speed = 0.0f;
}