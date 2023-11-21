using System;
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

        private Entity[] towers;

        private Entity airbunker;

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

            towers = new Entity[34];
            for (int i = 0; i < 34; i++)
            {
                towers[i] = GetEntityByName("TOWER" + i);
            }

            airbunker = GetEntityByName("AIRBUNKER");
        }

        public override void OnUpdate(float dt)
        {
            foreach (Entity bc in background)
            {
                Move(bc, dt);
            }

            foreach (Entity tw in towers)
            {
                Move(tw, dt);
            }

            if (Input.IsKeyPressed(Key.SPACE))
            {

                airbunker.GetComponent<Rigidbody2DComponent>().ApplyImpulse(new Vec2(0, 1), true);
            }
        }

        private void Move(Entity entity, float dt)
        {
            Vec3 pos = entity.GetComponent<TransformComponent>().Position;
            pos.X -= speed * dt;
            
            if (pos.X < -15)
                pos.X += 80;

            

            entity.GetComponent<TransformComponent>().Position = pos;

        }
    }
}