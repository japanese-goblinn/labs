using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
namespace GameStore
{
    public class Game
    {
        public string Name { get; set; }
        public string Developer { get; set; }
        public string Description { get; set; }

        public string PosterImage { get; set; }
        public string HeadImage { get; set; }
        public string ResourcePosterImage { get; set; }

        public int Metascore { get; set; }
        public double UserScore { get; set; }

        public string AgeLimit { get; set; }
        public int Year { get; set; }

        public bool IsAlreadyReleased { get; set; }
        public bool IsAlreadyAddedToLibrary { get; set; }

        public string[] ganers;
        public string[] platforms;

        public string MetacriticUrl { get; set; }
        public string CriticReviewUrl { get; set; }
        public string UserReviewUrl { get; set; }
    }
}