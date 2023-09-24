using System.Runtime;
using Paper;

namespace Sandbunker
{
    public class WTController : Entity
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