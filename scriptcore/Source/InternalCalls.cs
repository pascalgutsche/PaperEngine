using System;
using System.Runtime.CompilerServices;

namespace Paper
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void NativeLog(string _Text, int _Level = 0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void NativeLog_Vector(ref Vec3 _Parameter, out Vec3 _Result);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void IsKeyPressed(int _Code, out Boolean _Pressed);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_GetPosition(ulong _UUID, out Vec3 _Position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_SetPosition(ulong _UUID, ref Vec3 _Position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_GetRotation(ulong _UUID, out Vec3 _Rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_SetRotation(ulong _UUID, ref Vec3 _Rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_GetScale(ulong _UUID, out Vec3 _Scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void Entity_SetScale(ulong _UUID, ref Vec3 _Scale);
    }
}
