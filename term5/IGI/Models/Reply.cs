using System;
using System.ComponentModel.DataAnnotations;
using Newtonsoft.Json;

namespace Twitter.Models
{
    public class Reply
    {
        public int Id { get; set; }

        public string Content { get; set; }

        [DataType(DataType.DateTime)]
        [DisplayFormat(DataFormatString = "{0:dd.MM.yyyy, H:mm}", ApplyFormatInEditMode = true)]
        public DateTime Date { get; set; }

        public User User { get; set; }

        public Tweet OnTweet { get; set; }
    }
}