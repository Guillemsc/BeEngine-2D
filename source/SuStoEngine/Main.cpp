#include <stdlib.h>
#include "App.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum MainState
{
	MAIN_CREATION,
	MAIN_AWAKE,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_CLEAN,
	MAIN_EXIT
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	INTERNAL_LOG("Engine starting");

	int main_return = EXIT_FAILURE;
	MainState state = MainState::MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MainState::MAIN_CREATION:

			INTERNAL_LOG("CREATION PHASE ===============================");
			App = new Application(argc, argv);
			state = MainState::MAIN_AWAKE;
			break;

		case MainState::MAIN_AWAKE:

			INTERNAL_LOG("AWAKE PHASE ===============================");
			if (App->Awake() == false)
			{
				INTERNAL_LOG("Application Awake exits with ERROR");
				state = MainState::MAIN_EXIT;
			}
			else
			{
				state = MainState::MAIN_START;
			}

			break;

		case MainState::MAIN_START:

			INTERNAL_LOG("START PHASE ===============================");
			if (App->Start() == false)
			{
				INTERNAL_LOG("Application Start exits with ERROR");
				state = MainState::MAIN_EXIT;
			}
			else
			{
				state = MainState::MAIN_UPDATE;
				INTERNAL_LOG("UPDATE PHASE ===============================");
			}

			break;

		case MainState::MAIN_UPDATE:

			if (App->Update() == false)
			{
				state = MainState::MAIN_CLEAN;
			}

			break;

		case MainState::MAIN_CLEAN:

			INTERNAL_LOG("CLEANUP PHASE ===============================");
			if (App->CleanUp() == false)
			{
				INTERNAL_LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MainState::MAIN_EXIT;

			break;

		}
	}

	INTERNAL_LOG("Exiting engine");
	delete App;

	return main_return;
}