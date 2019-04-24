using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class QueueEventSpriteFade : QueueEvent
{
    public QueueEventSpriteFade(ComponentSpriteRenderer element, float starting_val, float ending_val, float fade_time,
        EasingFunctionsType movement_type, bool real_time = false) : base("fade")
    {
        this.movement_type = movement_type;
        this.affected_element = element;
        this.starting_val = starting_val;
        this.ending_val = ending_val;
        this.real_time = real_time;
        this.fade_time = fade_time;

        val_difference = ending_val - starting_val;
    }

    public override void OnStart()
    {
        Colour curr_col = affected_element.Colour;
        affected_element.Colour = new Colour(curr_col.r, curr_col.g, curr_col.b, starting_val);

        fade_timer.Start();
    }

    public override void OnUpdate()
    {
        float alpha = 0;

        float time = 0.0f;

        time = fade_timer.ReadTime();

        alpha = EasingFunctions.GetEasing(movement_type, val_difference, time, starting_val, fade_time);

        Colour curr_col = affected_element.Colour;
        affected_element.Colour = new Colour(curr_col.r, curr_col.g, curr_col.b, alpha);

        if (time >= fade_time)
        {
            curr_col = affected_element.Colour;
            affected_element.Colour = new Colour(curr_col.r, curr_col.g, curr_col.b, ending_val);

            Finish();
        }
    }

    private float starting_val = 0.0f;
    private float ending_val = 0.0f;
    private float val_difference = 0.0f;
    private float fade_time = 0.0f;
    private Timer fade_timer = new Timer();
    private ComponentSpriteRenderer affected_element = null;
    private EasingFunctionsType movement_type = new EasingFunctionsType();
    private bool real_time = false;
}