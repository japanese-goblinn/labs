using System;
namespace Twitter.Models
{
    public class TagsToTweets
    {
        public int Id { get; set; }

        public int TagId { get; set; }

        public int TweetId { get; set; }
    }
}
