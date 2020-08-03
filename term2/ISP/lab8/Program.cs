using System;
using System.Collections.Generic;

/* 
Необходимо дополнить классы, созданные в ходе лабораторных работ 3, 5 и 6, событиями. 
Тестирование событий выполнить при помощи назначения обработчиков, записанных в форме анонимных методов и лямбда-выражении.
Реализовать в методах классов генерацию исключительных ситуаций (в тех случаях, когда что-либо мешает нормальному выполнению операции)
и предусмотреть обработку этих исключений в коде, использующем эти классы. По возможности, заменить некоторые из ранее разработанных методов методами,
принимающими в качестве параметра делегат.
*/

namespace lab_8
{
	interface IAccount
    {
        int GetCurBalance { get; }
		void Put(int amount);
		void Withdraw(int amount);
    }

    interface IClient
    {
        string Name { get; set; }
        int Age { get; set; }
    }

	public delegate void OnAction(int onMoneyAction);


	class Client : IAccount, IClient, IEquatable<Client>, IComparable<Client>
	{
		public event OnAction OnPut;
		public event OnAction OnWithdraw;

		private int balance;
		private int age;
        
		public void Put(int amount) 
        {
			balance += amount;
			OnPut(amount);
		}
		public void Withdraw (int amount) 
		{
			if (OnWithdraw != null)
			{
				balance -= amount;
				OnWithdraw(amount);
				
			}
		}
      
        public int CompareTo(Client obj)
        {
             return this.balance.CompareTo(obj.balance);
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
        public int GetCurBalance
        {
            get
            {
				return balance;
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
        public Client() { }
    }
    
	class Program
	{
		static void Main(string[] args)
		{
			List<Client> clients = new List<Client>();
			clients.Add(new Client("Jhon", 10000, 47));
            clients.Add(new Client("Jess", 15000, 30));
			clients.Add(new Client("Mike", 1500, 18));

            
            void SortByBalance()
            {
                clients.Sort();
                foreach (Client cl in clients)
                {
                    Console.WriteLine(cl.Show());
                }
            }

			void AddMoneyOnBalance() 
			{
				bool checker = false;
               
				Console.WriteLine("What amount your want to add?");
                string amount = Console.ReadLine();

				Console.WriteLine("On whose balance your whant to add money?");
				string name = Console.ReadLine();

				foreach (Client who in clients) 
				{
					if (who.Name == name)
					{
						checker = true;

						who.OnPut += onMoneyAction => Console.WriteLine($"On {who.Name} balance was added {onMoneyAction}$");

						try
						{
							who.Put(Convert.ToInt32(amount));
						}
						catch (Exception)
						{
							Console.WriteLine("Inccorect input!");
						}
                       
						break;
					}
				}
				if (checker == false)
                {
                    Console.WriteLine($"No client in data base with name '{name}'!");
                }
                
			}

			void WithdrawMoneyFromBalance() 
			{
				bool checker = false;

				Console.WriteLine("What amount your want to withdraw?");
                string amount = Console.ReadLine();

				Console.WriteLine("From whose balance your whant to withdraw money?");
                string name = Console.ReadLine();
                
                foreach (Client who in clients)
                {
                    if (who.Name == name)
                    {
						checker = true;

						who.OnWithdraw += delegate (int onMoneyAction)
						{
							Console.WriteLine($"From {who.Name} balance was withdrawn {onMoneyAction}$");

						};

						try
						{
							if (who.GetCurBalance >= Convert.ToInt32(amount))
							{
								who.Withdraw(Convert.ToInt32(amount));
							}
							else
							{
								Console.WriteLine("Error! Current balance dont have enough money!");
							}
						}
						catch (Exception e) 
						{
							//Console.WriteLine(e.GetType());
							Console.WriteLine("Inccorect input!");
						}

                        break;
                    }
                }
				if (checker == false) 
				{
					Console.WriteLine($"No client in data base with name '{name}'!");
				}
			}

            void Show()
            {
				foreach (Client cl in clients) 
				{
					Console.WriteLine("Name {0} {1} age {2}", cl.Name, cl.GetCurBalance, cl.Age);
				}
            }
            
            while (true)
            {
                Console.WriteLine("\nMENU");
                Console.WriteLine("-----------------------------");
                Console.WriteLine("1) Show clients database;");
                Console.WriteLine("2) Sort by balance");
				Console.WriteLine("3) Add money on balance");
				Console.WriteLine("4) Withdraw money from balance");
                Console.WriteLine("0) Exit\n");
                string input = Console.ReadLine();
                switch (input)
                {
					case "1": Show(); break;
                    case "2": SortByBalance(); break;
					case "3": AddMoneyOnBalance(); break;
					case "4": WithdrawMoneyFromBalance(); break;
                    case "0": return;
                    default: Console.WriteLine("input error"); break;
                }
            }
			
		}
	}
}
