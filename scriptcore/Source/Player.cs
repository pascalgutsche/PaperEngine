using System;

using Paper;

namespace Sandbox
{
    public class Player : Entity
    {
        public Player()
        {

        }

        public override void OnCreate()
        {
        }

        public override void OnDestroy()
        {
        }

        public override void OnUpdate(float dt)
        {
            float speed = 8.0f;
            Vec3 pos = Position;
            Vec3 rot = Rotation;

            if (Input.IsKeyPressed(Key.W))
            {
                pos.Z -= speed * dt;
            }
            if (Input.IsKeyPressed(Key.A))
            {
                pos.X -= speed * dt;
            }
            if (Input.IsKeyPressed(Key.S))
            {
                pos.Z += speed * dt;
            }
            if (Input.IsKeyPressed(Key.D))
            {
                pos.X += speed * dt;
            }
            if (Input.IsKeyPressed(Key.E))
                pos.Y += speed * dt;
            if (Input.IsKeyPressed(Key.Q))
                pos.Y -= speed * dt;

            Position = pos;

        }
    }
}
