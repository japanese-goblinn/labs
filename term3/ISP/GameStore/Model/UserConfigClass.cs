using System;
namespace GameStore.Model
{
    public class UserConfigClass
    {
        public bool IsDarkTheme { get; set; }
        public string LangCulture { get; set; }

        public UserConfigClass() { }

        public UserConfigClass(bool isDarkTheme, string langCulture)
        {
            IsDarkTheme = isDarkTheme;
            LangCulture = langCulture;
        }
    }
}
