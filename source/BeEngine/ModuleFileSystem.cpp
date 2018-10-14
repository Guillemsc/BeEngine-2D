#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <ctype.h>
#include "Functions.h"
#include "tinyfiledialogs.h"

FileSystem::FileSystem() : Module()
{

}

FileSystem::~FileSystem()
{
}

bool FileSystem::Awake()
{
	bool ret = true;

	return ret;
}

bool FileSystem::Start()
{
	bool ret = true;

	FolderWatch(assets_path.c_str(), Changed, true);

	return ret;
}

bool FileSystem::Update()
{
	bool ret = true;

	//for (std::vector<WatchingFloder>::iterator it = watching_folders.begin(); it != watching_folders.end(); ++it)
	//{
	//	std::experimental::filesystem::file_time_type time_type = std::experimental::filesystem::last_write_time((*it).folder);
	//	std::time_t curr_time = decltype(time_type)::clock::to_time_t(time_type);

	//	std::time_t last_time = decltype((*it).last_time)::clock::to_time_t((*it).last_time);

	//	if (curr_time > last_time)
	//	{
	//		int i = 0;
	//		++i;
	//	}

	//}

	return ret;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	UnwatchAllFolders();

	return ret;
}

std::string FileSystem::GetAssetsPath()
{
	return assets_path;
}

std::string FileSystem::GetLibraryPath()
{
	return library_path;
}

std::string FileSystem::GetLibraryMeshPath()
{
	return library_mesh_path;
}

std::string FileSystem::GetLibraryPrefabPath()
{
	return library_prefab_path;
}

std::string FileSystem::GetLibraryTexturePath()
{
	return library_texture_path;
}

std::string FileSystem::GetLibraryScenePath()
{
	return library_scene_path;
}

std::string FileSystem::GetLibraryShadersPath()
{
	return library_shaders_path;
}

std::string FileSystem::GetSettingsPath()
{
	return settings_path;
}

std::string FileSystem::GetLookingPath()
{
	return looking_path;
}

void FileSystem::SetLookingPath(const std::string& new_path)
{
	looking_path = new_path;
}

DecomposedFilePath FileSystem::DecomposeFilePath(std::string file_path)
{
	DecomposedFilePath ret;

	bool adding_file_extension = false;
	bool adding_file_name = false;
	int last_bar_pos = 0;

	for (int i = 0; i < file_path.length(); ++i)
	{
		char curr_word = file_path[i];

		// Formating --------------------

		if (curr_word == '/')
			curr_word = '\\';

		ret.file_path += curr_word;

		// ------------------------------

		// File extension ---------------
		if (adding_file_extension)
			ret.file_extension += curr_word;

		if (curr_word == '.')
		{
			if (adding_file_extension && ret.file_extension.size() > 1)
			{
				std::string to_append = "." + ret.file_extension.substr(0, ret.file_extension.size() - 1);
				ret.file_name.append(to_append);
			}

			adding_file_extension = true;
			ret.file_extension.clear();
		}
		// -------------------------------

		// File name ---------------------

		if (curr_word == '.')
		{
			adding_file_name = false;
		}

		if (adding_file_name)
			ret.file_name += curr_word;

		if (curr_word == '\\')
		{
			last_bar_pos = i;
			adding_file_name = true;
			ret.file_name.clear();
		}

		// -------------------------------
	}

	ret.file_extension_lower_case = ToLowerCase(ret.file_extension);

	// Path ---------------------

	for (int i = 0; i <= last_bar_pos; i++)
	{
		ret.path += ret.file_path[i];
	}

	// --------------------------

	return ret;
}

std::string FileSystem::SelectFolderDialog(bool& canceled)
{
	std::string ret = "";

	const char* folder = tinyfd_selectFolderDialog("asd", NULL);

	if (folder != nullptr)
	{
		ret = folder;
		ret += "\\";

		canceled = false;
	}
	else
		canceled = true;

	return ret.c_str();
}

std::string FileSystem::SelectFileDilog(bool & canceled, const char* filter[])
{
	std::string ret;

	//const char* lFilterPatterns[2] = { "*.scene; *.jscene" };
	const char* path = tinyfd_openFileDialog("Load Scene...", NULL, 1, filter, NULL, 0);

	if (path == nullptr)
		canceled = true;
	else
	{
		canceled = false;
		ret = path;
	}

	return ret;
}

std::string FileSystem::NewNameForFileNameCollision(const char * filename)
{
	std::string ret;

	int number = GetFileNameNumber(filename);

	if (number != -1)
	{
		ret = SetFileNameNumber(filename, number + 1);
	}
	else
	{
		ret = SetFileNameNumber(filename, 1);
	}

	return ret;
}

int FileSystem::GetFileNameNumber(const char * filename)
{
	int ret = -1;

	std::string name = filename;

	std::string number_str;
	bool adding = false;

	for (int i = 0; i < name.size(); ++i)
	{
		if (name[i] == '(')
		{
			adding = true;
			number_str.clear();
			continue;
		}

		if (name[i] == ')')
		{
			adding = false;
		}

		if (adding)
		{
			if (!isdigit(name[i]))
			{
				number_str.clear();
				adding = false;
				continue;
			}

			number_str += name[i];
		}
	}

	if (number_str.size() > 0)
	{
		ret = atoi(number_str.c_str());
	}

	return ret;
}

std::string FileSystem::SetFileNameNumber(const char * filename, int number)
{
	std::string ret = filename;

	if (GetFileNameNumber(filename) != -1)
	{
		int start = 0;
		bool has_start = false;
		bool has_end = false;
		int end = 0;

		for (int i = ret.size() - 1; i >= 0; --i)
		{
			if (ret[i] == ')')
			{
				start = i;
				has_start = true;
			}

			if (ret[i] == '(' && has_start)
			{
				end = i;
				has_end = true;
				break;
			}
		}

		if (has_start && has_end)
			ret = ret.substr(0, end);
	}

	ret += ('(' + std::to_string(number) + ')');

	return ret;
}

std::string FileSystem::GetFileExtension(const char * file_name)
{
	std::string ret;

	bool adding = false;
	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
		{
			ret.clear();
			adding = true;
			continue;
		}

		if (adding)
			ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFilenameWithoutExtension(const char * file_name)
{
	std::string ret;

	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
		{
			break;
		}

		ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFileNameFromFilePath(const char * file_path)
{
	std::string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string FileSystem::GetPathFromFilePath(const char * file_path)
{
	std::string ret;

	int last = 0;
	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\')
		{
			last = i;
			last++;
		}
	}

	for (int i = 0; i < last && file_path[i] != '\0'; i++)
	{
		ret += file_path[i];
	}

	return ret;
}

std::string FileSystem::GetFolderNameFromPath(const char * path)
{
	std::string s_path = path;

	std::string ret;

	for (int i = 0; i < s_path.size(); ++i)
	{
		ret += s_path[i];

		if (s_path[i] == '\\' && s_path.size()-1 != i)
		{
			ret.clear();
		}
	}

	if (ret.size() > 0)
	{
		if (ret[ret.size() - 1] == '\\')
		{
			ret = ret.substr(0, ret.size() - 1);
		}
	}

	return ret;
}

std::string FileSystem::GetParentFolder(const char * folder_path)
{
	std::string s_path = folder_path;

	std::string ret;

	std::string curr_folder;
	for (int i = 0; i < s_path.size(); ++i)
	{
		curr_folder += s_path[i];

		if (s_path[i] == '\\')
		{
			if (i + 1 < s_path.size())
			{
				ret += curr_folder;
				curr_folder.clear();
			}
		}
	}

	return ret;
}

std::string FileSystem::CreateFolder(const char * path, const char * name)
{
	std::string ret;

	std::string filepath = path;
	
	if (filepath[filepath.length()-1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		INTERNAL_LOG("Error creating folder (path not found): %s", path);
		return ret;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		INTERNAL_LOG("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '\\';

	return ret;
}

void FileSystem::FileMove(const char * filepath, const char * new_path, bool replace_existing)
{
	std::string s_new_path = new_path;

	if (s_new_path[s_new_path.length() - 1] != '\\')
	{
		s_new_path += '\\';
	}

	if (FolderExists(s_new_path.c_str()) && FileExists(filepath))
	{
		DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

		if (!replace_existing)
		{
			d_filepath.file_name = FileRenameOnNameCollision(new_path, d_filepath.file_name.c_str(), d_filepath.file_extension.c_str());

			std::string new_filepath = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;

			//App->file_system->FileRename(filepath, d_filepath.file_name.c_str());

			std::string curr_path = d_filepath.path + d_filepath.file_name + "." + d_filepath.file_extension;
			std::string curr_new_path = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;
			if (MoveFileEx(curr_path.c_str(), curr_new_path.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING) == 0)
			{
				DWORD error = GetLastError();
				if (error != 0)
					INTERNAL_LOG("Error moving file:[%s] to [%s]", filepath, s_new_path.c_str())
			}
		}
		else
		{
			std::string curr_new_path = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;
			if (MoveFileEx(filepath, s_new_path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) == 0)
			{
				DWORD error = GetLastError();
				if (error != 0)
					INTERNAL_LOG("Error moving file:[%s] to [%s]", filepath, s_new_path.c_str())
			}
		}

	}
}

bool FileSystem::FileCopyPaste(const char * filepath, const char * new_path, bool overwrite, std::string &resultant_path)
{
	bool ret = false;

	std::string s_new_path = new_path;

	if (s_new_path[s_new_path.length() - 1] != '\\')
	{
		s_new_path += '\\';
	}

	if (FolderExists(s_new_path.c_str()) && FileExists(filepath))
	{
		DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

		if (d_filepath.path != new_path)
		{
			std::string original_name = d_filepath.file_name;

			std::string new_filepath = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;

			if (!overwrite)
			{
				d_filepath.file_name = FileRenameOnNameCollision(new_path, d_filepath.file_name.c_str(), d_filepath.file_extension.c_str());

				std::string new_filepath = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;

				bool need_rename = false;
				if (d_filepath.file_name != original_name)
					need_rename = true;

				//if(need_rename)
				//	App->file_system->FileRename(filepath, d_filepath.file_name.c_str());

				std::string curr_path = d_filepath.path + d_filepath.file_name + "." + d_filepath.file_extension;
				std::string curr_new_path = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;
				if (CopyFile(curr_path.c_str(), curr_new_path.c_str(), false) == 0)
				{
					DWORD error = GetLastError();
					if (error != 0)
						INTERNAL_LOG("Error moving file:[%s] to [%s]", filepath, s_new_path.c_str())
				}
				else
				{
					resultant_path = curr_new_path;
					ret = true;
				}

				if (need_rename)
					FileRename(curr_path.c_str(), original_name.c_str());
			}
			else
			{
				//if (App->file_system->FileExists(new_filepath.c_str()))
				//{
				//	App->file_system->FileDelete(new_filepath.c_str());
				//}

				if (CopyFile(filepath, new_filepath.c_str(), false) == 0)
				{
					DWORD error = GetLastError();
					if (error != 0)
						INTERNAL_LOG("Error moving file:[%s] to [%s]", filepath, s_new_path.c_str())
				}
				else
				{
					resultant_path = new_filepath;
					ret = true;
				}

			}
		}
	}

	return ret;
}

bool FileSystem::FileCopyPaste(const char * filepath, const char * new_path, bool overwrite = false)
{
	std::string result;
	return FileCopyPaste(filepath, new_path, overwrite, result);
}

void FileSystem::FileCopyPasteWithNewName(const char * filepath, const char * new_path, const char * new_name)
{
	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string changed_original_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (FileRename(filepath, new_name))
	{
		FileCopyPaste(changed_original_filepath.c_str(), new_path, false);

		FileRename(changed_original_filepath.c_str(), d_filepath.file_name.c_str());
	}
}

bool FileSystem::FileDelete(const char * filepath)
{
	bool ret = false;

	if (DeleteFile(filepath) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			INTERNAL_LOG("Error deleting file (path not found)): %s", filepath);
		}
	}
	else
		ret = true;

	return ret;
}

bool FileSystem::FolderDelete(const char * folderpath)
{
	bool ret = false;

	if (RemoveDirectory(folderpath) != 0)
		ret = true;
	else
	{		
		INTERNAL_LOG("Error deleting path): %s", folderpath);
	}

	return ret;
}

bool FileSystem::FileSave(const char * path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	std::string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else 
	{
		INTERNAL_LOG("Error saving file %s: ", name);
	}

	if(new_file)
		fclose(new_file);

	return ret;
}

std::vector<std::string> FileSystem::GetFilesAndFoldersInPath(const char * path, const char* extension)
{
	std::string s_path = path;

	if (s_path[s_path.length() - 1] != '\\')
	{
		s_path += '\\';
	}

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;

	if (!TextCmp(extension, ""))
	{
		path_ex += "*.";
		path_ex += extension;
	}
	else
	{
		path_ex += "*.*";
	}

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	int counter = 0;
	while (handle != INVALID_HANDLE_VALUE)
	{
		bool can_add = true;
		if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (TextCmp("..", search_data.cFileName) || TextCmp(".", search_data.cFileName))
			{
				can_add = false;
			}
		}

		if (can_add)
		{
			std::string path_new = s_path;
			path_new += search_data.cFileName;

			if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				path_new += "\\";

			files.push_back(path_new);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if(handle)
		FindClose(handle);

	return files;
}

std::vector<std::string> FileSystem::GetFoldersInPath(const char * path)
{
	std::string s_path = path;

	if (s_path[s_path.length() - 1] != '\\')
	{
		s_path += '\\';
	}

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;
	path_ex += "*.*";

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (!TextCmp("..", search_data.cFileName) && !TextCmp(".", search_data.cFileName))
			{
				std::string path_new = s_path;
				path_new += search_data.cFileName + std::string("\\");
				files.push_back(path_new);
			}
		}
		
		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if (handle)
		FindClose(handle);

	return files;
}

std::vector<std::string> FileSystem::GetFilesInPath(const char * path, const char * extension)
{
	std::string s_path = path;

	if (s_path[s_path.length() - 1] != '\\')
	{
		s_path += '\\';
	}

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;

	if (!TextCmp(extension, ""))
	{
		path_ex += "*.";
		path_ex += extension;
	}
	else
	{
		path_ex += "*.*";
	}

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		if (!(search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::string path_new = s_path;
			path_new += search_data.cFileName;
			files.push_back(path_new);
		}

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if (handle)
		FindClose(handle);

	return files;
}

std::vector<std::string> FileSystem::GetFilesInPathAndChilds(const char * path)
{
	std::vector<std::string> ret;

	if (FolderExists(path))
	{
		std::vector<std::string> folders_to_look;
		folders_to_look.push_back(path);

		while (folders_to_look.size() > 0)
		{
			//std::vector<std::string>::iterator fol_it = folders_to_look.begin();

			//std::vector<std::string> files = App->file_system->GetFilesInPath((*fol_it).c_str());

			//if(files.size() > 0)
			//	ret.insert(ret.begin(), files.begin(), files.end());

			//std::vector<std::string> new_directories = App->file_system->GetFoldersInPath((*fol_it).c_str());

			//if(new_directories.size() > 0)
			//	folders_to_look.insert(folders_to_look.end(), new_directories.begin(), new_directories.end());

			//folders_to_look.erase(folders_to_look.begin());
		}
	}

	return ret;
}

Folder FileSystem::GetFilesAndFoldersTree(const char * path)
{
	return GetFoldersRecursive(path);
}

Folder FileSystem::GetFoldersRecursive(const char * path)
{
	Folder ret;

	if (FolderExists(path))
	{
	/*	std::vector<std::string> new_directories = App->file_system->GetFoldersInPath(path);

		std::vector<std::string> files = App->file_system->GetFilesInPath(path);

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			DecomposedFilePath dfp = DecomposeFilePath((*it));
			ret.files.push_back(dfp);
		}

		ret.folder_path = path;
		ret.folder_name = GetFolderNameFromPath(path);

		ret.valid = true;

		for (std::vector<std::string>::iterator it = new_directories.begin(); it != new_directories.end(); ++it)
		{
			Folder to_add = GetFoldersRecursive((*it).c_str());

			ret.folders.push_back(to_add);
		}*/
	}

	return ret;
}

void FileSystem::UnwatchAllFolders()
{

}

bool FileSystem::FileExists(const char * path, const char * name, const char * extension)
{
	WIN32_FIND_DATA search_data;

	bool has_extension = true;
	if (TextCmp(extension, ""))
		has_extension = false;

	std::string filepath = path;
	if (has_extension)
	{
		filepath += "*.";
		filepath += extension;
	}
	else
	{
		filepath += "*.*";
	}

	std::string filename = name;
	if (has_extension)
	{
		filename += ".";
		filename += extension;
	}

	HANDLE handle = FindFirstFile(filepath.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		std::string found_file = search_data.cFileName;

		if(!has_extension)
			found_file = GetFilenameWithoutExtension(search_data.cFileName);
		
		if (TextCmp(found_file.c_str(), filename.c_str()))
			return true;
		
		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	return false;
}

bool FileSystem::FileExists(const char * filepath)
{
	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);


	return FileExists(d_filepath.path.c_str(), d_filepath.file_name.c_str(), d_filepath.file_extension.c_str());
}

bool FileSystem::FileRename(const char * filepath, const char * new_name)
{
	bool ret = false;

	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string new_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (rename(filepath, new_filepath.c_str()) == 0)
		ret = true;
	
	return ret;
}

bool FileSystem::FolderRename(const char * folderpath, const char * new_name)
{
	std::string parent_folder = GetParentFolder(folderpath);

	std::string new_path = parent_folder + new_name + '\\';
	if (MoveFileEx(folderpath, new_path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) == 0)
	{
		DWORD error = GetLastError();
		if (error != 0)
			INTERNAL_LOG("Error renaming folder:[%s] to [%s]", folderpath, new_path.c_str())
	}

	return false;
}

bool FileSystem::FolderExists(const char * path)
{
	bool ret = true;

	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		ret = false;

	return ret;
}

std::string FileSystem::FileRenameOnNameCollision(const char * path, const char* name, const char* extension)
{
	std::string ret;

	std::string s_path = path;
	std::string s_name = name;
	std::string s_extension = extension;

	ret = s_name;

	std::string new_filepath = s_path + s_name + "." + std::string(extension);

	//bool need_rename = false;
	//while (App->file_system->FileExists(new_filepath.c_str()))
	//{
	//	std::string check_new_name = NewNameForFileNameCollision(s_name.c_str());
	//	s_name = check_new_name;

	//	new_filepath = s_path + s_name + "." + s_extension;

	//	need_rename = true;
	//}

	//if (need_rename)
	//	ret = s_name;

	return ret;
}

bool FileSystem::FolderWatch(const char * path, const std::function<void(const std::experimental::filesystem::path&)> &callback, bool watch_subfolders)
{
	bool ret = false;

	if (FolderExists(path))
	{
	/*	WatchingFloder wfolder;
		wfolder.folder = path;
		wfolder.last_time = std::experimental::filesystem::last_write_time(path);
		watching_folders.push_back(wfolder);*/
	}

	return ret;
}

void Changed(const std::experimental::filesystem::path & path)
{
	INTERNAL_LOG(" -----------------------enter-----------------------");
	int i = 0;
	++i;
}
