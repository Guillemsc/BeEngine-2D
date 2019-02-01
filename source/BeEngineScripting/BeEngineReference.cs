using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineReference
    {
        public IntPtr InitReference(IntPtr game_object_ptr)
        {
            this.game_object_ptr = game_object_ptr;

            return game_object_ptr;
        }

        public IntPtr GetGameObjectPtr()
        {
            return game_object_ptr;
        }

        public virtual void Awake() { }
        public virtual void Start() { }
        public virtual void Update() { }
        public virtual void OnDestroy() { }

        private IntPtr game_object_ptr;
    }
}
