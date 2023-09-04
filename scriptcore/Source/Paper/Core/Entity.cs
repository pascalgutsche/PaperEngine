using System;

namespace Paper
{
    public class Entity
    {
        public readonly PaperID PaperID;

        public Entity()
        {
            PaperID = 0;
        }

        internal Entity(ulong _ID)
        {
            PaperID = _ID;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(PaperID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T { Entity = this };
            return component;
        }

        public Entity GetEntityByName(string _Name)
        {
            ulong entityID = InternalCalls.Entity_GetEntityByName(_Name);
            if (entityID == 0)
                return null;
            return new Entity(entityID);
        }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.Entity_GetScriptInstance(PaperID);
            if (instance == null) return null;
            return instance as T;
        }

        public virtual void OnCreate() { }
        public virtual void OnDestroy() { }
        public virtual void OnUpdate(float dt) { }

        
    }
}
