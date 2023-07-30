using System;

namespace Paper
{
    public class Entity
    {
        public Entity() { }

        public virtual void OnCreate() { }
        public virtual void OnDestroy() { }
        public virtual void OnUpdate(float dt) { }

        public virtual void OnEvent()
        {
            Console.WriteLine("Entity.OnEvent()");
        }
    }
}
