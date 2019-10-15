using System;
using System.ComponentModel.DataAnnotations;

namespace Twitter.Models
{
    public class Reply
    {
        public int Id { get; set; }

        public string Content { get; set; }

        [DataType(DataType.DateTime)]
        public DateTime Date { get; set; }

        public User User { get; set; }

        public Tweet OnTweet { get; set; }

        //TODO: Add Methods to get Tags, Likes, Retweets
    }
}