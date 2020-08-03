using System;

//Дана строка из 256 английских букв. Записать через пробел 30 символов этой
//строки, стоящих на случайных местах.Желательно сделать только одно обращение
//к классу Random.

namespace task4
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            string newstr = "";
            Random random = new Random();
            for (int i = 0; i < 256; i++) { 
            int leter = random.Next(0, alphabet.Length);
            newstr += alphabet[leter];
            }
            Console.WriteLine(newstr);
            for (int i = 0; i < 30; i++) {
                int place = random.Next(0, newstr.Length);
                Console.Write("{0} ", newstr[place]);
            }
        }
    }
}
