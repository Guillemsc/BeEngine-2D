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

        public static void DestroyGameObject(ref GameObject go)
        {
            if (go != null)
            {
                SceneDestroyGameObject(go);

                go = null;
            }
        }

        public static void DestroyComponent(ref Component component)
        {
            if (component != null)
            {
                SceneDestroyComponent(component);

                component = null;
            }
        }

        public static void LoadScene(string name)
        {
            SceneLoadScene(name);
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern GameObject SceneCreateGameObject();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneDestroyGameObject(GameObject go);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneDestroyComponent(Component component);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SceneLoadScene(string name);

        // -----------------------------------------------------
    }
}
