using System.Collections.Generic;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class RepliesViewModel
    {
        public Tweet Tweet { get; set; }
        
        public bool isCurrentUserLiked { get; set; }
        
        public bool isCurrentUserRetweeted { get; set; }
    }
}