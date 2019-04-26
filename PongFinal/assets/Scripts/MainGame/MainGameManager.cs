using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class MainGameManager : Singleton<MainGameManager>
{
    MainGameManager()
    {
        InitInstance(this);
    }

    public override void Start () 
	{
        queue_event = QueueEventManager.Instance.CreateContext();

        if(foreground_go != null)
        {
            foreground = foreground_go.GetComponent<ComponentSpriteRenderer>();

            Colour curr_col = foreground.Colour;
            foreground.Colour = new Colour(curr_col.r, curr_col.g, curr_col.b, 255);
        }

        if(countdown_text_go != null)
        {
            countdown_text = countdown_text_go.GetComponent<ComponentText>();
        }

        if(left_score_go != null)
        {
            left_score = left_score_go.GetComponent<ComponentText>();
            left_score.Text = "0";
        }

        if(right_score_go != null)
        {
            right_score = right_score_go.GetComponent<ComponentText>();
            right_score.Text = "0";
        }

        if(left_player_go != null)
        {
            left_player_movement = left_player_go.GetComponent<PlayerMovement>();
            left_player_movement.SetCanMove(false);
        }

        if (right_player_go != null)
        {
            right_player_movement = right_player_go.GetComponent<PlayerMovement>();
            right_player_movement.SetCanMove(false);
        }

        if(ball_go != null)
        {
            ball_movement = ball_go.GetComponent<BallMovement>();
            ball_movement.SetCanMove(false);
        }

        countdown_go.Active = false;

        controls_go.Active = true;

        Colour curr_colour = foreground.Colour;
        foreground.Colour = new Colour(curr_colour.r, curr_colour.g, curr_colour.b, 255);

        PlayFadeInAnimation();
    }
	
	public override void Update () 
	{
        UpdateCountDown();

        UpdateGoalTime();

        UpdateIncrementalSpeed();
    }

    private void UpdateCountDown()
    {
        if (on_countdown)
        {
            if (countdown_time > countdown_timer.ReadTime())
            {
                int time_passed = (int)countdown_timer.ReadTime();
                int time_left = countdown_time - time_passed - 1;
                countdown_text.Text = time_left.ToString();
            }
            else
            {
                on_countdown = false;
                countdown_go.Active = false;
                countdown_timer.Reset();

                left_player_movement.SetCanMove(true);
                right_player_movement.SetCanMove(true);

                controls_go.Active = false;

                ball_speed = ball_starting_speed;
                ball_movement.SetCanMove(true);
                ball_movement.ResetBall((BallMovement.BallDirection)Random.Range(0.0f, 1.99f));
                increment_speed_timer.Start();
            }
        }

        UpdateIncrementalSpeed();
    }

    private void UpdateGoalTime()
    {
        if(waiting_for_goal)
        {
            if(goal_timer.ReadTime() > 2)
            {
                goal_timer.Reset();
                waiting_for_goal = false;

                ball_movement.SetCanMove(true);
            }
        }
    }

    private void UpdateIncrementalSpeed()
    {
        if (increment_speed_timer.ReadTime() > increment_speed_time)
        {
            increment_speed_timer.Start();

            ball_speed += increment_speed;
        }
    }

    private void PlayFadeInAnimation()
    {
        if (foreground != null)
        {
            queue_event.PushEvent(new QueueEventSpriteFade(foreground, 255,
             0, 1.3f, EasingFunctionsType.QUAD_IN));

            queue_event.LastPushedEventOnFinish(OnFadInFinished);
        }
    }

    private void PlayFadeOutAnimation()
    {
        if (foreground != null)
        {
            queue_event.PushEvent(new QueueEventSpriteFade(foreground, 0,
                255, 1.3f, EasingFunctionsType.QUAD_OUT));
        }
    }

    private void OnFadInFinished(QueueEvent ev)
    {
        countdown_timer.Start();
        on_countdown = true;
        countdown_go.Active = true;
        countdown_text.Text = (countdown_time - 1).ToString();
    }

    public void AddPointLeft()
    {
        ++points_right;

        right_score.Text = points_right.ToString();

        ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_LEFT);
        ball_movement.SetCanMove(false);
        
        if (!CheckWin())
        {
            ball_speed = ball_starting_speed;

            goal_timer.Start();
            waiting_for_goal = true;
        }
    }

    public void AddPointRight()
    {
        ++points_left;

        left_score.Text = points_left.ToString();

        ball_movement.ResetBall(BallMovement.BallDirection.DIRECTION_RIGHT);
        ball_movement.SetCanMove(false);
       
        if (!CheckWin())
        {
            ball_speed = ball_starting_speed;

            goal_timer.Start();
            waiting_for_goal = true;
        }
    }

    private bool CheckWin()
    {
        bool ret = false;

        if(points_right >= 3)
        {
            PlayerWinsAnimation();

            ComponentText t = winner_text_go.GetComponent<ComponentText>();
            t.Text = "Player 2 wins";

            ret = true;
        }
        else if (points_left >= 3)
        {
            PlayerWinsAnimation();

            ComponentText t = winner_text_go.GetComponent<ComponentText>();
            t.Text = "Player 1 wins";

            ret = true;
        }

        return ret;
    }
    
    private void PlayerWinsAnimation()
    {
        ComponentSpriteRenderer background = player_wins_background_go.GetComponent<ComponentSpriteRenderer>();

        player_wins_text_go.Transform.Position = new Float2(enter_pos.Transform.Position.x, player_wins_text_go.Transform.Position.y);

        player_wins_parent.Active = true;

        Colour new_col = new Colour(background.Colour.r, background.Colour.g, background.Colour.b, 0);
        background.Colour = new_col;
        queue_event.PushEvent(new QueueEventSpriteFade(background, 0,
            230, 0.6f, EasingFunctionsType.QUAD_IN));

        queue_event.PushEvent(new QueueEventPosition(player_wins_text_go, new Float2(enter_pos.Transform.Position.x, player_wins_text_go.Transform.Position.y),
            new Float2(camera_go.Transform.Position.x, player_wins_text_go.Transform.Position.y), 0.8f, EasingFunctionsType.QUAD_OUT));

        queue_event.PushEvent(new QueueEventWaitTime(3));

        queue_event.PushEvent(new QueueEventPosition(player_wins_text_go, new Float2(camera_go.Transform.Position.x, player_wins_text_go.Transform.Position.y),
            new Float2(out_pos.Transform.Position.x, player_wins_text_go.Transform.Position.y), 0.8f, EasingFunctionsType.QUAD_IN));

        queue_event.PushEvent(new QueueEventSpriteFade(foreground, 0,
            255, 1.3f, EasingFunctionsType.QUAD_OUT));

        queue_event.LastPushedEventOnFinish(OnWinnerAnimationFinished);
    }

    private void OnWinnerAnimationFinished(QueueEvent ev)
    {
        Scene.LoadScene(main_menu_scene);
    }

    public float GetBallSpeed()
    {
        return ball_speed;
    }

    public GameObject GetLeftPlayer()
    {
        return left_player_go;
    }

    public GameObject GetRightPlayer()
    {
        return right_player_go;
    }

    public GameObject GetBoundariesUp()
    {
        return boundaries_up;
    }

    public GameObject GetBoundariesDown()
    {
        return boundaries_down;
    }

    public GameObject GetGoalLeft()
    {
        return goal_left;
    }

    public GameObject GetGoalRight()
    {
        return goal_right;
    }

    public float GetPlayerHeight()
    {
        return player_height;
    }

    [ShowOnInspector]
    private GameObject enter_pos;

    [ShowOnInspector]
    private GameObject out_pos;

    [ShowOnInspector]
    private GameObject camera_go = null;

    [ShowOnInspector]
    private GameObject foreground_go = null;

    [ShowOnInspector]
    private GameObject countdown_go = null;

    [ShowOnInspector]
    private GameObject countdown_text_go = null;

    [ShowOnInspector]
    private int countdown_time = 0;

    [ShowOnInspector]
    private GameObject controls_go = null;

    [ShowOnInspector]
    private GameObject left_score_go = null;

    [ShowOnInspector]
    private GameObject right_score_go = null;

    [ShowOnInspector]
    private GameObject left_player_go = null;

    [ShowOnInspector]
    private GameObject right_player_go = null;

    [ShowOnInspector]
    private float player_height = 0.0f;

    [ShowOnInspector]
    private GameObject ball_go;

    [ShowOnInspector]
    private GameObject boundaries_up = null;

    [ShowOnInspector]
    private GameObject boundaries_down = null;

    [ShowOnInspector]
    private GameObject goal_left = null;

    [ShowOnInspector]
    private GameObject goal_right = null;

    [ShowOnInspector]
    private float ball_starting_speed = 0.0f;

    [ShowOnInspector]
    private float increment_speed_time = 0.0f;

    [ShowOnInspector]
    private float increment_speed = 0.0f;

    [ShowOnInspector]
    private GameObject player_wins_parent = null;

    [ShowOnInspector]
    private GameObject player_wins_background_go = null;

    [ShowOnInspector]
    private GameObject player_wins_text_go = null;

    [ShowOnInspector]
    private GameObject winner_text_go = null;

    [ShowOnInspector]
    private ResourceScene main_menu_scene = null;

    private ComponentSpriteRenderer foreground = null;

    private ComponentText countdown_text = null;
    private Timer countdown_timer = new Timer();
    private bool on_countdown = false;

    private ComponentText left_score = null;
    private ComponentText right_score = null;
    private int points_left = 0;
    private int points_right = 0;

    private PlayerMovement left_player_movement = null;
    private PlayerMovement right_player_movement = null;

    private BallMovement ball_movement = null;
    private Timer goal_timer = new Timer();
    private bool waiting_for_goal = false;

    private Timer increment_speed_timer = new Timer();
    private float ball_speed = 0.0f;

    private QueueEventContext queue_event = null;
}