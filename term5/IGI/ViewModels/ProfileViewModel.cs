using System.Collections;
using System.Collections.Generic;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class ProfileViewModel
    {
        public User User { get; set; }
        
        public ICollection<Tweet> UserTweets { get; set; }
        
        public bool IsCurrentUserFollowing { get; set; }
        
        public List<Like> Likes { get; set; }
        
        public List<Retweet> Retweets { get; set; }
        
        public List<Reply> Replies { get; set; }

        public List<User> Following { get; set; }
        
        public List<User> Followers { get; set; }
    }
}