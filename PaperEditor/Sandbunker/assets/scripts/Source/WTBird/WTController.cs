using System.Collections.Generic;
using System.DirectoryServices.ActiveDirectory;
using System.Runtime;
using Paper;

namespace Sandbunker
{
    public class WTController : Entity
    {
        public float speed = 5.0f;

        private TransformComponent transformComponent;

        private Entity[] background;

        public override void OnCreate()
        {
            transformComponent = GetComponent<TransformComponent>();
            background = new Entity[8];
            background[0] = GetEntityByName("BC0");
            background[1] = GetEntityByName("BC1");
            background[2] = GetEntityByName("BC2");
            background[3] = GetEntityByName("BC3");
            background[4] = GetEntityByName("BC4");
            background[5] = GetEntityByName("BC5");
            background[6] = GetEntityByName("BC6");
            background[7] = GetEntityByName("BC7");
        }

        public override void OnUpdate(float dt)
        {
            foreach (Entity bc in background)
            {
                Vec3 pos = bc.GetComponent<TransformComponent>().Position;
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

                if (pos.X < -15)
                    pos.X += 80;

                bc.GetComponent<TransformComponent>().Position = pos;
            }
        }
    }
}