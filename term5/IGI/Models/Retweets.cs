using System;
namespace Twitter.Models
{
    public class Retweets
    {
        public int Id { get; set; }

        public User RetweetedBy { get; set; }

        public Tweet Tweet { get; set; }
    }
}
