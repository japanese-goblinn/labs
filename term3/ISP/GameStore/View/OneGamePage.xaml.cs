using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using GameStore.ViewModel;
using Xamarin.Forms;

namespace GameStore
{
    public partial class OneGamePage : ContentPage
    {
        public Game Game { get; set; }
        public LibraryViewModel ViewModel { get; set; }
        public OneGamePage()
        {
            InitializeComponent();
        }

        public OneGamePage(Game game)
        {
            InitializeComponent();
            Game = game;
            ViewModel = new LibraryViewModel();
            InicilizationMethod();
        }

        public void InicilizationMethod()
        {
            headerImage.Source = ImageSource.FromResource(Game.HeadImage);
            posterImage.Source = ImageSource.FromResource(Game.PosterImage);
            gameTitle.Text = Game.Name;

            releaseDateLabel.Text = Convert.ToString(Game.Year);

            if (!Game.IsAlreadyAddedToLibrary)
            {
                if ((string)Application.Current.Properties["language"] == "en-Us")
                {
                    addButton.Text = "Add To Library";
                }
                else
                {
                    addButton.Text = "Дадаць у Бібліятэку";
                }
                addButton.BackgroundColor = (Color)App.Current.Resources["buttonColor"];
                addButton.SetDynamicResource(StyleProperty, "button");
            }
            else
            {
                if ((string)Application.Current.Properties["language"] == "en-Us")
                {
                    addButton.Text = "Remove From Library";
                }
                else
                {
                    addButton.Text = "Выдаліць з Бібліятэкі";
                }
                addButton.BackgroundColor = Color.FromHex("#FD4D5E");
            }

            if (!Game.IsAlreadyReleased)
            {
                addButton.BackgroundColor = Color.FromHex("#999999");
                addButton.TextColor = Color.FromHex("#FFFFFF");
                if (Game.Year != 0)
                {
                    if ((string)Application.Current.Properties["language"] == "en-Us")
                    {
                        addButton.Text = $"Available in {Game.Year}";
                    }
                    else
                    {
                        addButton.Text = $"Даступна ў {Game.Year}";
                    }

                }
                else
                {
                    if ((string)Application.Current.Properties["language"] == "en-Us")
                    {
                        addButton.Text = "Available Soon...";
                        releaseDateLabel.Text = "Soon...";
                    }
                    else
                    {
                        addButton.Text = "Хутка Будзе Даступна...";
                        releaseDateLabel.Text = "Хутка выйдзе...";
                    }

                }
            }

            ganersLabel.Text = String.Join(", ", Game.ganers);

            descriptionLabel.Text = Game.Description;

            devName.Text = Game.Developer;
            ageLimit.Text = Game.AgeLimit;
            platformsLabel.Text = String.Join(", ", Game.platforms);

            gameLink.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(() => Device.OpenUri(new Uri(Game.MetacriticUrl))),
            });
            metascoreFrame.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(() => Device.OpenUri(new Uri(Game.CriticReviewUrl))),
            });
            userScoreFrame.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(() => Device.OpenUri(new Uri(Game.UserReviewUrl))),
            });
           
            posterImage.GestureRecognizers.Add(new TapGestureRecognizer
            {
                Command = new Command(async () => await HandleAnimation()),
            });

            if (!Game.IsAlreadyReleased)
            {
                metascoreLabel.Text = "N/A";
                userScoreLabel.Text = "N/A";
                HandleFramesColors();
                return;
            }

            metascoreLabel.Text = Convert.ToString(Game.Metascore);
            userScoreLabel.Text = Convert.ToString(Game.UserScore);
            HandleFramesColors();
        }

        async Task HandleAnimation()
        {
            await posterImage.RotateTo(360, 300);
            posterImage.Rotation = 0;
        }

        public void HandleFramesColors()
        { 
            if (!Game.IsAlreadyReleased)
            {
                metascoreFrame.BackgroundColor = Color.FromHex("#999999");
                userScoreFrame.BackgroundColor = Color.FromHex("#999999");
                metascoreLabel.Text = "N/A";
                userScoreLabel.Text = "N/A";
                metascoreLabel.TextColor = Color.FromHex("#FFFFFF");
                userScoreLabel.TextColor = Color.FromHex("#FFFFFF");
                return;
            }
            if (Game.Metascore >= 75)
            {
                metascoreFrame.BackgroundColor = Color.FromRgb(55, 215, 0);
            }
            else if (Game.Metascore >= 50 && Game.Metascore <= 74)
            {
                metascoreFrame.BackgroundColor = Color.FromRgb(255, 207, 0);
            }
            else
            {
                metascoreFrame.BackgroundColor = Color.FromRgb(255, 0, 0);
            }
            if (Game.UserScore >= 7)
            {
                userScoreFrame.BackgroundColor = Color.FromRgb(55, 215, 0);
            }
            else if (Game.UserScore <= 6 && Game.UserScore >= 4)
            {
                userScoreFrame.BackgroundColor = Color.FromRgb(255, 207, 0);
            }
            else
            {
                userScoreFrame.BackgroundColor = Color.FromRgb(255, 0, 0);
            }
        }

        void Handle_Clicked(object sender, System.EventArgs e)
        {
            if (!Game.IsAlreadyReleased)
            {
                addButton.BackgroundColor = Color.FromHex("#999999");
                addButton.TextColor = Color.FromHex("#FFFFFF");
                if (Game.Year != 0)
                {
                    if ((string)Application.Current.Properties["language"] == "en-Us")
                    {
                        addButton.Text = $"Available in {Game.Year}";
                    }
                    else
                    {
                        addButton.Text = $"Даступна ў {Game.Year}";
                    }
                }
                else
                {
                    if ((string)Application.Current.Properties["language"] == "en-Us")
                    {
                        addButton.Text = "Available Soon...";
                    }
                    else
                    {
                        addButton.Text = "{Хутка Будзе Даступна...";
                    }
                }
                return;
            }
            if (Game.IsAlreadyAddedToLibrary)
            {
                ViewModel.DeleteGame(Game);
                Game.IsAlreadyAddedToLibrary = false;
                if ((string)Application.Current.Properties["language"] == "en-Us")
                {
                    addButton.Text = "Add To Library";
                }
                else
                {
                    addButton.Text = "Дадаць у Бібліятэку";
                }
                addButton.BackgroundColor = (Color)App.Current.Resources["buttonColor"];
                addButton.SetDynamicResource(StyleProperty, "button");
                return;
            }
            ViewModel.AddGame(Game);
            Game.IsAlreadyAddedToLibrary = true;
            if ((string)Application.Current.Properties["language"] == "en-Us")
            {
                addButton.Text = "Remove From Library";
            }
            else
            {
                addButton.Text = "Выдаліць з Бібліятэкі";
            }
            addButton.BackgroundColor = Color.FromHex("#FD4D5E");
        }

        void Handle_Pressed(object sender, System.EventArgs e) => Device.OpenUri(new Uri(Game.CriticReviewUrl));

        void Handle_Pressed_1(object sender, System.EventArgs e) => Device.OpenUri(new Uri(Game.UserReviewUrl));

    }
}
