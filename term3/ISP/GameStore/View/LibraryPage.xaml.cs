using System;
using System.Collections.Generic;
using Xamarin.Forms;
using GameStore.ViewModel;
using System.Collections.ObjectModel;
using System.Threading.Tasks;

namespace GameStore
{

    public partial class LibraryPage : ContentPage
    {
        public LibraryViewModel ViewModel { get; set; }
        public LibraryPage()
        {
            InitializeComponent();
            ViewModel = new LibraryViewModel();
            BindingContext = ViewModel;
        }

        void Handle_Clicked(object sender, System.EventArgs e)
        {
            var selected = sender as MenuItem;
            var gameDeleted = selected?.BindingContext as Game;
            gameDeleted.IsAlreadyAddedToLibrary = false;
            ViewModel.DeleteGame(gameDeleted);
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
