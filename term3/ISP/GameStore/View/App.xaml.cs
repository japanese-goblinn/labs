using System;
using Xamarin.Forms;
using System.Globalization;
using Newtonsoft.Json;
using GameStore.Model;
using Xamarin.Forms.Xaml;
using System.Reflection;
using System.IO;
using System.IO.Compression;

[assembly: XamlCompilation(XamlCompilationOptions.Compile)]
namespace GameStore
{
    public partial class App : Application
    {
        public UserConfigClass UserConfig { get; set; }
        public App()
        {
            InitializeComponent();

            string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "UserDataCompressed.bin");
            
            //string json = JsonConvert.SerializeObject(UserConfig, Formatting.Indented);

            //string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "UserDataCompressed.bin");
            //using (Stream s = File.Open(path, FileMode.OpenOrCreate))
            //{
            //    using (var ds = new DeflateStream(s, CompressionMode.Compress))
            //    {
            //        using (StreamWriter writer = new StreamWriter(ds))
            //        {
            //            writer.WriteAsync(json);
            //        }
            //    }
            //}

            using (Stream s = File.OpenRead(path))
            {
                using (var ds = new DeflateStream(s, CompressionMode.Decompress))
                {
                    using (TextReader read = new StreamReader(ds))
                    {
                        try
                        {
                            UserConfig = JsonConvert.DeserializeObject<UserConfigClass>(read.ReadToEnd());
                            //UserConfig = new UserConfigClass(false, "en-Us");
                        }
                        catch (Exception)
                        {
                            UserConfig = new UserConfigClass(false, "en-Us");
                        }
                    }
                }
            }
            Application.Current.Properties["language"] = UserConfig.LangCulture;
            if (UserConfig.LangCulture == "en-Us")
            {
                
                GameStore.Resources.Culture = new CultureInfo("en-Us");
            }
            else
            {
                GameStore.Resources.Culture = new CultureInfo("be-By");
            }
            Application.Current.Properties["isDarkTheme"] = UserConfig.IsDarkTheme;
            if (UserConfig.IsDarkTheme == false)
            {
                App.Current.Resources["backgroundColor"] = Color.FromHex("#FFFFFF");
                App.Current.Resources["textColor"] = Color.FromHex("#000000");
                App.Current.Resources["searchbarBackground"] = Color.FromHex("#BABABE");
                App.Current.Resources["buttonColor"] = Color.FromHex("#0076CA");
            }
            else
            {
                App.Current.Resources["backgroundColor"] = Color.FromHex("#33302E");
                App.Current.Resources["textColor"] = Color.FromHex("#FFFFFF");
                App.Current.Resources["searchbarBackground"] = Color.FromHex("#33302E");
                App.Current.Resources["buttonColor"] = Color.FromHex("#00C88C");
            }
            MainPage = new MainPage();
        }

        protected override void OnStart()
        {
            // Handle when your app starts

        }

       

        protected override void OnSleep()
        {
            UserConfig.LangCulture = Application.Current.Properties["language"] as string;
            UserConfig.IsDarkTheme = (bool)Application.Current.Properties["isDarkTheme"];
            string json = JsonConvert.SerializeObject(UserConfig, Formatting.Indented);

            string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "UserDataCompressed.bin");
            using (Stream s = File.Open(path, FileMode.OpenOrCreate))
            {
                using (var ds = new DeflateStream(s, CompressionMode.Compress))
                {
                    using (StreamWriter writer = new StreamWriter(ds))
                    {
                        writer.WriteAsync(json);
                    }
                }
            }
        }

        protected override void OnResume()
        {
            // Handle when your app resumes
        }
    }
}
