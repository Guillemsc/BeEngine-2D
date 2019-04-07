using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class player_movement : ComponentScript 
{
	public override void Start () 
	{
		
	}
	
	public override void Update () 
	{
        float speed = Time.DeltaTime * 100.1f;

        if (Input.KeyRepeat("as"))
        {
            game_object.Transform.Position = new Float2(game_object.Transform.Position.x - speed, game_object.Transform.Position.y);
        }

        if (Input.KeyRepeat("d"))
        {
            game_object.Transform.Position = new Float2(game_object.Transform.Position.x + speed, game_object.Transform.Position.y);
        }

        if (Input.KeyRepeat("s"))
        {
            game_object.Transform.Position = new Float2(game_object.Transform.Position.x, game_object.Transform.Position.y - speed);
        }

        //if (Input.KeyRepeat("w"))
        //{
        //    game_object.Transform.Position = new float2(game_object.Transform.Position.x, game_object.Transform.Position.y + speed);
        //}
    }
}