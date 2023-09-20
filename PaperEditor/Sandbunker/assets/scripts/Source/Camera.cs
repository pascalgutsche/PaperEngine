using System.Runtime;
using System.Web.WebPages.Instrumentation;
using Paper;

namespace Sandbox
{
    public class Camera : Entity
    {
        public float speed = 5.0f;

        private TransformComponent transformComponent;

        public override void OnCreate()
        {
            transformComponent = GetComponent<TransformComponent>();
        }

        public override void OnUpdate(float dt)
        {
            Vec3 pos = transformComponent.Position;

            if (Input.IsKeyDown(Key.W))
            {
                pos.Z -= speed * dt;
            }
            if (Input.IsKeyDown(Key.A))
            {
                pos.X -= speed * dt;
            }
            if (Input.IsKeyDown(Key.S))
            {
                pos.Z += speed * dt;
            }
            if (Input.IsKeyDown(Key.D))
            {
                pos.X += speed * dt;
            }
            if (Input.IsKeyDown(Key.E))
                pos.Y += speed * dt;
            if (Input.IsKeyDown(Key.Q))
                pos.Y -= speed * dt;

            transformComponent.Position = pos;
        }
    }
}