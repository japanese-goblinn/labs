using Xamarin.Forms;
using System.IO;
using Newtonsoft.Json;
using System.Reflection;
using System.Collections.ObjectModel;
using System.Windows.Input;
using System.IO.Compression;

namespace GameStore.ViewModel
{
    public class LibraryViewModel 
    {
        public ObservableCollection<Game> Games { get; set; }

        public LibraryViewModel() 
        {
         
            var library = Singleton.GetInstance();
            Games = library.MyGames ?? (library.MyGames = new ObservableCollection<Game>());
        }

        public void DeleteGame(Game game)
        {
            Games.Remove(game);
        }

        public void AddGame(Game game) 
        {
            Games.Add(game);
        }

    }
}
