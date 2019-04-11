using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class asdf : ComponentScript 
{
	public override void Start () 
	{
        Scene.LoadScene(scene);
    }
	
	public override void Update () 
	{
       
    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }

    GameObject new_go;

    public ResourceScene scene;

    asdf comp;
}