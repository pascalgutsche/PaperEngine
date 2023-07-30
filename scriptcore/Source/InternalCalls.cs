using System;
using System.Runtime.CompilerServices;

namespace Paper
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void NativeLog(string _Text, int _Level = 0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void NativeLog_Vector(ref Vector3 _Parameter, out Vector3 _Result);
    }
}
