#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <ctype.h>
#include "Functions.h"
#include "tinyfiledialogs.h"
#include "Event.h"
#include "ModuleEvent.h"

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

	return ret;
}

bool FileSystem::Update()
{
	bool ret = true;

	return ret;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

std::string FileSystem::GetWorkingDirectory()
{
	std::string ret;

	char path[MAX_PATH] = "";
	GetCurrentDirectoryA(MAX_PATH, path);

	ret = path;

	ret += "\\";

	return ret;
}

bool FileSystem::FileExists(const std::string& filepath)
{
	bool ret = false;

	WIN32_FIND_DATA search_data;

	DecomposedFilePath dfp = DecomposeFilePath(filepath);

	std::string filepath_to_search = dfp.path + "*." + dfp.file_extension;

	std::string filename_to_search = dfp.file_name + "." + dfp.file_extension;

	HANDLE handle = FindFirstFile(filepath_to_search.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		std::string found_file = search_data.cFileName;

		if (filename_to_search.compare(found_file) == 0)
		{
			FindClose(handle);

			ret = true;
			break;
		}

		if (FindNextFile(handle, &search_data) == FALSE)
		{
			if (handle)
				FindClose(handle);

			break;
		}
	}

	return ret;
}

bool FileSystem::FileDelete(const std::string& filepath)
{
	bool ret = false;

	if (DeleteFile(filepath.c_str()) == 0)
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

bool FileSystem::FileRename(const std::string& filepath, const std::string& new_name, bool check_name_collision, std::string& new_filepath)
{
	bool ret = false;

	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	new_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (new_filepath.compare(filepath) != 0)
	{
		if (check_name_collision)
		{
			DecomposedFilePath d_new_filepath = DecomposeFilePath(new_filepath);

			new_filepath = FileRenameOnNameCollision(new_filepath);
		}

		if (rename(filepath.c_str(), new_filepath.c_str()) == 0)
		{
			ret = true;
		}
	}

	return ret;
}

bool FileSystem::FileCopyPaste(const std::string& filepath, const std::string& new_path, bool check_name_collision, std::string & resultant_path)
{
	bool ret = false;

	if (FolderExists(new_path.c_str()) && FileExists(filepath.c_str()))
	{
		std::string s_new_path = new_path;
				
		DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

		if (d_filepath.path.compare(new_path) != 0)
		{
			std::string original_name = d_filepath.file_name;

			std::string new_filepath = s_new_path + d_filepath.file_name + "." + d_filepath.file_extension;

			std::string curr_filepath = filepath;

			bool need_rename = false;

			if (check_name_collision)
			{
				new_filepath = FileRenameOnNameCollision(new_filepath);

				DecomposedFilePath new_dfp = DecomposeFilePath(new_filepath);

				if (new_dfp.file_name != original_name)
					need_rename = true;

				if (need_rename)
					App->file_system->FileRename(filepath.c_str(), new_dfp.file_name.c_str(), false, curr_filepath);
			}
			
			if (CopyFile(curr_filepath.c_str(), new_filepath.c_str(), false) == 0)
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

			if (need_rename)
				FileRename(curr_filepath.c_str(), original_name.c_str());			
		}
	}

	return ret;
}

bool FileSystem::FileSave(const std::string& path, const std::string& name, const std::string& extension, const char * file_content, uint size)
{
	bool ret = false;

	std::string filepath = path + name + "." + extension;

	std::ofstream;
	FILE* new_file = fopen(filepath.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		INTERNAL_LOG("Error saving file %s: ", name);
	}

	if (new_file)
		fclose(new_file);

	return ret;
}

bool FileSystem::FilePathInsideFolder(const std::string& file_path, const std::string& folder_path)
{
	bool ret = true;

	if (file_path.size() > folder_path.size())
	{
		for (int i = 0; i < folder_path.size(); ++i)
		{
			if (folder_path[i] != file_path[i])
			{
				ret = false;
			}
		}
	}
	else
		ret = false;

	return ret;
}

std::string FileSystem::SubstractFolder(const std::string& folder, const std::string& folder_to_substract)
{
	std::string ret;

	bool correct = true;
	for (int i = 0; i < folder.size(); ++i)
	{
		char curr_char = folder[i];

		if (i < folder_to_substract.size())
		{
			if (curr_char != folder_to_substract[i])
			{
				correct = false;
				break;
			}
		}
		else
		{
			ret += curr_char;
		}
	}

	if (!correct)
		ret = folder;

	return ret;
}

std::string FileSystem::SelectFolderDialog(bool& canceled)
{
	std::string ret = "";

	const char* folder = tinyfd_selectFolderDialog("Selecting Folder...", NULL);

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
	const char* path = tinyfd_openFileDialog("Load File...", NULL, 1, filter, NULL, 0);

	if (path == nullptr)
		canceled = true;
	else
	{
		canceled = false;
		ret = path;
	}

	return ret;
}

std::string FileSystem::NewNameForNameCollision(const char * filename)
{
	std::string ret;

	int number = GetNameCollisionNumber(filename);

	if (number != -1)
	{
		ret = SetNameCollisionNumber(filename, number + 1);
	}
	else
	{
		ret = SetNameCollisionNumber(filename, 1);
	}

	return ret;
}

int FileSystem::GetNameCollisionNumber(const char * filename)
{
	int ret = -1;

	std::string name = filename;

	std::string number_str;

	for (int i = name.size() - 1; i >= 0; --i)
	{
		char curr_char = name[i];

		if (isdigit(curr_char))
		{
			number_str.insert(0, 1, curr_char);
		}
		else
			break;
	}

	if (number_str.size() > 0)
	{
		ret = atoi(number_str.c_str());
	}

	return ret;
}

std::string FileSystem::SetNameCollisionNumber(const char * filename, int number)
{
	std::string ret = filename;

	int curr_number = GetNameCollisionNumber(filename);
	int curr_number_size = std::to_string(curr_number).size();

	if(curr_number != -1)
		ret = ret.substr(0, ret.size() - curr_number_size);
	
	std::string new_number_str = std::to_string(number);

	ret += new_number_str;

	return ret;
}

std::string FileSystem::FileRenameOnNameCollision(const std::string & filepath)
{
	std::string ret;

	DecomposedFilePath dfp = DecomposeFilePath(filepath);

	ret = filepath;

	bool need_rename = false;
	while (App->file_system->FileExists(ret.c_str()))
	{
		dfp.file_name = NewNameForNameCollision(dfp.file_name.c_str());

		ret = dfp.path + dfp.file_name + "." + dfp.file_extension;

		need_rename = true;
	}

	return ret;
}

std::string FileSystem::FolderRenameOnCollision(const std::string& path)
{
	std::string ret;

	std::string parent_path = FolderParent(path);

	ret = path;

	while (App->file_system->FolderExists(ret))
	{
		DecomposedFilePath dfp = DecomposeFilePath(ret);

		std::string check_new_name = NewNameForNameCollision(dfp.folder_name.c_str());

		ret = parent_path + check_new_name + "\\";
	}

	return ret;
}

bool FileSystem::FolderExists(const std::string& path)
{
	bool ret = true;

	DWORD ftyp = GetFileAttributesA(path.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		ret = false;

	return ret;
}

bool FileSystem::FolderCreate(const std::string& path, const std::string& name, bool check_name_collision, std::string& new_filepath)
{
	bool ret = false;

	if (FolderExists(path.c_str()))
	{
		std::string filepath = path;

		if (filepath.size() > 0)
		{
			filepath += name;

			new_filepath = filepath + "\\";

			if (check_name_collision)
			{
				filepath += "\\";
				
				filepath = FolderRenameOnCollision(filepath.c_str());

				new_filepath = filepath;

				filepath = filepath.substr(0, filepath.size() - 1);
			}

			DWORD error = GetLastError();

			if (CreateDirectory(filepath.c_str(), NULL) == 0)
			{
				error = GetLastError();

				if (error == ERROR_PATH_NOT_FOUND)
				{
					INTERNAL_LOG("Error creating folder (path not found): %s", path);
				}
			}
			else
				ret = true;
		}
	}

	return ret;
}

bool FileSystem::FolderDelete(const std::string& folderpath)
{
	bool ret = false;

	if (RemoveDirectory(folderpath.c_str()) != 0)
	{
		ret = true;
	}
	else
	{
		INTERNAL_LOG("Error deleting path): %s", folderpath);
	}

	return ret;
}

std::string FileSystem::FolderParent(const std::string& folder)
{
	std::string ret;

	std::string adding_path;
	for (int i = 0; i < folder.size(); ++i)
	{
		char curr_char = folder[i];

		adding_path += curr_char;

		if (curr_char == '\\' && i < folder.size() - 1)
		{
			ret = adding_path;
		}
	}

	return ret;
}

bool FileSystem::FolderRename(const std::string& folderpath, const std::string& new_name, bool check_name_collision, std::string& new_folderpath)
{
	bool ret = false;

	if (FolderExists(folderpath))
	{
		std::string parent_folder = FolderParent(folderpath);

		new_folderpath = parent_folder + new_name + '\\';

		if (new_folderpath.compare(folderpath) != 0)
		{
			if (check_name_collision)
			{
				new_folderpath = FolderRenameOnCollision(new_folderpath.c_str());
			}

			if (MoveFileEx(folderpath.c_str(), new_folderpath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) == 0)
			{
				DWORD error = GetLastError();
				if (error != 0)
				{
					INTERNAL_LOG("Error renaming folder:[%s] to [%s]", folderpath.c_str(), new_folderpath.c_str())
				}
				else
					ret = true;
			}
		}
	}

	return ret;
}

DecomposedFilePath FileSystem::DecomposeFilePath(const std::string& file_path)
{
	DecomposedFilePath ret;

	bool adding_file_extension = false;
	bool adding_file_name = false;

	std::string adding_folder_name;
	std::string adding_path;

	ret.its_folder = true;

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
		{
			ret.file_extension += curr_word;
			ret.file_extension_lower_case += ToLowerCase(curr_word);
		}

		if (curr_word == '.')
		{
			if (adding_file_extension && ret.file_extension.size() > 1)
			{
				std::string to_append = "." + ret.file_extension.substr(0, ret.file_extension.size() - 1);
				ret.file_name.append(to_append);
			}

			adding_file_extension = true;
			ret.file_extension.clear();
			ret.file_extension_lower_case.clear();

			ret.its_folder = false;
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
			adding_file_name = true;
			ret.file_name.clear();
		}

		// -------------------------------

		// Folder path and folder name -------------------

		adding_path += curr_word;

		if (curr_word == '\\')
		{
			ret.folder_name = adding_folder_name;
			adding_folder_name.clear();

			ret.path = adding_path;
		}
		else
			adding_folder_name += curr_word;

		// -------------------------------
	}

	return ret;
}

std::vector<std::string> FileSystem::GetFilesAndFoldersInPath(const std::string& path, const std::string& extension)
{
	std::vector<std::string> files;

	if (FolderExists(path))
	{
		std::string s_path = path;

		if (s_path.size() > 0)
		{
			WIN32_FIND_DATA search_data;

			std::string path_ex = s_path;

			if (extension.compare("") != 0)
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

			if (handle)
				FindClose(handle);
		}
	}

	return files;
}

std::vector<std::string> FileSystem::GetFoldersInPath(const std::string& path)
{
	std::vector<std::string> files;

	if (FolderExists(path))
	{
		WIN32_FIND_DATA search_data;

		std::string path_ex = path;
		path_ex += "*.*";

		HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

		while (handle != INVALID_HANDLE_VALUE)
		{
			if ((search_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (!TextCmp("..", search_data.cFileName) && !TextCmp(".", search_data.cFileName))
				{
					std::string path_new = path;
					path_new += search_data.cFileName + std::string("\\");
					files.push_back(path_new);
				}
			}

			if (FindNextFile(handle, &search_data) == FALSE)
				break;
		}

		if (handle)
			FindClose(handle);
	}

	return files;
}

std::vector<std::string> FileSystem::GetFilesInPath(const std::string& path, const std::string& extension)
{
	std::string s_path = path;

	std::vector<std::string> files;

	WIN32_FIND_DATA search_data;

	std::string path_ex = s_path;

	if (extension.compare("") != 0)
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

std::vector<std::string> FileSystem::GetFilesInPathAndChilds(const std::string& path, const std::string& extension)
{
	std::vector<std::string> ret;

	if (FolderExists(path))
	{
		std::vector<std::string> folders_to_look;
		folders_to_look.push_back(path);

		while (folders_to_look.size() > 0)
		{
			std::vector<std::string>::iterator fol_it = folders_to_look.begin();

			std::vector<std::string> files = App->file_system->GetFilesInPath((*fol_it).c_str(), extension);

			if(files.size() > 0)
				ret.insert(ret.begin(), files.begin(), files.end());

			std::vector<std::string> new_directories = App->file_system->GetFoldersInPath((*fol_it).c_str());

			if(new_directories.size() > 0)
				folders_to_look.insert(folders_to_look.end(), new_directories.begin(), new_directories.end());

			folders_to_look.erase(folders_to_look.begin());
		}
	}

	return ret;
}


Folder FileSystem::GetFilesAndFoldersTree(const std::string & path)
{
	Folder ret;

	if (FolderExists(path))
	{
		DecomposedFilePath dc_path = DecomposeFilePath(path);

		std::vector<std::string> new_directories = App->file_system->GetFoldersInPath(path);

		std::vector<std::string> files = App->file_system->GetFilesInPath(path);

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			DecomposedFilePath dfp = DecomposeFilePath((*it));
			ret.files.push_back(dfp);
		}

		ret.folder_path = path;
		ret.folder_name = dc_path.folder_name;

		ret.valid = true;

		for (std::vector<std::string>::iterator it = new_directories.begin(); it != new_directories.end(); ++it)
		{
			Folder to_add = GetFilesAndFoldersTree((*it).c_str());

			ret.folders.push_back(to_add);
		}
	}

	return ret;
}
