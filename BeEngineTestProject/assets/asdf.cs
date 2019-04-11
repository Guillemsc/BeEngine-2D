using System.Collections;
using System.Collections.Generic;
using BeEngine;

public class asdf : ComponentScript 
{
	public override void Start () 
	{
        GameObject go = Scene.LoadPrefab(pref);

        go.Name = "This is a prefab";
    }
	
	public override void Update () 
	{
        Scene.DestroyComponent(game_object.GetComponent<ComponentText>());
    }

    public void OnClick(ComponentButton but)
    {
        //Debug.LOG("Click");
    }

    GameObject new_go;

    public ResourcePrefab pref;

    asdf comp;
}