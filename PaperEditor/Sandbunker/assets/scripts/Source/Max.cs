using System;
using System.Collections.Generic;
using System.EnterpriseServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Paper;

namespace Sandbunker
{
    public class Max : Entity
    {
        public float x = 0.0f;
        public float y = 0.0f;

        public float speedX = 3.0f;
        public float speedY = 3.0f;

        public override void OnUpdate(float dt)
        {
            TransformComponent tc = GetComponent<TransformComponent>();

            Vec3 pos = tc.Position;
            pos.X = (float)Math.Sin(x); ;
            pos.Y = (float)Math.Cos(y);

            x += speedX * dt * 2;
            y += speedY * dt;

            if (x > Math.PI * 2)
                x = 0.0f;

            if (y > Math.PI * 2)
                y = 0.0f;

            //if (x >= 3.14f || x <= -3.14f)
            //    speedX = -speedX;
            //
            //if (y >= 3.14f || y <= -3.14f)
            //    speedY = -speedY;


            tc.Position = pos;
        }
    }
}
