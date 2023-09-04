using System;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Windows.Forms;
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

        public static bool IsMouseButtonPressed(MouseButton _Button)
        {
            return InternalCalls.IsMouseButtonPressed((int)_Button);
        }

        public static bool IsMouseButtonHeld(MouseButton _Button)
        {
            return InternalCalls.IsMouseButtonHeld((int)_Button);
        }

        public static bool IsMouseButtonDown(MouseButton _Button)
        {
            return InternalCalls.IsMouseButtonDown((int)_Button);
        }

        public static bool IsMouseButtonReleased(MouseButton _Button)
        {
            return InternalCalls.IsMouseButtonReleased((int)_Button);
        }
    }
}