#include "ScriptingCluster.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingBridgeGameObject.h"
#include "ScriptingBridgeComponentTransform.h"
#include "ScriptingBridgeDebug.h"
#include "PhysicsBody.h"
#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "ScriptingBridgeInput.h"
#include "ScriptingBridgeTime.h"
#include "ScriptingBridgeComponentText.h"
#include "ScriptingBridgeScene.h"
#include "ScriptingBridgeComponentCamera.h"
#include "ScriptingBridgeApplication.h"
#include "ScriptingBridgeComponentSpriteRenderer.h"

void ScriptingCluster::RegisterInternalCalls()
{
	if (rebuild_internal_calls)
	{
		rebuild_internal_calls = false;

		// Debug ------------------------------------
		mono_add_internal_call("BeEngine.Debug::ConsoleLog", (const void*)ScriptingBridgeDebug::Log);
		// ------------------------------------ Debug

		// Application ------------------------------
		mono_add_internal_call("BeEngine.Application::ApplicationQuit", (const void*)ScriptingBridgeApplication::ApplicationQuit);
		// ------------------------------ Application

		// Input ------------------------------------
		mono_add_internal_call("BeEngine.Input::GetKeyDown", (const void*)ScriptingBridgeInput::GetKeyDown);
		mono_add_internal_call("BeEngine.Input::GetKeyRepeat", (const void*)ScriptingBridgeInput::GetKeyRepeat);
		mono_add_internal_call("BeEngine.Input::GetKeyUp", (const void*)ScriptingBridgeInput::GetKeyUp);
		// ------------------------------------ Input

		// Time ------------------------------------
		mono_add_internal_call("BeEngine.Time::GetDeltaTime", (const void*)ScriptingBridgeTime::GetDeltaTime);
		mono_add_internal_call("BeEngine.Time::GetTimeSinceStart", (const void*)ScriptingBridgeTime::GetTimeSinceStart);
		// ------------------------------------- Time

		// Scene -----------------------------------
		mono_add_internal_call("BeEngine.Scene::SceneCreateGameObject", (const void*)ScriptingBridgeScene::CreateGameObject);
		mono_add_internal_call("BeEngine.Scene::SceneDestroyGameObject", (const void*)ScriptingBridgeScene::DestroyGameObject);
		mono_add_internal_call("BeEngine.Scene::SceneDestroyComponent", (const void*)ScriptingBridgeScene::DestroyComponent);
		mono_add_internal_call("BeEngine.Scene::SceneLoadPrefab", (const void*)ScriptingBridgeScene::LoadPrefab);
		mono_add_internal_call("BeEngine.Scene::SceneLoadScene", (const void*)ScriptingBridgeScene::LoadScene);
		// ----------------------------------- Scene

		// GameObject -------------------------------
		mono_add_internal_call("BeEngine.GameObject::SetName", (const void*)ScriptingBridgeGameObject::SetName);
		mono_add_internal_call("BeEngine.GameObject::GetName", (const void*)ScriptingBridgeGameObject::GetName);
		mono_add_internal_call("BeEngine.GameObject::SetActive", (const void*)ScriptingBridgeGameObject::SetActive);
		mono_add_internal_call("BeEngine.GameObject::GetActive", (const void*)ScriptingBridgeGameObject::GetActive);
		mono_add_internal_call("BeEngine.GameObject::AddComponentTypeName", (const void*)ScriptingBridgeGameObject::AddComponent);
		mono_add_internal_call("BeEngine.GameObject::GetComponentTypeName", (const void*)ScriptingBridgeGameObject::GetComponent);
		// ------------------------------- GameObject

		// ComponentTransform -----------------------
		mono_add_internal_call("BeEngine.ComponentTransform::SetPosition", (const void*)ScriptingBridgeComponentTransform::SetPosition);
		mono_add_internal_call("BeEngine.ComponentTransform::GetPosition", (const void*)ScriptingBridgeComponentTransform::GetPosition);
		mono_add_internal_call("BeEngine.ComponentTransform::SetRotationDegrees", (const void*)ScriptingBridgeComponentTransform::SetRotationDegrees);
		mono_add_internal_call("BeEngine.ComponentTransform::GetRotationDegrees", (const void*)ScriptingBridgeComponentTransform::GetRotationDegrees);
		mono_add_internal_call("BeEngine.ComponentTransform::SetScale", (const void*)ScriptingBridgeComponentTransform::SetScale);
		mono_add_internal_call("BeEngine.ComponentTransform::GetScale", (const void*)ScriptingBridgeComponentTransform::GetScale);
		// ----------------------- ComponentTransform

		// ComponentSpriteRenderer -----------------------
		mono_add_internal_call("BeEngine.ComponentSpriteRenderer::SetSpriteColour", (const void*)ScriptingBridgeComponentSpriteRenderer::SetSpriteColour);
		mono_add_internal_call("BeEngine.ComponentSpriteRenderer::GetSpriteColour", (const void*)ScriptingBridgeComponentSpriteRenderer::GetSpriteColour);
		// ----------------------- ComponentText

	    // ComponentText -----------------------
		mono_add_internal_call("BeEngine.ComponentText::SetText", (const void*)ScriptingBridgeComponentText::SetText);
		mono_add_internal_call("BeEngine.ComponentText::GetText", (const void*)ScriptingBridgeComponentText::GetText);
		mono_add_internal_call("BeEngine.ComponentText::SetTextColour", (const void*)ScriptingBridgeComponentText::SetTextColour);
		mono_add_internal_call("BeEngine.ComponentText::GetTextColour", (const void*)ScriptingBridgeComponentText::GetTextColour);
		// ----------------------- ComponentText

		// ComponentCamera -----------------------
		mono_add_internal_call("BeEngine.ComponentCamera::GetViewUpPosition", (const void*)ScriptingBridgeComponentCamera::GetViewUpPosition);
		mono_add_internal_call("BeEngine.ComponentCamera::GetViewDownPosition", (const void*)ScriptingBridgeComponentCamera::GetViewDownPosition);
		mono_add_internal_call("BeEngine.ComponentCamera::GetViewLeftPosition", (const void*)ScriptingBridgeComponentCamera::GetViewLeftPosition);
		mono_add_internal_call("BeEngine.ComponentCamera::GetViewRightPosition", (const void*)ScriptingBridgeComponentCamera::GetViewRightPosition);
		// ----------------------- ComponentText
	}
}

void ScriptingCluster::RebuildClasses()
{
	if (App->scripting->scripting_assembly != nullptr && App->scripting->scripting_assembly->GetAssemblyLoaded())
	{
		// BeEngineObject
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "BeEngineObject", beengine_object_class);

		// Debug
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Debug", debug_class);

		// Application
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Application", application_class);

		// Input
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Input", input_class);

		// Time
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Time", time_class);

		// Scene
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Scene", scene_class);

		// Math
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Float2", float2_class);

		// Colour
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Colour", colour_class);

		// Physics
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Collision", collision_class);

		// GameObject
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "GameObject", game_object_class);

		// Components
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Component", component_class);

		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentScript", component_script_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentTransform", component_transform_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentButton", component_button_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentSpriteRenderer", component_sprite_renderer_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentCamera", component_camera_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentPhysicsBody", component_physics_body_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentPolygonCollider", component_polygon_collider_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentCanvas", component_canvas_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ComponentText", component_text_class);

		// Resources
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "Resource", resource_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ResourceTexture", resource_texture_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ResourceScript", resource_script_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ResourceScene", resource_scene_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ResourcePrefab", resource_prefab_class);
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ResourceFont", resource_font_class);

		// Attributes
		App->scripting->scripting_assembly->UpdateClassPointer("BeEngine", "ShowOnInspector", show_on_inspector_attribute_class);
	}
}

void ScriptingCluster::CleanUp()
{
	RELEASE(beengine_object_class);

	RELEASE(debug_class);
	RELEASE(application_class);
	RELEASE(input_class);
	RELEASE(time_class);
	RELEASE(scene_class);

	RELEASE(float2_class);
	RELEASE(colour_class);
	RELEASE(collision_class);

	RELEASE(game_object_class);

	RELEASE(component_class);
	RELEASE(component_script_class);
	RELEASE(component_transform_class);
	RELEASE(component_button_class);
	RELEASE(component_sprite_renderer_class);
	RELEASE(component_camera_class);
	RELEASE(component_text_class);
	RELEASE(component_canvas_class);
	RELEASE(component_physics_body_class);
	RELEASE(component_polygon_collider_class);

	RELEASE(resource_class);
	RELEASE(resource_texture_class);
	RELEASE(resource_script_class);
	RELEASE(resource_scene_class);
	RELEASE(resource_prefab_class);
	RELEASE(resource_font_class);

	RELEASE(show_on_inspector_attribute_class);
}

ScriptFieldType ScriptingCluster::GetScriptFieldTypeFromName(const std::string & name)
{
	ScriptFieldType ret = ScriptFieldType::SCRIPT_FIELD_UNDEFINED;

	if (name.compare("System.String") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_STRING;
	}
	else if (name.compare("System.Int32") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_INT;
	}
	else if (name.compare("System.Single") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_FLOAT;
	}
	else if (name.compare("System.Boolean") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_BOOL;
	}
	else if (name.compare("BeEngine.GameObject") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT;
	}
	else if (name.compare("BeEngine.ResourcePrefab") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB;
	}
	else if (name.compare("BeEngine.ResourceScene") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE;
	}

	return ret;
}

float2 ScriptingCluster::UnboxFloat2(MonoObject* obj)
{
	float2 ret = float2::zero;

	if (obj != nullptr)
	{
		MonoObject* x_val = App->scripting->GetFieldValue(obj, float2_class->GetMonoClass(), "_x");
		MonoObject* y_val = App->scripting->GetFieldValue(obj, float2_class->GetMonoClass(), "_y");

		if (x_val != nullptr && y_val != nullptr)
		{
			ret.x = App->scripting->UnboxFloat(x_val);
			ret.y = App->scripting->UnboxFloat(y_val);
		}
	}

	return ret;
}

float4 ScriptingCluster::UnboxColour(MonoObject * obj)
{
	float4 ret = float4::zero;

	if (obj != nullptr)
	{
		MonoObject* r_val = App->scripting->GetFieldValue(obj, colour_class->GetMonoClass(), "_r");
		MonoObject* g_val = App->scripting->GetFieldValue(obj, colour_class->GetMonoClass(), "_g");
		MonoObject* b_val = App->scripting->GetFieldValue(obj, colour_class->GetMonoClass(), "_b");
		MonoObject* a_val = App->scripting->GetFieldValue(obj, colour_class->GetMonoClass(), "_a");

		if (r_val != nullptr && g_val != nullptr && b_val != nullptr && a_val != nullptr)
		{
			ret.x = App->scripting->UnboxFloat(r_val) / 255.0f;
			ret.y = App->scripting->UnboxFloat(g_val) / 255.0f;
			ret.w = App->scripting->UnboxFloat(b_val) / 255.0f;
			ret.z = App->scripting->UnboxFloat(a_val) / 255.0f;
		}
	}

	return ret;
}

MonoObject * ScriptingCluster::BoxFloat2(const float2 & val)
{
	MonoObject* ret = nullptr;

	if (float2_class != nullptr)
	{
		ScriptingClassInstance ins = float2_class->CreateWeakInstance();

		ins.SetFieldValue("_x", &(float)val.x);
		ins.SetFieldValue("_y", &(float)val.y);

		ret = ins.GetMonoObject();
	}

	return ret;
}

MonoObject * ScriptingCluster::BoxColour(const float4 & val)
{
	MonoObject* ret = nullptr;

	if (colour_class != nullptr)
	{
		ScriptingClassInstance ins = colour_class->CreateWeakInstance();

		float4 to_255 = float4(val.x * 255, val.y * 255, val.z * 255, val.w * 255);

		ins.SetFieldValue("_r", &(float)to_255.x);
		ins.SetFieldValue("_g", &(float)to_255.y);
		ins.SetFieldValue("_b", &(float)to_255.w);
		ins.SetFieldValue("_a", &(float)to_255.z);

		ret = ins.GetMonoObject();
	}

	return ret;
}

MonoObject* ScriptingCluster::BoxCollision(PhysicsBody * pb)
{
	MonoObject* ret = nullptr;

	if (pb != nullptr)
	{
		ComponentTransform* component = pb->GetComponentTransform();

		if (component != nullptr)
		{
			GameObject* collided_go = component->GetOwner();

			ScriptingClassInstance* collided_go_instance = collided_go->GetScriptingBridge()->GetInstance();

			ScriptingClassInstance instance = collision_class->CreateWeakInstance();

			if (App->scripting->SetFieldValue(instance.GetMonoObject(), collision_class->GetMonoClass(),
				"_go_collided", collided_go_instance->GetMonoObject()))
			{
				ret = instance.GetMonoObject();
			}
		}
	}

	return ret;
}
