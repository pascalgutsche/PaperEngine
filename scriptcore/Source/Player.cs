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
            Console.WriteLine("Player.OnCreate()");
        }

        public override void OnDestroy()
        {
            Console.WriteLine("Player.OnDestroy()");
        }

        public override void OnUpdate(float dt)
        {
            Console.WriteLine($"Player.OnUpdate(): {dt}");
        }
    }
}
