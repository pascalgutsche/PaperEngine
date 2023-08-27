namespace Paper
{
    public class EntityCamera
    {
        private readonly PaperID EntityID;

        internal EntityCamera(PaperID _EntityID)
        {
            EntityID = _EntityID;
        }

        public float PerspectiveFOV
        {
            get => InternalCalls.EntityCamera_GetPerspectiveFOV(EntityID);
            set => InternalCalls.EntityCamera_SetPerspectiveFOV(EntityID, value);
        }

        public float PerspectiveNearClip
        {
            get => InternalCalls.EntityCamera_GetPerspectiveNearClip(EntityID);
            set => InternalCalls.EntityCamera_SetPerspectiveNearClip(EntityID, value);
        }

        public float PerspectiveFarClip
        {
            get => InternalCalls.EntityCamera_GetPerspectiveFarClip(EntityID);
            set => InternalCalls.EntityCamera_SetPerspectiveFarClip(EntityID, value);
        }

        public float OrthographicSize
        {
            get => InternalCalls.EntityCamera_GetOrthographicSize(EntityID);
            set => InternalCalls.EntityCamera_SetOrthographicSize(EntityID, value);
        }

        public float OrthographicNearClip
        {
            get => InternalCalls.EntityCamera_GetOrthographicNearClip(EntityID);
            set => InternalCalls.EntityCamera_SetOrthographicNearClip(EntityID, value);
        }

        public float OrthographicFarClip
        {
            get => InternalCalls.EntityCamera_GetOrthographicFarClip(EntityID);
            set => InternalCalls.EntityCamera_SetOrthographicFarClip(EntityID, value);
        }

        public Vec2 ViewportSize
        {
            set => InternalCalls.EntityCamera_SetViewportSize(EntityID, ref value);
        }
    }
}