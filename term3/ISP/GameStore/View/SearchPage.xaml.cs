using System;
using System.Linq;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Xamarin.Forms;
using Lottie.Forms.EventArguments;
using GameStore.ViewModel;

namespace GameStore
{
    public partial class SearchPage : ContentPage
    {
        ShopViewModel ViewModel { get; set; }
        public SearchPage()
        {
            InitializeComponent();
            ViewModel = new ShopViewModel();
            BindingContext = this.ViewModel;
        }


        void Handle_TextChanged(object sender, Xamarin.Forms.TextChangedEventArgs e)
        {

            var text = searchBar?.Text?.ToUpperInvariant();
            if (e.NewTextValue == null)
            {
                searchList.ItemsSource = null;
                return;
            }

            var selectedGames = from item in ViewModel.Games where
            item.Name.ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture) ||
            item.Developer.ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture) ||
            item.AgeLimit.ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture) ||
            Convert.ToString(item.Year).ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture)
            orderby item.Name select item;

            if (!selectedGames.Any())
            {

                var selectedGamesByGenere = from game in ViewModel.Games
                               from ganere in game.ganers
                               where
                               ganere.ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture)
                               orderby game.Name
                               select game;
                               
                               
                               
                                if (!selectedGamesByGenere.Any())
                                {
                                     var selectedGamesByPlatforms = from game in ViewModel.Games
                                                         from platform in game.platforms
                                                         where
                                                         platform.ToUpperInvariant().StartsWith(text, StringComparison.CurrentCulture)
                                                         orderby game.Name
                                                         select game;
                                                      
                                                         searchList.ItemsSource = string.IsNullOrEmpty(e.NewTextValue) ? null : selectedGamesByPlatforms;
                                                         return;
                                }

                                searchList.ItemsSource = string.IsNullOrEmpty(e.NewTextValue) ? null : selectedGamesByGenere;
                                return;
            }

            searchList.ItemsSource = string.IsNullOrEmpty(e.NewTextValue) ? null : selectedGames;
        }

      


        async void Handle_ItemTapped(object sender, Xamarin.Forms.ItemTappedEventArgs e)
        {
            if (e.Item == null) return;
            await Navigation.PushAsync(new OneGamePage((Game)e.Item));
            await Task.Delay(200);
            if (sender is ListView lv)
            {
                lv.SelectedItem = null;
            }
        }


    }      
}
