using System.Collections;
using System.Collections.Generic;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class ProfileViewModel
    {
        public User User { get; set; }
        
        public ICollection<Tweet> UserTweets { get; set; }

        public int AmountOfFollowing { get; set; }
        
        public int AmountOfFollowers { get; set; }
    }
}