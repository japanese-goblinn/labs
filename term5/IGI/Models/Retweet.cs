using System;
namespace Twitter.Models
{
    public class Retweet
    {
        public int Id { get; set; }

        public User RetweetedBy { get; set; }

        public Tweet Tweet { get; set; }
    }
}
