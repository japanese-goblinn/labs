using System;
using System.Collections.Generic;
using GameStore.ViewModel;
using System.Linq;
using Xamarin.Forms;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.ObjectModel;
namespace GameStore
{
    public partial class ShopPage : ContentPage
    {
        public ShopViewModel ViewModel { get; set; }
        public ShopPage()
        {
            InitializeComponent();
            ViewModel = new ShopViewModel();
            BindingContext = ViewModel;
            var elementsQuantity = ViewModel.Games.Count;

            var soon = from item in ViewModel.Games where item.Metascore == 0 select item;
            var top = from item in ViewModel.Games where item.Metascore >= 95 select item;
            var others = from item in ViewModel.Games where item.Metascore != 0 && item.Metascore < 95 select item;

            var soonList = soon.ToList();
            var topList = top.ToList();
            var othersList = others.ToList();

            for (int i = 0; i < soonList.Count(); i++)
            {
                newAndNoteworthyStackView.Children.Add(createBasicMovieCell(soonList[i], soonList[i].Name, soonList[i].Developer));      
            }
              
            for (int i = 0; i < topList.Count(); i++)
            {
                topGamesStack.Children.Add(createBasicMovieCell(topList[i], topList[i].Name, topList[i].Developer));
            }

            for (int i = 0; i < othersList.Count(); i++)
            {
                firstCoverGameStack.Children.Add(createImageMovieCell(othersList[i], othersList[i].HeadImage));
                othersList.Remove(othersList[i]);
            }

            for (int i = 0; i < othersList.Count(); i++)
            {
                secondCoverGameStack.Children.Add(createImageMovieCell(othersList[i], othersList[i].HeadImage));
                othersList.Remove(othersList[i]);
            }

            var othersList2 = top.ToList();
            othersList2.Reverse();
            createHeaderView(othersList2);

            var repetition = 1;
            Device.StartTimer(TimeSpan.FromSeconds(5), () =>
            {
                switch (repetition)
                {
                    case 1:
                        HeaderScrollView.ScrollToAsync(375, 0, true);
                        repetition = 2;
                        break;
                    case 2:
                        HeaderScrollView.ScrollToAsync(750, 0, true);
                        repetition = 3;
                        break;
                    case 3:
                        HeaderScrollView.ScrollToAsync(0, 0, true);
                        repetition = 1;
                        break;
                    default: break;
                }
                return true;
            });
        }

        StackLayout createBasicMovieCell(Game game, string gameName, string genre)
        {
            var stackLayout = new StackLayout()
            {
                HeightRequest = 100,
                WidthRequest = 95,
                Spacing = 9
            };

            var image = new Image { HeightRequest = 142, WidthRequest = 95 };
            image.Source = ImageSource.FromResource(game.PosterImage);
            image.Aspect = Aspect.AspectFill;

            var descriptionStack = new StackLayout { Spacing = 3, Padding = new Thickness(3, 0, 3, 0) };

            var titleLabel = new Label
            {
                Text = gameName,
                HeightRequest = 30,
                WidthRequest = 95,
                FontSize = 12,
                FontAttributes = FontAttributes.Bold,
                LineBreakMode = LineBreakMode.WordWrap,
            };
            titleLabel.SetDynamicResource(StyleProperty, "label");
            var genreLabel = new Label
            {
                Text = genre,
                HeightRequest = 12,
                WidthRequest = 95,
                FontSize = 12,
                LineBreakMode = LineBreakMode.TailTruncation,
            };
            genreLabel.SetDynamicResource(StyleProperty, "label");
            descriptionStack.Children.Add(titleLabel);
            descriptionStack.Children.Add(genreLabel);
            stackLayout.Children.Add(image);
            stackLayout.Children.Add(descriptionStack);

            stackLayout.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(() => Navigation.PushAsync(new OneGamePage(game)))
            });

            return stackLayout;
        }



        Frame createImageMovieCell(Game game, string imageName)
        {
            var frame = new Frame
            {
                CornerRadius = 9,
                WidthRequest = 205,
                HeightRequest = 100,
                Padding = 0,
                HasShadow = false,
                IsClippedToBounds = true
            };

            var image = new Image { HeightRequest = 142, WidthRequest = 95 };
            image.Source = ImageSource.FromResource(imageName);
            image.Aspect = Aspect.AspectFill;

            frame.Content = image;

            frame.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(() => Navigation.PushAsync(new OneGamePage(game)))
            });

            return frame;
        }

        void createHeaderView(List<Game> someGames)
        {

            foreach (var game in someGames)
            {
                var contentView = new ContentView();
                var image = new Image { HeightRequest = 200, Aspect = Aspect.AspectFill, WidthRequest = 375 };
                image.Source = ImageSource.FromResource(game.HeadImage);
                contentView.Content = image;

                contentView.GestureRecognizers.Add(new TapGestureRecognizer
                {
                    Command = new Command(() => Navigation.PushAsync(new OneGamePage(game)))
                });

                headerStack.Children.Add(contentView);
            }
        }

        void Handle_Pressed(object sender, System.EventArgs e)
        {
            ShowAnimation();
            Navigation.PushAsync(new AllShopGamesPage());
        }

        async void ShowAnimation()
        {
            await Navigation.PushModalAsync(new AnimationPage());
            await Task.Delay(900);
            await Navigation.PopModalAsync();
        }
    }
}
