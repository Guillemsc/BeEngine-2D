using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class GameManager : ComponentScript 
{
	public override void Start () 
	{
        ball_movement = ball.GetComponent<BallMovement>();
        ball_movement.SetSpeed(wall_speed);
        ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_LEFT);

        ball_movement.game_object.SuscribeToOnCollisionEnter(BallOnCollisionEnter);

        game_timer.Start();
    }
	
	public override void Update () 
	{
        if (!game_started)
        {
            if (game_timer.ReadTime() > 1)
            {
                ball_movement.StartMoving();
                game_started = true;
            }
        }
    }

    private void BallOnCollisionEnter(Collision coll)
    {
        if(coll.GameObjectCollidedWith == left_score)
        {
            ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_RIGHT);

            ball_movement.StopMoving();
            game_timer.Start();
            game_started = false;
        }

        if (coll.GameObjectCollidedWith == right_score)
        {
            ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_LEFT);

            ball_movement.StopMoving();
            game_timer.Start();
            game_started = false;
        }
    }

    public float wall_speed = 0;

    public GameObject ball = null;

    public GameObject left_score;
    public GameObject right_score;

    private BallMovement ball_movement = null;

    private Timer game_timer = new Timer();
    private bool game_started = false;
}