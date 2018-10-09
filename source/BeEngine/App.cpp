#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFileSystem.h"
#include "ModuleJson.h"
#include "ModuleShader.h"
#include "ModuleEditor.h"
#include "imgui.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	// Profiler
	profiler = new Profiler();

	CreateProfiles();

	// Create modules
	json = new ModuleJson();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	shader = new ModuleShader();
	editor = new ModuleEditor();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Add modules
	AddModule(json, "Module JSON");
	AddModule(window, "Module Window");
	AddModule(camera, "Module Camera");
	AddModule(input, "Module Input");
	AddModule(audio, "Module Audio");
	AddModule(audio, "Module Audio");
	AddModule(shader, "Module Shader");

	// Renderers
	AddModule(renderer3D, "Module Renderer");
	AddModule(editor, "Module Editor");
}

Application::~Application()
{
}

void Application::CreateProfiles()
{
	prof_app_awake = profiler->AddStartProfile("App Awake");
	prof_app_start = profiler->AddStartProfile("App Start");
	prof_app_total_update = profiler->AddUpdateProfile("App Total Update");
	prof_app_preupdate = profiler->AddUpdateProfile("App Modules PreUpdate");
	prof_app_update = profiler->AddUpdateProfile("App Modules Update");
	prof_app_postupdate = profiler->AddUpdateProfile("App Modules PostUpdate");
}

bool Application::Awake()
{
	bool ret = true;

	window->GetWindowNamer()->AddNamePart("app_name", "");
	window->GetWindowNamer()->AddNamePart("app_version", "");

	prof_app_awake->Start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->prof_module_awake->Start();

		ret = (*it)->Awake();

		(*it)->prof_module_awake->Finish();

		if (!ret) 
			return false;
	}

	LoadConfig();

	prof_app_awake->Finish();

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	prof_app_start->Start();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->prof_module_start->Start();

		ret = (*it)->Start();

		(*it)->prof_module_start->Finish();

		if (!ret) 
			return false;
	}

	prof_app_start->Finish();

	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;

	prof_app_total_update->Start();

	profiler->AppUpdateStart();

	prof_app_preupdate->Start();

	// PreUpdate
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		(*it)->prof_module_preupdate->Start();

		ret = (*it)->PreUpdate();

		(*it)->prof_module_preupdate->Finish();

		if (!ret) return false;
	}

	prof_app_preupdate->Finish();

	prof_app_update->Start();

	// Update
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		(*it)->prof_module_update->Start();

		ret = (*it)->Update();

		(*it)->prof_module_update->Finish();

		if (!ret)
			return false;
	}

	prof_app_update->Finish();

	prof_app_postupdate->Start();

	// PostUpdate
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!ret)
			break;
		
		(*it)->prof_module_postupdate->Start();

		if ((*it)->GetEnabled())
			ret = (*it)->PostUpdate();

		(*it)->prof_module_postupdate->Finish();
	}

	prof_app_postupdate->Finish();

	if (input->GetWindowEvent(WE_QUIT) == true || to_quit)
		ret = false;

	prof_app_total_update->Finish();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveConfig();

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
	{
		ret = (*it)->CleanUp();

		if (!ret) 
			return false;
	}

	profiler->CleanUp();
	RELEASE(profiler);

	return ret;
}

int Application::GetArgc() const
{
	return argc;
}

const char * Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return nullptr;
}

void Application::LoadConfig()
{
	config = json->LoadJSON("config.json");

	if (config != nullptr)
	{
		config->MoveToRoot();

		const char* title = config->GetString("app.title", "No title");
		const char* organization = config->GetString("app.organization", "No org");
		const char* version = config->GetString("app.version", "No version");
		int max_fps = config->GetNumber("app.max_fps", 60);

		SetAppName(title);
		SetAppOrganization(organization);
		SetVersion(version);
		SetMaxFps(max_fps);

		for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		{
			(*it)->OnLoadConfig(config);
		}
	}
}

void Application::SaveConfig(Module* module)
{
	if (config != nullptr)
	{
		config->MoveToRoot();

		config->SetString("app.title", App->GetAppName());
		config->SetString("app.organization", App->GetAppOrganization());
		config->SetNumber("app.max_fps", App->GetMaxFps());
		config->SetString("app.version", App->GetVersion());

		for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		{
			if(module == nullptr)
				(*it)->OnSaveConfig(config);

			else if(module == (*it))
				(*it)->OnSaveConfig(config);	
		}

		config->Save();
	}
}

void Application::Quit()
{
	to_quit = true;
}

float Application::GetDT() const
{
	return profiler->GetFrameTime() / 1000;
}

void Application::SetAppName(const char* name)
{	
	title = name;

	window->GetWindowNamer()->UpdateNamePart("app_name", title);
}

const char * Application::GetAppName() const
{
	return title.c_str();
}

void Application::SetAppOrganization(const char* name)
{	
	organization = name;
}

const char * Application::GetAppOrganization()
{
	return organization.c_str();
}

void Application::SetMaxFps(int const &set)
{
	profiler->SetMaxFps(set);
}

int Application::GetMaxFps() const
{
	return profiler->GetMaxFps();
}

void Application::SetVersion(const char * set)
{
	version = set;

	window->GetWindowNamer()->UpdateNamePart("app_version", version);
}

const char * Application::GetVersion() const
{
	return version.c_str();;
}

SDL_version Application::GetSDLVersion()
{
	SDL_version version;
	SDL_GetVersion(&version);
	return version;
}

const char * Application::GetBasePath()
{
	return SDL_GetBasePath();
}

void Application::OpenBrowser(const char* url)
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::OpenFolder(const char * folder)
{
	ShellExecute(NULL, "open", folder, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::AddInternalLog(const char * log)
{
	if(log != nullptr)
		internal_logs.push_back(log);
}

std::vector<const char*> Application::GetInternalLogs() const
{
	return internal_logs;
}

void Application::ClearInternalLogs()
{
	internal_logs.clear();
}

void Application::AddConsoleLog(ConsoleLogLine text)
{
	console_logs.push_back(text);
}

std::list<ConsoleLogLine> Application::GetConsoleLogs() const
{
	return console_logs;
}

void Application::ClearConsoleLogs()
{
	console_logs.clear();
}

void Application::AddModule(Module* mod, const char* name)
{
	if (mod != nullptr)
	{
		mod->SetName(name);

		mod->CreateProfiles();

		mod->prof_module_awake = prof_app_awake->AddProfileChild("Awake " + std::string(name));
		mod->prof_module_start = prof_app_start->AddProfileChild("Start " + std::string(name));
		mod->prof_module_preupdate = prof_app_preupdate->AddProfileChild("PreUpdate " + std::string(name));
		mod->prof_module_update = prof_app_update->AddProfileChild("Update " + std::string(name));
		mod->prof_module_postupdate = prof_app_postupdate->AddProfileChild("PostUpdate " + std::string(name));

		modules.push_back(mod);
	}
}