using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Timers;

namespace BeEngine.Internal
{
    class FileWatcher
    {
        public bool Watch(string path)
        {
            bool ret = true;

            if (!IsWatching(path))
            {
                FileWatcherItem item = new FileWatcherItem(path, OnChanged);

                if (rising_events)
                    item.StartRisingEvents();

                watching.Add(item);
            }

            return ret;
        }

        public void StopWatch(string path)
        {
            for (int i = 0; i < watching.Count; ++i)
            {
                FileWatcherItem curr_watcher = watching[i];

                if (curr_watcher.GetPath() == path)
                {
                    curr_watcher.StopRisingEvents();

                    watching.RemoveAt(i);

                    break;
                }
            }
        }

        public bool IsWatching(string path)
        {
            bool ret = false;

            for (int i = 0; i < watching.Count; ++i)
            {
                if (watching[i].GetPath() == path)
                {
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        public void StartRisingEvents()
        {
            rising_events = true;

            for (int i = 0; i < watching.Count; ++i)
            {
                FileWatcherItem curr_watcher = watching[i];

                curr_watcher.StartRisingEvents();
            }
        }

        public string StopRisingEvents()
        {
            string ret = "hi";

            rising_events = false;

            for (int i = 0; i < watching.Count; ++i)
            {
                FileWatcherItem curr_watcher = watching[i];

                curr_watcher.StopRisingEvents();

                ret = "stoping";
            }

            return ret;
        }

        public string[] GetChangesStack()
        {
            string[] ret = stack.ToArray();

            stack.Clear();

            return ret;
        }

        private bool AleadyExistsOnStack(string path)
        {
            bool ret = false;

            for(int i = 0; i < stack.Count; ++i)
            {
                if(path == stack[i])
                {
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        private void OnChanged(string path)
        {
            lock (stack)
            {
                stack.Add(path);
            }
        }

        public delegate void FileWatcherChangeEvent(string path);

        public class FileWatcherItem
        {
            public FileWatcherItem(string path, FileWatcherChangeEvent callback)
            {
                this.path = path;
                on_change_callback += callback;
            }

            public string GetPath()
            {
                return path;
            }

            public void StartRisingEvents()
            {
                try
                {
                    if (watcher == null)
                    {
                        watcher = new FileSystemWatcher();
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

                        watcher.Changed += OnChangeInternal;
                        watcher.Created += OnChangeInternal;
                        watcher.Deleted += OnChangeInternal;
                        watcher.Renamed += OnChangeInternal;

                        watcher.EnableRaisingEvents = true;
                        watcher.IncludeSubdirectories = true;
                    }
                }
                catch(Exception ex)
                {

                }
            }

            public void StopRisingEvents()
            {
                if (watcher != null)
                {
                    watcher.Changed -= OnChangeInternal;
                    watcher.Created -= OnChangeInternal;
                    watcher.Deleted -= OnChangeInternal;
                    watcher.Renamed -= OnChangeInternal;
                    watcher.Dispose();

                    watcher = null;
                }
            }

            private void OnChangeInternal(object source, FileSystemEventArgs e)
            {
                if (on_change_callback != null)
                    on_change_callback(e.FullPath);
            }

            private FileSystemWatcher watcher = null;
            private string path = "";
            private FileWatcherChangeEvent on_change_callback = null;
        }

        private List<FileWatcherItem> watching = new List<FileWatcherItem>();
        private List<string> stack = new List<string>();
        private bool rising_events = true;
    }
}
