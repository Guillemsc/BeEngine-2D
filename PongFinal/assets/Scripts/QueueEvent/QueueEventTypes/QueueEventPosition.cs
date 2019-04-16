
using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class QueueEventPosition : QueueEvent
{
    public QueueEventPosition(GameObject element, Float2 starting_val, Float2 ending_val, float position_time,
        EasingFunctionsType movement_type, bool real_time = false) : base("scale")
    {
        this.movement_type = movement_type;
        this.affected_element = element;
        this.starting_val = starting_val;
        this.ending_val = ending_val;
        this.real_time = real_time;
        this.position_time = position_time;

        val_difference = new Float2(ending_val.x - starting_val.x, ending_val.y - starting_val.y);
    }

    public override void OnStart()
    {
        affected_element.Transform.Position = starting_val;

        position_timer.Start();
    }

    public override void OnUpdate()
    {
        float val_x = 0.0f;
        float val_y = 0.0f;

        float time = position_timer.ReadTime();

        val_x = EasingFunctions.GetEasing(movement_type, val_difference.x, time, starting_val.x, position_time);
        val_y = EasingFunctions.GetEasing(movement_type, val_difference.y, time, starting_val.y, position_time);

        affected_element.Transform.Position = new Float2(val_x, val_y);

        if (time >= position_time)
        {
            affected_element.Transform.Position = ending_val;

            Finish();
        }
    }

    private Float2 starting_val;
    private Float2 ending_val;
    private Float2 val_difference;
    private float position_time = 0.0f;
    private Timer position_timer = new Timer();
    private GameObject affected_element = null;
    private EasingFunctionsType movement_type = new EasingFunctionsType();
    private bool real_time = false;
}