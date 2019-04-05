using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class PlayerMovement : ComponentScript 
{
	public override void Start () 
	{
		
	}
	
	public override void Update () 
	{
        MovePlayer();
    }

    private void MovePlayer()
    {
        float speed = Time.DeltaTime * player_speed;

        if (Input.KeyRepeat(move_up))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, game_object.Transform.Position.y + speed);
        }
        else if(Input.KeyRepeat(move_down))
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, game_object.Transform.Position.y - speed);
        }

        if(max_top_pos.Transform.Position.y < game_object.Transform.Position.y)
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, max_top_pos.Transform.Position.y);
        }

        if (max_bottom_pos.Transform.Position.y > game_object.Transform.Position.y)
        {
            game_object.Transform.Position = new float2(game_object.Transform.Position.x, max_bottom_pos.Transform.Position.y);
        }

    }

    public float player_speed = 0.0f;

    public GameObject max_top_pos;
    public GameObject max_bottom_pos;

    public string move_up;
    public string move_down;
}