using System;
using System.Collections.ObjectModel;
namespace GameStore
{
    public class Singleton
    {
        private static Singleton shared { get; set; }
        public static Singleton GetInstance()
        {
            if (shared == null)
            {
                shared = new Singleton();
            }
            return shared;
        }
        private Singleton() {}
        public ObservableCollection<Game> Games { get; set; }
        public ObservableCollection<Game> MyGames { get; set; }
    }
}
