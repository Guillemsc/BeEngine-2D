using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class BallMovement : ComponentScript 
{
    public enum BallDirection
    {
        DIRECTION_LEFT,
        DIRECTION_RIGHT,
    }

	public override void Start () 
	{
        game_object.SuscribeToOnCollisionEnter(OnCollisionEnter);

        ResetBall(BallDirection.DIRECTION_LEFT);
    }
	
	public override void Update () 
	{
        if (can_move)
        {
            float speed = Time.DeltaTime * ball_speed;

            float angle_radiants = curr_angle * 0.0174532925199432957f;

            float add_x = (float)System.Math.Sin(angle_radiants) * speed;
            float add_y = (float)System.Math.Cos(angle_radiants) * speed;

            Float2 position_addition = new Float2(add_x, add_y);

            game_object.Transform.Position = new Float2(game_object.Transform.Position.x - position_addition.x,
                game_object.Transform.Position.y + position_addition.y);
        }
    }

    public void SetSpeed(float speed)
    {
        ball_speed = speed;
    }

    public void ResetBall(BallDirection dir)
    {
        game_object.Transform.Position = new Float2(0, 0);

        switch (dir)
        {
            case BallDirection.DIRECTION_LEFT:
                {
                    curr_angle = Random.Range(30.0f, 150.0f);
                    curr_dir = 1;

                    break;
                }
            case BallDirection.DIRECTION_RIGHT:
                {
                    curr_angle = Random.Range(-30.0f, -150.0f);
                    curr_dir = 2;
                    break;
                }
        }
    }

    public void StartMoving()
    {
        can_move = true;
    }

    public void StopMoving()
    {
        can_move = false;
    }

    private void OnCollisionEnter(Collision coll)
    {
        if(coll.GameObjectCollidedWith == player_1)
        {            
            if (curr_dir == 1)
            {
                curr_angle = Random.Range(-40.0f, -140.0f);
                curr_dir = 2;

                bottom_disabled = false;
                top_disabled = false;
            }
        }

        if(coll.GameObjectCollidedWith == player_2)
        {           
           if (curr_dir == 2)
           {
               curr_angle = Random.Range(40.0f, 140.0f);
               curr_dir = 1;

               bottom_disabled = false;
               top_disabled = false;
            }
        }

        if(coll.GameObjectCollidedWith == boundaries_up)
        {
            if (!top_disabled)
            {
                if (curr_dir == 1)
                {
                    curr_angle = 180 - curr_angle;
                }
                else if (curr_dir == 2)
                {
                    curr_angle = -180 - curr_angle;
                }

                top_disabled = true;
                bottom_disabled = false;

            }
        }
        else if (coll.GameObjectCollidedWith == boundaries_down)
        {
            if (!bottom_disabled)
            {
                if (curr_dir == 1)
                {
                    curr_angle = 180 - curr_angle;
                }
                else if (curr_dir == 2)
                {
                    curr_angle = -180 - curr_angle;
                }

                bottom_disabled = true;
                top_disabled = false;
            }
        }
    }

    public GameObject player_1;
    public GameObject player_2;

    public GameObject boundaries_up;
    public GameObject boundaries_down;

    private float ball_speed = 0.0f;

    private int curr_dir = 1;
    private float curr_angle = 0;

    private bool top_disabled = false;
    private bool bottom_disabled = false;

    private bool can_move = false;
}