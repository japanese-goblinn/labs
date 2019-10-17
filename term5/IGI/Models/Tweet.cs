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
        
//        [DisplayFormat(DataFormatString = "{0:yyyy-MM-dd}")]
        [DataType(DataType.DateTime)]
        public DateTime Date { get; set; }

        [Required]
        public User Author { get; set; }

        //TODO:
        //Add Methods to get Tags, Likes, Retweets

    }
}