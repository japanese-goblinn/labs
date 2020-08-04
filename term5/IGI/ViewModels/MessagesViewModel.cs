using System.Collections.Generic;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class MessagesViewModel
    {
        public User ChatWith { get; set; }
        
        public IEnumerable<Message> Messages { get; set; }
    }
}