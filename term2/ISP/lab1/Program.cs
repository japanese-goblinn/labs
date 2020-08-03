using System;


namespace lab1
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            //игра угадай число
            Console.WriteLine("В этой игре компьютер загадывает число в пределах от 1 до 30, а пользователь пытается его отгадать");
            Console.WriteLine("Генерирую число...");
            Random random = new Random();
            int num = random.Next(1, 30);
            bool result = true;
            int counter = 0;
            while (result)
            {
                Console.Write("Введите число: ");
                int res = Convert.ToInt32(Console.ReadLine());
                if (num == res)
                {
                    counter++;
                    Console.WriteLine("Поздравляю, вы отгадали число {0} всего за {1} раз(a)\n", num, counter);
                    result = false;
                }
                else
                {
                    Console.Write("Неправильно, попробуйте снова, ");
                    if (res > num) {
                        Console.Write("ваше число больше\n");
                    }
                    else {
                        Console.Write("ваше число меньше\n");
                    }
                    counter++;
                }
            }
        }
    }
}