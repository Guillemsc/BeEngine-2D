using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine.Internal
{
    class FileWatcher
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void FileFolderChangedCallback(string path);

        public bool FileFolderWatch(string path)
        {
            bool ret = true;

            if (!IsFileFolderWatching(path))
            {
                try
                {
                    FileSystemWatcher watcher = new FileSystemWatcher();
                    watcher.Path = path;
                    watcher.NotifyFilter = NotifyFilters.Attributes |
                                           NotifyFilters.CreationTime |
                                           NotifyFilters.FileName |
                                           NotifyFilters.LastAccess |
                                           NotifyFilters.LastWrite |
                                           NotifyFilters.Size |
                                           NotifyFilters.Security |
                                           NotifyFilters.DirectoryName;

                    watcher.Filter = "*.*";

                    watcher.Changed += OnChanged;
                    watcher.Created += OnChanged;
                    watcher.Deleted += OnChanged;
                    watcher.Renamed += OnChangedName;

                    watcher.EnableRaisingEvents = true;
                    watcher.IncludeSubdirectories = true;

                    watching_file_folders.Add(watcher);
                }
                catch (Exception ex)
                {
                    ret = false;
                }
            }
            else
                ret = true;

            return ret;
        }

        public bool IsFileFolderWatching(string path)
        {
            bool ret = false;

            for (int i = 0; i < watching_file_folders.Count; ++i)
            {
                if (watching_file_folders[i].Path == path)
                {
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        public void StopWatchingFileFolder(string path)
        {
            for (int i = 0; i < watching_file_folders.Count; ++i)
            {
                if (watching_file_folders[i].Path == path)
                {
                    watching_file_folders.RemoveAt(i);
                    break;
                }
            }
        }

        private void OnChanged(object source, FileSystemEventArgs e)
        {
            FileFolderChangedCallback(e.FullPath);
        }

        private void OnChangedName(object sender, RenamedEventArgs e)
        {
            FileFolderChangedCallback(e.FullPath);
        }

        private List<FileSystemWatcher> watching_file_folders = new List<FileSystemWatcher>();
    }
}
