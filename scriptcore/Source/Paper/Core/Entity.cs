using System;

namespace Paper
{
    public class Entity
    {
        public readonly UUID UUID;

        protected Entity()
        {
            UUID = 0;
        }

        internal Entity(ulong _UUID)
        {
            UUID = _UUID;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(UUID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T { Entity = this };
            return component;
        }

        public virtual void OnCreate() { }
        public virtual void OnDestroy() { }
        public virtual void OnUpdate(float dt) { }

        
    }
}
