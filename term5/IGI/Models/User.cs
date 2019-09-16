using System.Collections.Generic;

namespace Twitter.Models
{
    public class User
    {   
        public int Id { get; set; }
        
        public string Name { get; set; }
        
        public string Username { get; set; }
        
        public List<Tweet> Tweets { get; set; }
        
        public List<Reply> Replies { get; set; }
        
        public HashSet<Tweet> Likes { get; set; }


        public override string ToString()
        {
            return Username;
        } 
    }
}