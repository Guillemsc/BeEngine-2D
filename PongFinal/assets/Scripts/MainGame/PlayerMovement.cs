using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class PlayerMovement : ComponentScript 
{
    public override void Start()
    {

    }

    public override void Update()
    {
        MovePlayer();
    }

    private void MovePlayer()
    {
        if (can_move)
        {
            float speed = Time.DeltaTime * player_speed;

            if (Input.KeyRepeat(move_up_key))
            {
                game_object.Transform.Position = new Float2(game_object.Transform.Position.x, game_object.Transform.Position.y + speed);
            }
            else if (Input.KeyRepeat(move_down_key))
            {
                game_object.Transform.Position = new Float2(game_object.Transform.Position.x, game_object.Transform.Position.y - speed);
            }

            if (max_top_pos.Transform.Position.y < game_object.Transform.Position.y)
            {
                game_object.Transform.Position = new Float2(game_object.Transform.Position.x, max_top_pos.Transform.Position.y);
            }

            if (max_bottom_pos.Transform.Position.y > game_object.Transform.Position.y)
            {
                game_object.Transform.Position = new Float2(game_object.Transform.Position.x, max_bottom_pos.Transform.Position.y);
            }
        }
    }

    public void SetCanMove(bool set)
    {
        can_move = set;
    }

    [ShowOnInspector]
    private float player_speed = 0.0f;

    [ShowOnInspector]
    private GameObject max_top_pos = null;

    [ShowOnInspector]
    private GameObject max_bottom_pos = null;

    [ShowOnInspector]
    public string move_up_key;

    [ShowOnInspector]
    public string move_down_key;

    private bool can_move = true;
}