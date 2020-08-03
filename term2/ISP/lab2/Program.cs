using System;
using System.Linq;

//Дана строка, состоящая из строчных английских букв. Заменить в ней все буквы,
//стоящие после гласных, на следующие по алфавиту(z заменяется на a)

namespace task1
{
    class MainClass
    {
        static void change(string str)
        {
            string newstr = "";
            newstr += str[0];
            for (int i = 1; i < str.Length; i++) {
                if ("aeiouy".Contains(str[i - 1])) {
                    newstr += serch(str[i]);
                } 
                else {
                    newstr += str[i];
                }
            }
            Console.WriteLine("Ваша новая строка:");
            calalph(newstr);
        }

        static char serch(char a) {
            string alphabet = "abcdefghijklmnopqrstuvwxyz";
            for (int i = 0; i < alphabet.Length; i++) {
                if (a == 'z') {
                    return 'a';
                }
                else if (a == alphabet[i]) {
                    return alphabet[i + 1];
                }
            }
            return a;
        }

        static void calalph(string b)
        {
            for (int i = 0; i < b.Length; i++)
            {
                Console.Write("{0} ", b[i]);
            }
            Console.WriteLine();
        }

        public static void Main(string[] args)
        {
            string alphabet = "abcdefghijklmnopqrstuvwxyz";
            Console.WriteLine("Введите строку: ");
            string str = Console.ReadLine();
            Console.WriteLine("Алфавит:");
            calalph(alphabet);
            change(str);
        }
    }
}