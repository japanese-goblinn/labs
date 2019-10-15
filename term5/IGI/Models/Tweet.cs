using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Twitter.Models
{
    public class Tweet
    {
        public int Id { get; set; }

        [Required]
        public string Content { get; set; }
        
        [DataType(DataType.DateTime)]
        public DateTime Date { get; set; }

        [Required]
        public User User { get; set; }

        //TODO:
        //Add Methods to get Tags, Likes, Retweets

    }
}