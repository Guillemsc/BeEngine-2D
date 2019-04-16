using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class Singleton<T> : ComponentScript where T : ComponentScript
{
    protected void InitInstance(T _instance)
    {
        if (_instance != null)
            instance = _instance;
    }

    public static T Instance
    {
        get
        {
            if (instance != null)
            {
                return instance;
            }
            else
            {
                return null;
            }
        }
    }

    public static bool Valid()
    {
        return instance != null;
    }

    private static T instance;
}