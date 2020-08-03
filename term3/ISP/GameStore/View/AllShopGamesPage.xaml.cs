using System;
using System.Collections.Generic;
using GameStore.ViewModel;
using Xamarin.Forms;
using System.Threading.Tasks;

namespace GameStore
{
    public partial class AllShopGamesPage : ContentPage
    {
        public ShopViewModel ViewModel { get; set; }
        public AllShopGamesPage()
        {
            InitializeComponent();
            ViewModel = new ShopViewModel();
            BindingContext = ViewModel;
        }
       
        async void Handle_ItemTapped(object sender, Xamarin.Forms.ItemTappedEventArgs e)
        {
            if (e.Item == null) return;
            await Navigation.PushAsync(new OneGamePage((Game)e.Item));
            await Task.Delay(500);
            if (sender is ListView lv)
            {
                lv.SelectedItem = null;
            }
        }
    }
}
