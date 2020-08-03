using System;
using System.Collections.Generic;

using Xamarin.Forms;
using GameStore.Model;

namespace GameStore
{
    public partial class SettingsPage : ContentPage
    {
        public SettingsPage()
        {
            InitializeComponent();
            if ((bool)Application.Current.Properties["isDarkTheme"] == true)
            {
                darkTheme.IsToggled = true;
            }
            else
            {
                darkTheme.IsToggled = false;
            }
            if ((string)Application.Current.Properties["language"] == "en-Us")
            {
                langSwitch.IsToggled = false;
            }
            else
            {
                langSwitch.IsToggled = true;
            }
        }


        void Handle_Toggled_1(object sender, Xamarin.Forms.ToggledEventArgs e)
        {
            if (!langSwitch.IsToggled)
            {
                GameStore.Resources.Culture = new System.Globalization.CultureInfo("en-Us");
                Application.Current.Properties["language"] = "en-Us";
                langLabel.Text = "English";
            }
            else
            {
                GameStore.Resources.Culture = new System.Globalization.CultureInfo("be-By");
                Application.Current.Properties["language"] = "be-By";
                langLabel.Text = "Беларуская";
            }
            Translator.Instance.Invalidate();

        }

        void Handle_Toggled(object sender, Xamarin.Forms.ToggledEventArgs e)
        {
            if (darkTheme.IsToggled)
            {

                App.Current.Resources["backgroundColor"] = Color.FromHex("#33302E");
                App.Current.Resources["textColor"] = Color.FromHex("#FFFFFF");
                App.Current.Resources["searchbarBackground"] = Color.FromHex("#33302E");
                App.Current.Resources["buttonColor"] = Color.FromHex("#00C88C");
                Application.Current.Properties["isDarkTheme"] = true;
            }
            else
            {
                App.Current.Resources["backgroundColor"] = Color.FromHex("#FFFFFF");
                App.Current.Resources["textColor"] = Color.FromHex("#000000");
                App.Current.Resources["searchbarBackground"] = Color.FromHex("#BABABE");
                App.Current.Resources["buttonColor"] = Color.FromHex("#0076CA");
                Application.Current.Properties["isDarkTheme"] = false;
            }
        }
    }
}
