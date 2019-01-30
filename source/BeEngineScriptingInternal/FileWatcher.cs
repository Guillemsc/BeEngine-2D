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
                    watcher.Renamed += OnChanged;

                    watcher.EnableRaisingEvents = true;
                    watcher.IncludeSubdirectories = true;

                    watching.Add(watcher);
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

        public void StopWatch(string path)
        {
            for (int i = 0; i < watching.Count; ++i)
            {
                FileSystemWatcher curr_watcher = watching[i];

                if (curr_watcher.Path == path)
                {
                    curr_watcher.Changed -= OnChanged;
                    curr_watcher.Created -= OnChanged;
                    curr_watcher.Deleted -= OnChanged;
                    curr_watcher.Renamed -= OnChanged;
                    curr_watcher.Dispose();
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
                if (watching[i].Path == path)
                {
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        public void AddException(string path)
        {
            RemoveException(path);

            exceptions.Add(path);

            if (clear_exceptions_timer == null)
            {
                clear_exceptions_timer = new Timer();
                clear_exceptions_timer.Elapsed += OnClearExceptions;
                clear_exceptions_timer.AutoReset = true;
            }

            clear_exceptions_timer.Stop();
            clear_exceptions_timer.Start();
            clear_exceptions_timer.Interval = 1000 * 4;
        }

        public void RemoveException(string path)
        {
            for(int i = 0; i < exceptions.Count; ++i)
            {
                if(exceptions[i] == path)
                {
                    exceptions.RemoveAt(i);
                    break;
                }
            }
        }

        public bool IsException(string path)
        {
            bool ret = false;

            for (int i = 0; i < exceptions.Count; ++i)
            {
                if (exceptions[i] == path)
                {
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        public string[] GetChangesStack()
        {
            if (stack.Count > 0)
            {
                for (int i = 0; i < stack.Count; ++i)
                {
                    if (IsException(stack[i]))
                    {
                        stack.RemoveAt(i);
                    }
                    else
                        ++i;
                }
            }

            string[] ret = stack.ToArray();

            stack.Clear();

            return ret;
        }

        private void OnClearExceptions(object sender, ElapsedEventArgs e)
        {
            exceptions.Clear();
        }

        private void OnChanged(object source, FileSystemEventArgs e)
        {
            stack.Add(e.FullPath);
        }

        private List<FileSystemWatcher> watching = new List<FileSystemWatcher>();
        private List<string> stack = new List<string>();
        private List<string> exceptions = new List<string>();

        private Timer clear_exceptions_timer = null;
    }
}
