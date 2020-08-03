using System;
using System.Collections.Generic;
using GameStore.ViewModel;
using System.Linq;
using Microcharts;
using SkiaSharp;
using System.Threading.Tasks;

using Xamarin.Forms;

namespace GameStore
{
    public partial class StatisticsPage : ContentPage
    {
        ShopViewModel ViewModel { get; set; }
        LibraryViewModel LibraryViewModel { get; set; }
        Dictionary<string, int> platforms;
        public StatisticsPage()
        {
            InitializeComponent();
            ViewModel = new ShopViewModel();
            LibraryViewModel = new LibraryViewModel();

        }

        protected override void OnAppearing()
        {
            base.OnAppearing();

            platforms = new Dictionary<string, int>();
            Dictionary<string, int> years = new Dictionary<string, int>();
            List<int> vs = new List<int>();
            foreach (Game game in ViewModel.Games)
            {
                if (game.Year == 0)
                {
                    vs.Add(2077);
                    continue;
                }
                vs.Add(game.Year);
            }
            vs.Sort();
            foreach (int year in vs)
            {
                if (years.ContainsKey(Convert.ToString(year)))
                {
                    years[Convert.ToString(year)] += 1;
                }
                else
                {
                    years.Add(Convert.ToString(year), 1);
                }
            }

            Microcharts.Entry[] yearStats = new Microcharts.Entry[years.Count];
            int g = 0;
            foreach (var element in years)
            {
                var random = new Random((int)DateTime.Now.Ticks);
                var color = String.Format("#{0:X6}", random.Next(0x1000000));

                yearStats[g] = new Microcharts.Entry(element.Value)
                {
                    Label = element.Key,
                    ValueLabel = $"{element.Value}",
                    Color = SKColor.Parse(color),
                    TextColor = SKColor.Parse(color)
                };
                g++;
            }

            foreach (Game game in ViewModel.Games)
            {
                foreach (string platform in game.platforms)
                {
                    if (platforms.ContainsKey(platform))
                    {
                        platforms[platform] += 1;
                    }
                    else
                    {
                        platforms.Add(platform, 1);
                    }
                }
            }

            Microcharts.Entry[] platStats = new Microcharts.Entry[platforms.Count];
            int k = 0;
            foreach (var element in platforms)
            {
                var random = new Random((int)DateTime.Now.Ticks);
                var color = String.Format("#{0:X6}", random.Next(0x1000000));

                platStats[k] = new Microcharts.Entry(element.Value)
                {
                    Label = element.Key,
                    ValueLabel = $"{element.Value}",
                    Color = SKColor.Parse(color),
                    TextColor = SKColor.Parse(color)
                };
                k++;
            }

            var entries = new[]
            {
                new Microcharts.Entry(ViewModel.Games.Count)
                {
                    Label = "Shop",
                    ValueLabel = $"{ViewModel.Games.Count}",
                    Color = SKColor.Parse("#fed766"),
                    TextColor = SKColor.Parse("#fed766")
                },

                new Microcharts.Entry(LibraryViewModel.Games.Count)
                {
                    Label = "Library",
                    ValueLabel = $"{LibraryViewModel.Games.Count}",
                    Color = SKColor.Parse("#fe4a49"),
                    TextColor = SKColor.Parse("#fe4a49")
                }
            };

            var chart = new DonutChart() { Entries = platStats, BackgroundColor = SKColor.Empty, LabelTextSize = 23};
            var chart1 = new RadarChart() { Entries = yearStats, BackgroundColor = SKColor.Empty, LabelTextSize = 23 };
            var chart2 = new LineChart() { Entries = entries, BackgroundColor = SKColor.Empty,  LabelTextSize = 23 };

            chartView.Chart = chart;
            chartView1.Chart = chart1;
            chartView2.Chart = chart2;
        }
    }
}
