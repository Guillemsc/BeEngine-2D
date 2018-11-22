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

void FileSystem::OnEvent(Event * ev)
{

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

DecomposedFilePath FileSystem::DecomposeFilePath(std::string file_path)
{
	DecomposedFilePath ret;

	bool adding_file_extension = false;
	bool adding_file_name = false;
	int last_bar_pos = 0;

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
			last_bar_pos = i;
			adding_file_name = true;
			ret.file_name.clear();
		}

		// -------------------------------
	}

	ret.file_extension_lower_case = ToLowerCase(ret.file_extension);
	ret.folder_name = GetFolderNameFromPath(ret.file_path.c_str());

	// Path ---------------------

	for (int i = 0; i <= last_bar_pos; i++)
	{
		ret.path += ret.file_path[i];
	}

	// --------------------------

	return ret;
}

bool FileSystem::FilePathInsideFolder(std::string file_path, std::string folder_path)
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

std::string FileSystem::SubstractFolder(std::string folder, std::string folder_to_substract)
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

bool FileSystem::CreateFolder(const char * path, const char * name, bool check_name_collision, std::string& new_filepath)
{
	bool ret = true;

	std::string filepath = path;

	if (filepath.size() > 0)
	{
		if (filepath[filepath.length() - 1] != '\\')
		{
			filepath += '\\';
		}

		filepath += name;

		new_filepath = filepath + "\\";

		if (check_name_collision)
		{
			filepath += "\\";

			if (FolderExists(filepath.c_str()))
			{
				filepath = FolderRenameOnCollision(filepath.c_str());

				new_filepath = filepath;

				filepath = filepath.substr(0, filepath.size() - 1);
			}
		}

		DWORD error = GetLastError();

		if (CreateDirectory(filepath.c_str(), NULL) == 0)
		{
			error = GetLastError();
		}

		if (error == ERROR_PATH_NOT_FOUND)
		{
			INTERNAL_LOG("Error creating folder (path not found): %s", path);
			ret = false;
		}
	}
	else
		ret = false;

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

	if (s_new_path.size() > 0)
	{
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

					if(need_rename)
						App->file_system->FileRename(filepath, d_filepath.file_name.c_str());

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
					if (App->file_system->FileExists(new_filepath.c_str()))
					{
						App->file_system->FileDelete(new_filepath.c_str());
					}

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
	std::vector<std::string> files;

	std::string s_path = path;

	if (s_path.size() > 0)
	{
		if (s_path[s_path.length() - 1] != '\\')
		{
			s_path += '\\';
		}

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

		if (handle)
			FindClose(handle);
	}

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
			std::vector<std::string>::iterator fol_it = folders_to_look.begin();

			std::vector<std::string> files = App->file_system->GetFilesInPath((*fol_it).c_str());

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

Folder FileSystem::GetFilesAndFoldersTree(const char * path)
{
	return GetFoldersRecursive(path);
}

Folder FileSystem::GetFoldersRecursive(const char * path)
{
	Folder ret;

	if (FolderExists(path))
	{
		std::vector<std::string> new_directories = App->file_system->GetFoldersInPath(path);

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
		}
	}

	return ret;
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

bool FileSystem::FileRename(const char * filepath, const char * new_name, bool check_name_collision, std::string& new_f)
{
	bool ret = false;

	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string new_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (new_filepath.compare(filepath) != 0)
	{
		if (check_name_collision)
		{
			DecomposedFilePath d_new_filepath = DecomposeFilePath(new_filepath);

			std::string renamed_name = FileRenameOnNameCollision(d_new_filepath.path.c_str(), d_new_filepath.file_name.c_str(), d_new_filepath.file_extension.c_str());

			new_filepath = d_new_filepath.path + renamed_name + "." + d_new_filepath.file_extension;
		}

		if (rename(filepath, new_filepath.c_str()) == 0)
		{
			new_f = new_filepath;
			ret = true;
		}
	}
	
	return ret;
}

bool FileSystem::FolderRename(const char * folderpath, const char * new_name, bool check_name_collision, std::string & new_folderpath)
{
	bool ret = false;

	std::string parent_folder = GetParentFolder(folderpath);

	std::string new_path = parent_folder + new_name + '\\';

	if (new_path.compare(folderpath) != 0)
	{
		if (check_name_collision)
		{
			if (FolderExists(new_path.c_str()))
			{
				new_path = FolderRenameOnCollision(new_path.c_str());
			}
		}

		new_folderpath = new_path;

		if (MoveFileEx(folderpath, new_path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) == 0)
		{
			DWORD error = GetLastError();
			if (error != 0)
			{
				INTERNAL_LOG("Error renaming folder:[%s] to [%s]", folderpath, new_path.c_str())
			}
			else
				ret = true;
		}
	}

	return ret;
}

bool FileSystem::FolderExists(const char * path)
{
	bool ret = true;

	DWORD ftyp = GetFileAttributesA(path);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		ret = false;

	return ret;
}

std::string FileSystem::FolderParent(const char * path)
{
	std::string ret;

	std::string s_path = path;

	std::string adding_path;
	for (int i = 0; i < s_path.size(); ++i)
	{
		char curr_char = s_path[i];

		adding_path += curr_char;

		if (curr_char == '\\' && i < s_path.size() - 1)
		{
			ret = adding_path;
		}
	}

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

	bool need_rename = false;
	while (App->file_system->FileExists(new_filepath.c_str()))
	{
		std::string check_new_name = NewNameForNameCollision(s_name.c_str());
		s_name = check_new_name;

		new_filepath = s_path + s_name + "." + s_extension;

		need_rename = true;
	}

	if (need_rename)
		ret = s_name;

	return ret;
}

std::string FileSystem::FolderRenameOnCollision(const char * path)
{
	std::string ret;

	std::string s_path = path;
	std::string parent_path = FolderParent(path);

	ret = s_path;

	bool need_rename = false;

	while (App->file_system->FolderExists(s_path.c_str()))
	{
		std::string name_to_change = GetFolderNameFromPath(s_path.c_str());

		std::string check_new_name = NewNameForNameCollision(name_to_change.c_str());

		s_path = parent_path + check_new_name + "\\";

		need_rename = true;
	}

	if (need_rename)
		ret = s_path;

	return ret;
}
