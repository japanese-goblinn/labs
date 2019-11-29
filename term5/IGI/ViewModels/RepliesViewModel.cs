using System.Collections.Generic;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class RepliesViewModel
    {
        public Tweet Tweet { get; set; }
        
        public List<Reply> Replies { get; set; }
        
        public bool isCurrentUserLiked { get; set; }
        
        public bool isCurrentUserRetweeted { get; set; }
        
        public List<User> Likers { get; set; } 
        
        public List<User> Retweeters { get; set; }
    }
}