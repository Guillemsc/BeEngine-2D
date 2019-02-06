using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BeEngine
{
    public class BeEngineScript
    {
        private bool Init(GameObject go)
        {
            bool ret = false;

            if (go != null)
            {
                game_object = go;

                ret = true;
            }

            return ret;
        }

        public virtual void Awake() { }
        public virtual void Start() { }
        public virtual void Update() { }
        public virtual void OnDestroy() { }

        public GameObject game_object = null;
    }
}
