using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineReference
    {
        public char[] InitReference(char[] game_object_ptr_address)
        {
            this.game_object_ptr_address = game_object_ptr_address;

            return game_object_ptr_address;
        }

        public virtual void Awake() { }
        public virtual void Start() { }
        public virtual void Update() { }
        public virtual void OnDestroy() { }

        private char[] game_object_ptr_address;
    }
}
