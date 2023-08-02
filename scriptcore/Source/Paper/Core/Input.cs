using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Paper;

namespace Paper
{
    public static class Input
    {
        public static bool IsKeyPressed(Key _Code)
        {
            return InternalCalls.IsKeyPressed((int)_Code);
        }

        public static bool IsKeyHeld(Key _Code)
        {
            return InternalCalls.IsKeyHeld((int)_Code);
        }

        public static bool IsKeyDown(Key _Code)
        {
            return InternalCalls.IsKeyDown((int)_Code);
        }

        public static bool IsKeyReleased(Key _Code)
        {
            return InternalCalls.IsKeyReleased((int)_Code);
        }
    }
}