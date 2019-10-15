using System.Collections.Generic;

namespace Twitter.Models
{
    public class User
    {   
        public int Id { get; set; }
        public string Name { get; set; }
        public string Username { get; set; }
        
        public ICollection<Tweet> Tweets { get; set; }

        public ICollection<Reply> Replies { get; set; }

        public override string ToString()
        {
            return Username;
        }

    }
}