using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;

using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace lab4
{

    class Program
    {
        [DllImport("user32.dll")]
        public static extern int GetAsyncKeyState(Int32 i);
        static void Main(string[] args)
        {
            while (true)
            {
                Thread.Sleep(100);
                for (Int32 i = 0; i < 255; i++)
                {
                    int state = GetAsyncKeyState(i);
                    if ((state == 1) || (state == -32767))
                    {
                        Console.WriteLine((ConsoleKey)i); 
                    }

                }
            }
        }
    }

}