using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Paper;

namespace Paper
{
    public class Input
    {
        public static bool IsKeyPressed(Key _Code)
        {
            InternalCalls.IsKeyPressed((int)_Code, out Boolean pressed);
            return pressed;
        }
    }
}