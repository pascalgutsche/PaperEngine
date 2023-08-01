using System;

namespace Paper
{
    public class Entity
    {
        public Vec3 Position
        {
            get
            {
                InternalCalls.Entity_GetPosition(UUID, out Vec3 position);
                return position;
            }
            set
            {
                InternalCalls.Entity_SetPosition(UUID, ref value);
            }
        }

        public Vec3 Rotation
        {
            get
            {
                InternalCalls.Entity_GetRotation(UUID, out Vec3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.Entity_SetRotation(UUID, ref value);
            }
        }

        public Vec3 Scale
        {
            get
            {
                InternalCalls.Entity_GetScale(UUID, out Vec3 scale);
                return scale;
            }
            set
            {
                InternalCalls.Entity_SetScale(UUID, ref value);
            }
        }

        public readonly ulong UUID;

        protected Entity()
        {
            UUID = 0;
        }

        internal Entity(ulong _UUID)
        {
            UUID = _UUID;
        }

        public virtual void OnCreate() { }
        public virtual void OnDestroy() { }
        public virtual void OnUpdate(float dt) { }

        
    }
}
