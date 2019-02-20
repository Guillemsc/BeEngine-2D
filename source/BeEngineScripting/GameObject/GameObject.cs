using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;

namespace BeEngine
{
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

        public ComponentTransform transform
        {
            get; private set;
        }

        // -----------------------------------------------------

        private void SetComponentTransform(ComponentTransform transform)
        {
            this.transform = transform;
        }

        // Internal Calls --------------------------------------

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetName(string value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern string GetName();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Component AddComponentType(Type type);

        // -----------------------------------------------------
    }
}
