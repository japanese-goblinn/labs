using System;
namespace Twitter.Models
{
    public class TagsToTweets
    {
        public int Id { get; set; }

        public Tag Tag { get; set; }

        public Tweet OnTweet { get; set; }
    }
}
