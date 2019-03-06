using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
    public delegate void CollisionDelegate(Collision coll);

    public class GameObject : BeEngineObject
    {
        /// <summary>
        ///   <para>Sets or gets the GameObject name</para>
        /// </summary>
        public string Name
        {
            get
            {
                return GetName();
            }
            set
            {
                SetName(value);
            }
        }

        public T AddComponent<T>() where T : Component
        {
            T ret = null;

            Type type = typeof(T);

            Component created_comp = AddComponentType(type);

            if(created_comp != null)
                ret = (T)Convert.ChangeType(created_comp, type);

            return ret;
        }

        public ComponentTransform Transform
        {
            get; private set;
        }

        public void SuscribeToOnCollisionEnter(CollisionDelegate callback)
        {
            on_collision_enter += callback;
        }

        public void UnSuscribeFromOnCollisionEnter(CollisionDelegate callback)
        {
            on_collision_enter -= callback;
        }

        public void SuscribeToOnCollisionStay(CollisionDelegate callback)
        {
            on_collision_stay += callback;
        }

        public void UnSuscribeFromOnCollisionStay(CollisionDelegate callback)
        {
            on_collision_stay -= callback;
        }

        public void SuscribeToOnCollisionExit(CollisionDelegate callback)
        {
            on_collision_exit += callback;
        }

        public void UnSuscribeFromOnCollisionExit(CollisionDelegate callback)
        {
            on_collision_exit -= callback;
        }

        private CollisionDelegate on_collision_enter;
        private CollisionDelegate on_collision_stay;
        private CollisionDelegate on_collision_exit;

        // Internal functions ----------------------------------

        private void SetComponentTransform(ComponentTransform transform)
        {
            this.Transform = transform;
        }

        private void CallOnCollisionEnter(Collision coll)
        {
            if (on_collision_enter != null)
                on_collision_enter(coll);
        }

        private void CallOnCollisionStay(Collision coll)
        {
            if (on_collision_stay != null)
                on_collision_stay(coll);
        }

        private void CallOnCollisionExit(Collision coll)
        {
            if (on_collision_exit != null)
                on_collision_exit(coll);
        }

        // Internal Calls --------------------------------------

        // -----------------------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetName(string value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string GetName();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Component AddComponentType(Type type);

        // -----------------------------------------------------
    }
}
