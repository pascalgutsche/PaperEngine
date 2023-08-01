using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;

namespace Paper
{
    public struct Vec3
    {
        public float X, Y, Z;

        public Vec3(float _X, float _Y, float _Z)
        {
            X = _X; 
            Y = _Y; 
            Z = _Z;
        }
    }

    public class Main
    {
        public float Bunker { get; set; }

        public Main()
        {
            Console.WriteLine("INITIALIZE MAIN!!");
            Log("BunkerGamer", 0);
            Log("BunkerGamer", 1);
            Log("BunkerGamer", 2);
            Log("BunkerGamer", 3);
            Log("BunkerGamerDEBUG");

            Vec3 pos = new Vec3(88.759834f, 42.2f, 69.007f);
            Vec3 returnVal = Log(pos);
            Log($"C# VECTOR: {returnVal.X}, {returnVal.Y}, {returnVal.Z}", 3);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Welcome to C#");
        }

        public void PrintIntMessage(int value)
        {
            Console.WriteLine($"C# value says: {value}");
        }

        public void PrintIntsMessage(int value, int value2)
        {
            Console.WriteLine($"C# value says: {value}, {value2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        public void Log(string text, int level = 0)
        {
            InternalCalls.NativeLog(text, level);
        }
        
        public Vec3 Log(Vec3 parameter)
        {
            InternalCalls.NativeLog_Vector(ref parameter, out Vec3 result);
            return result;
        }

        
    }
}
