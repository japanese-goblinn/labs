using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Twitter.Models
{
    public class Tweet
    {
        public int Id { get; set; }
        public string Content { get; set; }
        public DateTime Date { get; set; }

        [NotMapped]
        public User Author { get; set; }
        
        [NotMapped]
        public HashSet<User> Likes { get; set; }
        
        [NotMapped]
        public HashSet<User> Retweets { get; set; }
        
        [NotMapped]
        public HashSet<Tag> Tags { get; set; }
        
    }
}