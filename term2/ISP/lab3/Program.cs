using System;

namespace lab3
{
    class MainClass
    {
        public static void Main(string[] args)
        {
          
            Human human1 = new Human("Kirill", 18);
            human1.City = "Minsk";
            Console.WriteLine("ID {0}, name {1}, age {2}, city {3}", human1.ShowID, human1.Name, human1.Age, human1.CityInfo());
            Human human2 = new Human("NY");
            Console.WriteLine("Enter Name:");
            human2.Name = Console.ReadLine();
            Console.WriteLine("Enter Age:");
            human2.Age = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("ID {0}, name {1}, age {2}, city {3}", human2.ShowID, human2.Name, human2.Age, human2.CityInfo());
            Console.WriteLine("ID {0}", human1.ShowID);
            human1[0] = "Honest";
            human1[1] = "Good boi";
            Console.WriteLine("Charistiristics of {0}:", human1.Name);
            for (int i = 0; i < 2; i++) {
                Console.Write("{0} ", human1[i]); 
            }
        }
    }

    class Human
    {
        public static int nextID;
        private int id;
        protected string name;
        protected int age;
        protected string city;
        protected string[] charst = new string[3];
        public Human(string name, int age)
        {
            nextID++;
            id = nextID;
            this.name = name;
            this.age = age;
        }
        public Human(string city)
        {
            nextID++;
            id = nextID;
            this.city = city;
        }
        public int ShowID {
            get {
                return id; 
            }
        }
        public string this[int i]
        {
            get
            {
                return charst[i];
            }
            set
            {
                charst[i] = value;
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
        public int Age {
            get {
                return age;
            }
            set {
                if (value > 0 && value <= 100) {
                    age = value;
                }
                else {
                    Console.WriteLine("Imposible Age");
                }
            }
        }
        public string City {
            set {
                city = value; 
            } 
        }
        public string CityInfo() {
            return city;
        }
    }
}
