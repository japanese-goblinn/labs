using System;
using System.Text;

namespace lab5
{
    class Human
    {
        private static int nextID;
        private int ID;
        protected string name;
        protected int age;
        protected string city;

        public Human(string name, int age, string city) 
        {
            nextID++;
            ID = nextID;
            this.name = name;
            this.age = age;
            this.city = city;
        }

        public virtual string ShowInfo()
        {
            string str = "ID " + GetID + " имя: " + Name + " возраст: " + Age + " город: " + City;
            return str;
        }

        public virtual int GetID
        {
            get
            {
                return ID;
            }
        }
        public string Name
        {
            get
            {
                return name;
            }
            set
            {
                name = value;
            }
        }
        public int Age
        {
            get
            {
                return age;
            }
            set
            {
                if (value <= 100 && value > 0)
                {
                    age = value; 
                }
                else
                {
                    Console.WriteLine("IMPOSIBLE AGE");
                }
            }
        }
        public string City
        {
            set
            {
                city = value;
            }
            get 
            {
                return city;
            }
        }
    }

    abstract class Student : Human
    {
        protected Student(string name, int age, string city) : base(name, age, city) {}

        public abstract override string ShowInfo();
    }

    class Specialist : Student
    {
        private static int nextID;
        private int ID;
        protected int mark;

        public struct Info
        {
            public string speciality;
            public string facult;
            public string univer;
            public Info(string s, string f, string u)
            {
                speciality = s;
                facult = f;
                univer = u;
            }

        }

        public Info univerInfo = new Info("ИиТП", "КСиС", "БГУИР");

        public enum Marks { One = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten };

        public Specialist(string name, int age, string city, int mark) : base(name, age, city)
        {
            nextID++;
            ID = nextID;
            this.mark = mark;
        }
        public Specialist() : base("", 0, "")
        {
            nextID++;
            ID = nextID;
        }
        public override int GetID
        {
            get
            {
                return ID;
            }
        }
        public int Mark 
        {
            get
            {
                return mark;
            }
            set
            {
                if (value >= 0 && value <= 10) {
                    mark = value;
                }
                else {
                    Console.WriteLine("НЕВЕРНОЕ ЗНАЧЕНИЕ"); 
                }
            }
        }

        public override string ShowInfo()
        {
            //return $"ID {GetID} name: {Name} ";
            string str = "ID " + GetID + " имя: " + Name + " возраст: " + Age + " город: "
                + City + " университет: " + univerInfo.univer + " факультет: " + univerInfo.facult + " специальность: " + univerInfo.speciality + " средний балл: " + Mark;
            return str;
        }
    }

    class MainClass
    {
        public static void Main(string[] args)
        {
            //new Human("asd", 12, "asd");
            //var t = new Specialist();
            //Console.WriteLine(t.GetID);
            //Console.WriteLine(((Human) t).GetID);
            //return;
            int counter = 0;

            Specialist[] students = new Specialist[10];

            Human human0 = new Human("Андрей", 15, "Брест");
            Human human1 = new Human("Катя", 16, "Минск");
            Specialist student0 = new Specialist("Кирилл", 17, "Минск", 6);

            void WhatTheMark()
            {
                Console.Write("Введите ID студента успеваемость которого нужно узнать: ");
                int checkID = Convert.ToInt32(Console.ReadLine());
                if (checkID == student0.GetID)
                {
                    Console.Write("Студент {0} - ", student0.Name);
                    if(student0.Mark < (int)Specialist.Marks.Five) 
                    {
                  
                        Console.Write("ПЛОХАЯ успеваемость\n");
                    }
                    else
                    {
                        Console.Write("ХОРОШАЯ успеваемость\n");
                    }
                }
                else if (counter > 0)
                {
                    for (int i = 0; i < counter; i++)
                    {
                        if (checkID == students[i].GetID)
                        {
                            Console.Write("Студент {0} - ", students[i].Name);       
                            if(students[i].Mark < (int)Specialist.Marks.Five)
                            {
                                Console.WriteLine("ПЛОХАЯ успеваемость");
                            }
                            else 
                            {
                                Console.WriteLine("ХОРОШАЯ успеваемость");
                            }
                        }
                    }
                }
                else
                {
                    Console.WriteLine("ТАКОЙ СТУДЕНТ НЕ НАЙДЕН");
                }
            }

            void AddStudent()
            {
                counter++;
                students[counter - 1] = new Specialist();
                Console.Write("Введите имя: ");
                students[counter - 1].Name = Console.ReadLine();
                Console.Write("\nВведите возраст: ");
                students[counter - 1].Age = Convert.ToInt32(Console.ReadLine());
                Console.Write("\nВведите город: ");
                students[counter - 1].City = Console.ReadLine();

                Console.Write("\nИзменить данные по-умолчанию о университете/факультете/специальности?\nВвод: ");
                string choose = Console.ReadLine();

                if(choose == "Да" || choose == "да" || choose == "ДА")
                {
                    Console.Write("\nВведите университет: ");
                    students[counter - 1].univerInfo.univer = Console.ReadLine();
                    Console.Write("\nВведите факультет: ");
                    students[counter - 1].univerInfo.facult = Console.ReadLine();
                    Console.Write("\nВведите специальность: ");
                    students[counter - 1].univerInfo.speciality = Console.ReadLine();
                    Console.WriteLine("\nДАННЫЕ УСПЕШНО ИЗМЕНЕНЫ...\n");
                }

                Console.Write("\nВведите средний балл: ");
                students[counter - 1].Mark = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine("");
                Console.WriteLine("\nДАННЫЕ УСПЕШНО ВНЕСЕНЫ...\n");
            }
            
            void CurrentStudets()
            {
                Human t;
                t = human0;
                Console.WriteLine(t.ShowInfo());
                t = human1;
                Console.WriteLine(t.ShowInfo());
                t = student0;
                Console.WriteLine(t.ShowInfo());
                if (counter > 0)
                {
                    for (int i = 0; i < counter; i++)
                    {
                        Console.WriteLine(students[i].ShowInfo());
                    }
                }
            }

            while (true)
            {
                Console.Write("1.Вывести имеющихся студентов/абитуриентов;\n" +
                              "2.Узнать успеваемость студента;\n" +
                              "3.Добавить нового студента;\n" +
                              "0.Выйти из программы;\n");
                Console.Write("Ввод: ");
                string choose = Console.ReadLine();
                switch (choose)
                {
                    case "1": CurrentStudets(); break;
                    case "2": WhatTheMark(); break;
                    case "3": AddStudent(); break;
                    case "0": return;
                    default: Console.WriteLine("ОШИБКА ВВОДА"); break;
                }
            }
        }
    }
}