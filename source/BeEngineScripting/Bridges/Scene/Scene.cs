using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.CompilerServices;

namespace BeEngine
{
    public class Scene
    {
        public static GameObject CreateGameObject()
        {
            return SceneCreateGameObject();
        }

        public static void DestroyGameObject(GameObject go)
        {
            if (go != null)
            {
                SceneDestroyGameObject(go);

                go = null;
            }
        }

        public static void DestroyComponent(Component component)
        {
            if (component != null)
            {
                SceneDestroyComponent(component);

                component = null;
            }
        }

        public static GameObject LoadPrefab(ResourcePrefab prefab)
        {
            return SceneLoadPrefab(prefab);
        }

        public static void LoadScene(ResourceScene scene)
        {
            SceneLoadScene(scene);
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern GameObject SceneCreateGameObject();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneDestroyGameObject(GameObject go);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneDestroyComponent(Component component);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern GameObject SceneLoadPrefab(ResourcePrefab prefab);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneLoadScene(ResourceScene scene);

        // -----------------------------------------------------
    }
}
