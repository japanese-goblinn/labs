using System;
using System.Linq;
using System.Text.RegularExpressions;

namespace lab_7
{

	class Fraction: IComparable<Fraction>, IEquatable<Fraction>
    {
        private int n;
        private uint m;

        public Fraction(string str)
        {
            try
			{
				//"1"
				//int x = (int)4.5;
				int[] match = Regex.Matches(str, @"\d+").Cast<Match>().Select(x => int.Parse(x.Value)).ToArray();
				int a = match[0];
				int b = match[1];
                int nod;
                if (a < 0 && b < 0)
                {
                    nod = NOD(-a, -b);
                }
                else if (a < 0)
                {
                    nod = NOD(-a, b);
                }
                else if (b < 0)
                {
                    nod = NOD(a, -b);
                }
                else
                {
                    nod = NOD(a, b);
                }
                a /= nod;
                b /= nod;
			    if (b < 0)
                {
                    b *= -1;
					a *= -1;
                }
				n = a;
				m = (uint)b;
            }
			catch (DivideByZeroException)
            {
                Console.WriteLine("Input Error");
                return;
            }
            
        }

        private int NOD(int n, int m)
        {
            while (n != 0 && m != 0)
            {
                if (n > m)
                {
                    n = n % m;
                }
                else
                {
                    m = m % n;
                }
            }
            return n + m;
        }

        public string Show
        {
            get
            {
				if (m == 1) 
				{
					return $"{n}";
				}
				else if (n == 0)
				{
					return $"{0}";
				} 
                return $"{n}/{m}";
            }
        }

		//public string ToString(string format) {
		//	"/", "-"
		//}
        public string ShowFrac(int choose)
        {
            if (choose == 1)
            {
                return $"{n}/{m}";
            }
            else
            {
                return $"\n{n}\n--\n{m}\n";
            }
        }

		public int CompareTo(Fraction obj)
        {
			if ((double)this > (double)obj)
            {
                return 1;
            }
			if ((double)this < (double)obj)
			{
				return -1;
			}
			return 0;
        }
		public bool Equals(Fraction obj)
		{
			if (CompareTo(obj) == 0)
			{
				return true;
			}
            return false;
        }

		public override bool Equals(object obj)
		{
			if (obj == null) 
			{
				return false;
			}
			Fraction frac = obj as Fraction;
			if (frac == null) 
			{
				return false;
			}
			return frac.n == this.n && frac.m == this.m;
		}
        
		public override int GetHashCode()
		{
			return (n + (int)m) / 10;
		}
       

		public static Fraction operator + (Fraction frst, Fraction secd)
        {
            int frstd = (int)frst.m;
            int secndd = (int)secd.m;

            if (secndd == frstd)
            {
                int comon = frst.n + secd.n;
                return new Fraction($"{comon}/{frst.m}");
            }
            else if (secndd > frstd)
            {
                if ((secndd % frstd) == 0)
                {
                    int comden = secndd / frstd;
                    int res = frst.n * comden;
                    int comon = res + secd.n;
                    return new Fraction($"{comon}/{secd.m}");
                }
                else
                {
                    int res1 = frst.n * secndd;
                    int res2 = secd.n * frstd;
                    int comon = res1 + res2;
                    uint den = (uint)(frstd * secndd);
                    return new Fraction($"{comon}/{den}");
                }
            }
            else if (frstd > secndd)
            {
                if ((frstd % secndd) == 0)
                {
                    int comden = frstd / secndd;
                    int res = secd.n * comden;
                    int comon = frst.n + res;
                    return new Fraction($"{comon}/{frst.m}");
                }
                else
                {
                    int res1 = frst.n * secndd;
                    int res2 = secd.n * frstd;
                    int comon = res1 + res2;
                    uint den = (uint)(frstd * secndd);
                    return new Fraction($"{comon}/{den}");
                }
            }
            return new Fraction(null);
        }

        public static Fraction operator - (Fraction frst, Fraction scnd)
        {
            int frstd = (int)frst.m;
            int secndd = (int)scnd.m;

            if (secndd == frstd)
            {
                int comon = frst.n - scnd.n;
                return new Fraction($"{comon}/{frst.m}");
            }
            else if (secndd > frstd)
            {
                if ((secndd % frstd) == 0)
                {
                    int comden = secndd / frstd;
                    int res = frst.n * comden;
                    int comon = res - scnd.n;
                    return new Fraction($"{comon}/{scnd.m}");
                }
                else
                {
                    int res1 = frst.n * secndd;
                    int res2 = scnd.n * frstd;
                    int comon = res1 - res2;
                    uint den = (uint)(frstd * secndd);
                    return new Fraction($"{comon}/{den}");
                }
            }
            else if (frstd > secndd)
            {
                if ((frstd % secndd) == 0)
                {
                    int comden = frstd / secndd;
                    int res = scnd.n * comden;
                    int comon = frst.n - res;
                    return new Fraction($"{comon}/{frst.m}");
                }
                else
                {
                    int res1 = frst.n * secndd;
                    int res2 = scnd.n * frstd;
                    int comon = res1 - res2;
                    uint den = (uint)(frstd * secndd);
                    return new Fraction($"{comon}/{den}");
                }
            }
            return new Fraction(null);
        }

        public static Fraction operator * (Fraction frst, Fraction secnd)
        {
            int res1 = frst.n * secnd.n;
            uint res2 = (uint)((int)frst.m * (int)secnd.m);
            return new Fraction($"{res1}/{res2}");
        }

        public static Fraction operator / (Fraction frst, Fraction secnd)
        {
            int res1 = frst.n * (int)secnd.m;
            int res2 = (int)frst.m * secnd.n;
            if (res2 < 0)
            {
                res1 *= -1;
                res2 *= -1;
            }
            uint a = (uint)res2;
            return new Fraction($"{res1}/{a}");
        }

        public static bool operator >= (Fraction frst, Fraction scnd)
        {
			//return frst.CompareTo(scnd) >= 0;
			if (frst.CompareTo(scnd) >= 0)
			{
				return true;
			}
			else 
			{
				return false;
			}
        }
        public static bool operator <= (Fraction frst, Fraction scnd) 
        {
			if (frst.CompareTo(scnd) <= 0) 
			{
				return true;
			}
			else 
			{
				return false;
			}
        }

		public static bool operator > (Fraction frst, Fraction scnd) => !(frst <= scnd);
		public static bool operator < (Fraction frst, Fraction scnd) => !(frst >= scnd);
      
        public static bool operator == (Fraction frst, Fraction scnd) 
        { 
			if (frst.Equals(scnd))
            {
                return true; 
            }
            else 
            {
                return false;
            }
        }
		public static bool operator != (Fraction frst, Fraction scnd) => !(frst == scnd);
        
        public static explicit operator int(Fraction frac)
        {
            int FinalInt = frac.n / (int)frac.m;
            return FinalInt;
        }
        public static explicit operator double(Fraction frac) 
        {
            double FinalDoub = (double)frac.n / (double)frac.m;
            return FinalDoub;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            
            Console.WriteLine("------------------------------");
            Console.WriteLine("Write first fraction:");
            string inpt = Console.ReadLine();
            Fraction frac1 = new Fraction(inpt);
            Console.WriteLine("Write second fraction:");
            string inpt1 = Console.ReadLine();
            Fraction frac2 = new Fraction(inpt1);

            void Show()
            {
                Console.WriteLine("Choose how to show:");
                while (true)
                {
                    Console.WriteLine("1) Show with '/'");
                    Console.WriteLine("2) Show with '--'");
                    Console.Write("Input: ");
                    string a = Console.ReadLine();
                    switch (a)
                    {
                        case "1": Console.WriteLine("\n{0} & {1}\n", frac1.ShowFrac(1), frac2.ShowFrac(1)); break;
                        case "2": Console.WriteLine("{0} \n {1}", frac1.ShowFrac(2), frac2.ShowFrac(2)); break;
                        default: break;
                    }
                    break;
                }
            }

            void Calculate() 
            {
                Console.WriteLine("Write operation:");
                string exp = Console.ReadLine();
                if (exp.Contains("+"))
                {
                    Fraction res = frac1 + frac2;
                    Console.Write("\n = ");
                    Console.WriteLine(res.Show);
                }
                else if (exp.Contains("-"))
                {
                    Fraction res = frac1 - frac2;
                    Console.Write("\n = ");
                    Console.WriteLine(res.Show);
                }
                else if (exp.Contains("*"))
                {
                    Fraction res = frac1 * frac2;
                    Console.Write("\n = ");
                    Console.WriteLine(res.Show);
                }
                else if (exp.Contains("/"))
                {
                    Fraction res = frac1 / frac2;
                    Console.Write("\n = ");
                    Console.WriteLine(res.Show);
                }
            }

            void Compare() 
            {
                Console.WriteLine("Write operation:");
                string exp = Console.ReadLine();
                if (exp.Contains(">=")) 
                {
                    Console.WriteLine(frac1 >= frac2);
                }
                else if (exp.Contains("<=")) 
                {
                    Console.WriteLine(frac1 <= frac2);
                }
                else if (exp.Contains(">"))
                {
                    Console.WriteLine(frac1 > frac2);
                }
                else if (exp.Contains("<"))
                {
                    Console.WriteLine(frac1 < frac2);
                }
                else if (exp.Contains("=="))
                {
                    Console.WriteLine(frac1 == frac2);
                }
                else if (exp.Contains("!="))
                {
                    Console.WriteLine(frac1 != frac2);
                }
            }

            void Convert() 
            {
                Console.WriteLine("Fitst in int: {0}, in double: {1}", (int)frac1, (double)frac1);
                Console.WriteLine("Second in int: {0}, in double: {1}", (int)frac2, (double)frac2);
            }

            while (true)
            {
                Console.WriteLine("\n------------------------------");
                Console.WriteLine("1) Show fractions in different ways");
                Console.WriteLine("2) Calculate fractions");
                Console.WriteLine("3) Compare fractions");
                Console.WriteLine("4) Make explicit convertation to int and double");
                Console.WriteLine("0) Exit");
				Console.WriteLine("------------------------------\n");
                Console.Write("Input: ");
                string input = Console.ReadLine();
                switch (input)
                {
                    case "1": Show(); break;
                    case "2": Calculate(); break;
                    case "3": Compare(); break;
                    case "4": Convert(); break;
                    case "0": return;
                    default: Console.WriteLine("!input error!"); break;
                }
            }
        }
    }
}
