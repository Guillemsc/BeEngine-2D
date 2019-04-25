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
    }
	
	public override void Update () 
	{
        UpdateMovement();
    }

    private void UpdateMovement()
    {
        if (can_move)
        {
            float speed = Time.DeltaTime * MainGameManager.Instance.GetBallSpeed();

            float angle_radiants = curr_angle * 0.0174532925199432957f;

            float add_x = (float)System.Math.Sin(angle_radiants) * speed;
            float add_y = (float)System.Math.Cos(angle_radiants) * speed;

            Float2 position_addition = new Float2(add_x, add_y);

            game_object.Transform.Position = new Float2(game_object.Transform.Position.x - position_addition.x,
                game_object.Transform.Position.y + position_addition.y);
        }
    }

    public void ResetBall(BallDirection dir)
    {
        game_object.Transform.Position = new Float2(0, 0);

        switch (dir)
        {
            case BallDirection.DIRECTION_LEFT:
                {
                    curr_angle = Random.Range(30.0f, 150.0f);
                    direction = dir;

                    break;
                }
            case BallDirection.DIRECTION_RIGHT:
                {
                    curr_angle = Random.Range(-30.0f, -150.0f);
                    direction = dir;
                    break;
                }
        }
    }

    public void SetCanMove(bool set)
    {
        can_move = set;
    }

    private void OnCollisionEnter(Collision coll)
    {
        if (coll.GameObjectCollidedWith == MainGameManager.Instance.GetLeftPlayer())
        {
            if (direction == BallDirection.DIRECTION_LEFT)
            {
                float center_diff = game_object.Transform.Position.y - MainGameManager.Instance.GetLeftPlayer().Transform.Position.y;

                float abs_center_diff = System.Math.Abs(center_diff);

                float player_height = MainGameManager.Instance.GetPlayerHeight();
                float player_half_height = player_height * 0.5f;

                float angle_to_set = -90.0f;

                if (center_diff > 0)
                {
                    float to_add = (50 * abs_center_diff) / player_half_height;

                    angle_to_set += to_add;
                }
                else
                {
                    float to_add = (50 * abs_center_diff) / player_half_height;

                    angle_to_set -= to_add;
                }

                curr_angle = angle_to_set;

                //curr_angle = Random.Range(-40.0f, -140.0f);
                direction = BallDirection.DIRECTION_RIGHT;
            }
        }

        if (coll.GameObjectCollidedWith == MainGameManager.Instance.GetRightPlayer())
        {
            if (direction == BallDirection.DIRECTION_RIGHT)
            {
                float center_diff = game_object.Transform.Position.y - MainGameManager.Instance.GetRightPlayer().Transform.Position.y;
                float abs_center_diff = System.Math.Abs(center_diff);

                float player_height = MainGameManager.Instance.GetPlayerHeight();
                float player_half_height = player_height * 0.5f;

                float angle_to_set = 90.0f;

                if (center_diff > 0)
                {
                    float to_add = (50 * abs_center_diff) / player_half_height;

                    angle_to_set -= to_add;
                }
                else
                {
                    float to_add = (50 * abs_center_diff) / player_half_height;

                    angle_to_set += to_add;
                }

                curr_angle = angle_to_set;
                direction = BallDirection.DIRECTION_LEFT;
            }
        }

        if (coll.GameObjectCollidedWith == MainGameManager.Instance.GetBoundariesUp())
        {
            if (direction == BallDirection.DIRECTION_LEFT)
            {
                curr_angle = 180 - curr_angle;
            }
            else if (direction == BallDirection.DIRECTION_RIGHT)
            {
                curr_angle = -180 - curr_angle;
            }
        }
        else if (coll.GameObjectCollidedWith == MainGameManager.Instance.GetBoundariesDown())
        {
            if (direction == BallDirection.DIRECTION_LEFT)
            {
                curr_angle = 180 - curr_angle;
            }
            else if (direction == BallDirection.DIRECTION_RIGHT)
            {
                curr_angle = -180 - curr_angle;
            }
        }

        if(coll.GameObjectCollidedWith == MainGameManager.Instance.GetGoalLeft())
        {
            MainGameManager.Instance.AddPointLeft();
        }
        else if (coll.GameObjectCollidedWith == MainGameManager.Instance.GetGoalRight())
        {
            MainGameManager.Instance.AddPointRight();
        }
    }

    private BallDirection direction;

    private float curr_angle = 0;

    private bool can_move = false;
}