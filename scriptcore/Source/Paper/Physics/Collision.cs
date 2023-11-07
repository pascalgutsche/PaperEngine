using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Paper
{
    class Collision
    {
        public static void BeginContact(ulong idA, ulong idB)
        {
            Entity entityA = new Entity(idA);
            Entity entityB = new Entity(idB);
            
            entityB.OnCollisionBeginEvent(entityA);
        }

        public static void EndContact(ulong idA, ulong idB)
        {
            Entity entityA = new Entity(idA);
            Entity entityB = new Entity(idB);

            entityB.OnCollisionBeginEvent(entityA);
        }
    }
}
