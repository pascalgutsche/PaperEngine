using System.Runtime;
using System.Web.WebPages.Instrumentation;
using Paper;

namespace Sandbox
{
    public class Camera : Entity
    {
        public float distanceFromFocusedEntity = 5.0f;

        public Vec3 pos;

        private TransformComponent TC;

        public Entity focusedEntity;
        public override void OnCreate()
        {
            //focusedEntity = GetEntityByName("PlayerSprite");
            TC = GetComponent<TransformComponent>();
        }

        public override void OnUpdate(float dt)
        {
            if(focusedEntity ==  null) return;
            TransformComponent focusedEntityTC = focusedEntity.GetComponent<TransformComponent>();

            if (focusedEntityTC == null) return;
            Vec3 fpos = focusedEntityTC.Position;

            TC.Position = new Vec3(fpos.X, fpos.Y, fpos.Z + distanceFromFocusedEntity);
        }
    }
}