using System;
using System.Collections;
using System.Collections.Generic;

//В заданной строке поменять порядок слов на обратный (слова разделены
//пробелами).

namespace task2
{
	
   
	static class MainClass
	{
		class A 
		{
			public A() 
            {
				Console.WriteLine("A");
			}
		}
		class B : A 
		{
			public B() 
			{
				Console.WriteLine("B");
			}
		}
   
		public static void Main(string[] args)
		{
			int i = 1;
				switch(i){
				case 0:
					goto default;
				case 1:
					break;
				default:
					Console.WriteLine("LOL");
					break;
			}
			//suicide IS an option 

			//string start = Console.ReadLine();
			//string newstr = "";
			//int cnt = 0;
			//for (int i = start.Length - 1; i >= 0; i--)
			//{
			//    cnt++;
			//    if (i == 0) {
			//        newstr += ' ';
			//        newstr += start.Substring(i, cnt);
			//        break;
			//    }
			//    if (start[i] == ' ')
			//    {
			//        newstr += start.Substring(i, cnt);
			//        cnt = 0;
			//    }
			//}
			//string endlstr = "";
			//endlstr += newstr.Substring(1, newstr.Length - 1);
			//Console.WriteLine(endlstr);
		}
	}
}

