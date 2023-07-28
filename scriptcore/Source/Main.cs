using System;

namespace Paper
{
    public class Main
    {
        public float Bunker { get; set; }

        public Main()
        {
            Console.WriteLine("INITIALIZE MAIN!!");
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
    }
}
