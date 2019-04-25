#ifndef __SCRIPTING_CLUSTER_H__
#define __SCRIPTING_CLUSTER_H__

#include "ModuleScripting.h"

class PhysicsBody;

enum ScriptFieldType
{
	SCRIPT_FIELD_UNDEFINED,
	SCRIPT_FIELD_INT,
	SCRIPT_FIELD_FLOAT,
	SCRIPT_FIELD_STRING,
	SCRIPT_FIELD_BOOL,
	SCRIPT_FIELD_GAMEOBJECT,
	SCRIPT_FIELD_RESOURCE_PREFAB,
	SCRIPT_FIELD_RESOURCE_SCENE,
};

class ScriptingCluster 
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

private:
	void RegisterInternalCalls();
	void RebuildClasses();
	void CleanUp();

public:
	ScriptFieldType GetScriptFieldTypeFromName(const std::string& name);

	// Unboxing
	float2 UnboxFloat2(MonoObject* obj);
	float4 UnboxColour(MonoObject* obj);

	// Boxing
	MonoObject* BoxFloat2(const float2& val);
	MonoObject* BoxColour(const float4& val);

	MonoObject* BoxCollision(PhysicsBody* pb);

public:
	ScriptingClass* beengine_object_class = nullptr;

	ScriptingClass* debug_class = nullptr;

	ScriptingClass* application_class = nullptr;

	ScriptingClass* input_class = nullptr;

	ScriptingClass* time_class = nullptr;

	ScriptingClass* scene_class = nullptr;

	ScriptingClass* float2_class = nullptr;

	ScriptingClass* colour_class = nullptr;

	ScriptingClass* collision_class = nullptr;

	ScriptingClass* game_object_class = nullptr;

	ScriptingClass* component_class = nullptr;
	ScriptingClass* component_script_class = nullptr;
	ScriptingClass* component_transform_class = nullptr;
	ScriptingClass* component_button_class = nullptr;
	ScriptingClass* component_sprite_renderer_class = nullptr;
	ScriptingClass* component_camera_class = nullptr;
	ScriptingClass* component_canvas_class = nullptr;
	ScriptingClass* component_text_class = nullptr;
	ScriptingClass* component_physics_body_class = nullptr;
	ScriptingClass* component_polygon_collider_class = nullptr;

	ScriptingClass* resource_class = nullptr;
	ScriptingClass* resource_texture_class = nullptr;
	ScriptingClass* resource_script_class = nullptr;
	ScriptingClass* resource_scene_class = nullptr;
	ScriptingClass* resource_prefab_class = nullptr;
	ScriptingClass* resource_font_class = nullptr;
	ScriptingClass* resource_audio_class = nullptr;

	ScriptingClass* show_on_inspector_attribute_class = nullptr;

private:
	bool rebuild_internal_calls = true;
};

#endif // !__SCRIPTING_CLUSTER_H__