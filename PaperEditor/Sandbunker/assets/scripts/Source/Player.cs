using System;

using Paper;

namespace Sandbox
{
    public class Player : Entity
    {
        public float impulseSpeed = 3.0f;

        private Rigidbody2DComponent rigidbody;

        public override void OnCreate()
        {
            rigidbody = GetComponent<Rigidbody2DComponent>();
        }

        public override void OnUpdate(float dt)
        { 
           //TransformComponent tc = GetEntityByName("Camera").GetComponent<TransformComponent>();
           Vec2 impulse = new Vec2(0.0f);
           
           if (Input.IsKeyDown(Key.UP))
           {
               impulse.Y = 1.0f;
           }
           if (Input.IsKeyDown(Key.LEFT))
           {
               impulse.X = -1.0f;
           }
           if (Input.IsKeyDown(Key.DOWN))
           {
               impulse.Y = -1.0f;
           }
           if (Input.IsKeyDown(Key.RIGHT))
           {
               impulse.X = 1.0f;
           }

           if (Input.IsKeyDown(Key.SPACE))
           {
               impulse.Y = 10.0f;
           }

           impulse.X *= impulseSpeed * dt;
           impulse.Y *= impulseSpeed * dt;

           rigidbody.ApplyImpulse(impulse, true);
        }
    }
}
