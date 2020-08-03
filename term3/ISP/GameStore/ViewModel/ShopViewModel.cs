using System;
using System.Collections.ObjectModel;
using GameStore.Model;
using Xamarin.Forms;
using System.IO;
using Newtonsoft.Json;
using System.Reflection;
using System.IO.Compression;

namespace GameStore.ViewModel
{
    public class ShopViewModel
    {
        public ObservableCollection<Game> Games { get; set;  }

        public ShopViewModel()
        {      
            var library = Singleton.GetInstance();
            Games = library.Games ?? (library.Games = HandleFile());
        }

        public ObservableCollection<Game> HandleFile() 
        {
            var assembly = typeof(ShopViewModel).GetTypeInfo().Assembly;
            Stream stream = assembly.GetManifestResourceStream("GameStore.AppData.json");
  
            using (var reader = new StreamReader(stream))
            {
                string info = reader.ReadToEnd();
                string fileName = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "СompressedData.bin");
                string key = info.HashSHA512();
                using (Stream s = File.Create(fileName))
                {
                    using (var ds = new DeflateStream(s, CompressionMode.Compress))
                    {
                        using (TextWriter w = new StreamWriter(ds))
                        {
                            w.Write(info.Encrypt(key));
                        }
                    }
                }
                string result;
                using (Stream s = File.OpenRead(fileName))
                {
                    using (var ds = new DeflateStream(s, CompressionMode.Decompress))
                    {
                        using (TextReader r = new StreamReader(ds))
                        {
                            info = r.ReadToEnd();
                            result = info.Decrypt(key);
                        }
                    }
                }

                return JsonConvert.DeserializeObject<ObservableCollection<Game>>(result);
            }
        }
    }
}
