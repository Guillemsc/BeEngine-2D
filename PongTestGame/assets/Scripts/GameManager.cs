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
        ball_movement.StartMoving();

        ball_movement.game_object.SuscribeToOnCollisionEnter(BallOnCollisionEnter);
    }
	
	public override void Update () 
	{
    }

    private void BallOnCollisionEnter(Collision coll)
    {
        if(coll.GameObjectCollidedWith == left_score)
        {
            ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_RIGHT);
        }

        if (coll.GameObjectCollidedWith == right_score)
        {
            ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_LEFT);
        }
    }

    public float wall_speed = 0;

    public GameObject ball = null;

    public GameObject left_score;
    public GameObject right_score;

    private BallMovement ball_movement = null;
}