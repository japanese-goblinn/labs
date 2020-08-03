using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace Metrology
{
    class Kotlin
    {
        public string Code { get; private set; }

        public Kotlin()
        {
            Init();
        }

        private void Init()
        {
            using (FileStream fstream = File.OpenRead("sample.txt"))
            {
                byte[] array = new byte[fstream.Length];             
                fstream.Read(array, 0, array.Length);
                Code = System.Text.Encoding.Default.GetString(array);
            }
            MaxLevlOfNesting();
        }

        private void MaxLevlOfNesting() 
        {
            var codeCopy = Code;
            var stack = new Stack<string>();
            var maxLevlOfNest = new List<int>();
            var codeForAnalize = new List<string>();
            AddToListCodeSegments();

            int result = LevelOfNesting();
            Console.WriteLine($"Максимальный уровень вложенности: {result}");
           
            int LevelOfNesting()
            {
                foreach (string block in codeForAnalize)
                {
                    int k;
                    string beginsFromWhat = "";
                    bool lastIsOpen = false;
                    int constant = 0;
                    for (k = 0; block[k] != '\n'; k++)
                    {
                        beginsFromWhat += block[k];
                        if (beginsFromWhat.Contains("when"))
                        {
                            constant = -2;
                            break;
                        }
                    }
                    for (int i = k; i < block.Length; i++)
                    {
                        if (((i + "else".Length < block.Length) && (block.Substring(i, "else".Length) == "else") && (block["else".Length + i + 1] == '-')))
                        {
                            constant--; 
                        }
                        else if ((i + "else if".Length < block.Length) && ((block[i] == '-' && block[i + 1] == '>') || ((block.Substring(i, "else if".Length) == "else if"))))
                        {
                            constant++;
                        }
                        if ((i + "when".Length < block.Length) && (block.Substring(i, "when".Length) == "when"))
                        {
                            constant -= 2;
                        }
                        if (block[i] == '{')
                        {
                            stack.Push(Convert.ToString(block[i]));
                            lastIsOpen = true;
                        }
                        if (block[i] == '}')
                        {
                            if (lastIsOpen)
                            {
                                int stackCount = stack.Count + constant - 1;
                                maxLevlOfNest.Add(stackCount);
                            }
                            stack.Pop();
                            lastIsOpen = false;
                        }
                     
                    }
                }
               
                return maxLevlOfNest.Max();
            }

            void AddToListCodeSegments() 
            {
                while (true) 
                {
                    string word = ParseAndCompareMethod();
                    if (word is null) 
                    {
                        break;
                    }
                    if (codeCopy.Contains(word)) 
                    {
                        var startAndEnd = GetStartAndLastIndex(codeCopy, word);
                        codeForAnalize.Add(codeCopy.Substring(startAndEnd.Item1, startAndEnd.Item2 - startAndEnd.Item1));
                        codeCopy = codeCopy.Remove(startAndEnd.Item1, startAndEnd.Item2 - startAndEnd.Item1);
                    }
                } 
            }

            string ParseAndCompareMethod() 
            {
                string[] localCopy = codeCopy.Split(new char[] { ' ', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                for (int i = 0; i < localCopy.Length; i++) 
                {
                    if (localCopy[i] == "else" && localCopy[i + 1] == "if") 
                    {
                        return "else if";
                    }
                    else if (localCopy[i] == "if" || localCopy[i] == "else" ||
                             localCopy[i] == "for" || localCopy[i] == "while" ||
                             localCopy[i] == "do" || localCopy[i] == "when") 
                    {
                        return localCopy[i];
                    }
                }
                return null;
             }

            (int, int) GetStartAndLastIndex(string src, string start)
            {
                int _start;
                if (src.Contains(start))
                {
                    _start = src.IndexOf(start, 0, StringComparison.Ordinal);
                    int openCurlyBraces = 0;
                    int closedCurlyBraces = 0;
                    for (int i = _start; i < src.Length; i++) 
                    {
                        if (closedCurlyBraces == openCurlyBraces && openCurlyBraces != 0)
                        {
                            return (_start, i);
                        }
                        if (src[i] == '{')
                        {
                            openCurlyBraces++;
                        }
                        else if (src[i] == '}')
                        {
                            closedCurlyBraces++;
                        }
                    }
                }
                return (0, 0);
            }
        }
        
    }
    class Program
    {
        static void Main(string[] args)
        {
            Kotlin kotlin = new Kotlin();
        }
    }
}
