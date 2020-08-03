using System;
using System.Text;
using System.Collections.Generic;

namespace lab6
{
    interface IAccount
    {
        string GetCurBalance { get; }
        string Put(int amount);
        string Withdraw(int amount);
    }

    interface IClient
    {
        string Name { get; set; }
        int Age { get; set; }
    }

	class Client : IAccount, IClient, IEquatable<Client>, IComparable<Client>
    {

        private int balance;
        private int age;

        public string Put(int amount)
        {
            balance += amount;
            return "operation was successfully completed";
        }
        public string Withdraw(int amount)
        {
            if (amount <= balance)
            {
                balance -= amount;
                return "operation was successfully completed";
            }
            else
            {
                return "operation failed";
            }
        }
        public int CompareTo(Client obj)
		{
			// return this.balance.CompareTo(obj.balance);
			if (this.balance > obj.balance)
            {
                return 1;
            }
			if (this.balance < obj.balance)
			{
				return -1;
			}
			else 
			{
				return 0;
			}
        }
        public bool Equals(Client obj)
        {
            if (obj == null)
            {
                return false;
            }
            if (CompareTo(obj) == 0)
            {
                return true;
            }
            return false;
        }


        public string GetCurBalance
        {
            get
            {
                return $"current balanсe is {balance}$";
            }
        }
        public string Name { get; set; }
        public int Age
        {
            get
            {
                return age;
            }
            set
            {
                if (value >= 18)
                {
                    age = value;
                }
                else
                {
                    age = 0;
                    Console.WriteLine("you're too young");
                }
            }
        }

		public string Show() 
		{
			return $"name '{Name}' age '{Age}' {GetCurBalance}";
		}

        public Client(string name, int balance, int age)
        {
            this.balance = balance;
            Name = name;
            Age = age;
        }
        public Client() {}
    }

    class MainClass
    {
        public static void Main(string[] args)
		{
			List<Client> list = new List<Client>();
			list.Add(new Client("Jhon", 10000, 47));
			list.Add(new Client("Jess", 15000, 30));
			list.Add(new Client("Mike", 1500, 18));
          
			void SortByBalance()
            {
                list.Sort();
                foreach (Client cl in list)
                {
                    Console.WriteLine(cl.Show());
                }
            }

            void Show()
            {
                //Console.WriteLine("Name {0} {1} age {2}", client1.Name, client1.GetCurBalance, client1.Age);
                //Console.WriteLine("Name {0} {1} age {2}", client2.Name, client2.GetCurBalance, client2.Age);
                //Console.WriteLine("Name {0} {1} age {2}", client3.Name, client3.GetCurBalance, client3.Age);
            }

            void CompareObjs()
            {
                //if (client1.CompareTo(client2) > 0)
                //{
                //    Console.WriteLine("age of client2 < client1");
                //}
                //if (client1.Equals(client1))
                //{
                //    Console.WriteLine("client1 == client1");
                //}
                //if (!client3.Equals(client4)) {
                //    Console.WriteLine("client4 is null");
                //}
            }

     

            while (true)
            {
                Console.WriteLine("\nMENU");
                Console.WriteLine("-----------------------------");
                Console.WriteLine("1) Show clients database;");
                Console.WriteLine("2) Compare two objects");
				Console.WriteLine("3) Sort by balance");
                Console.WriteLine("0) Exit\n");
                string input = Console.ReadLine();
                switch (input)
                {
                    case "1": Show(); break;
                    case "2": CompareObjs(); break;
					case "3": SortByBalance(); break;
					case "0": return;
                    default: Console.WriteLine("input error"); break;
                }
            }

        }
    }
};