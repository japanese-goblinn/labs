using System;
using System.ComponentModel.DataAnnotations;

namespace Twitter.Models
{
    public class Message
    {
        public int Id { get; set; }
        
        public string Text { get; set; }
        
        public User SendedTo { get; set; }
        
        public User SendedBy { get; set; }
        
        [DataType(DataType.DateTime)]
        [DisplayFormat(DataFormatString = "{0:dd.MM.yyyy, H:mm}", ApplyFormatInEditMode = true)]
        public DateTime DateTime { get; set; }
    }
} 