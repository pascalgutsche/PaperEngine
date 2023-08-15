using System;

using Paper;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent transformComponent;
        private SpriteComponent spriteComponent;
        private int count = 1;

        public float speed = 3.004f;

        //public string bunker = "bunker123";

        public char charVar = 'b';

        public override void OnCreate()
        {
            transformComponent = GetComponent<TransformComponent>();
            spriteComponent = GetComponent<SpriteComponent>();
        }

        public override void OnDestroy()
        {
        }

        

        public override void OnUpdate(float dt)
        {
            //TransformComponent tc = GetEntityByName("Camera").GetComponent<TransformComponent>();
            Vec3 pos = transformComponent.Position;

            if (Input.IsKeyDown(Key.W))
            {
                pos.Z += speed * dt;
            }
            if (Input.IsKeyDown(Key.A))
            {
                pos.X += speed * dt;
            }
            if (Input.IsKeyDown(Key.S))
            {
                pos.Z -= speed * dt;
            }
            if (Input.IsKeyDown(Key.D))
            {
                pos.X -= speed * dt;
            }
            if (Input.IsKeyDown(Key.E))
                pos.Y += speed * dt;
            if (Input.IsKeyDown(Key.Q))
                pos.Y -= speed * dt * 2;

            transformComponent.Position = pos;

            Camera camera = GetEntityByName("Camera").As<Camera>();
            if (camera != null)
            {
                if (Input.IsKeyDown(Key.F))
                {
                    camera.distanceFromFocusedEntity -= speed * dt;
                }
                if (Input.IsKeyDown(Key.G))
                {
                    camera.distanceFromFocusedEntity += speed * dt;
                }
            }

            if (Input.IsKeyReleased(Key.P))
            {
                spriteComponent.Texture = new Texture("assets/textures/putain.jpg");
                transformComponent.Scale = new Vec3(2.0f, 2.0f, 1.0f);
                spriteComponent.Geometry = Geometry.Circle;
                spriteComponent.Color = new Vec4(1.0f, 0.0f, 0.0f, 1.0f);
                spriteComponent.TilingFactor = count++;
            }
        }
    }
}
