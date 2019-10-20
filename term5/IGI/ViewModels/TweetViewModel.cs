using System.ComponentModel.DataAnnotations;

namespace Twitter.ViewModels
{
    public class TweetViewModel
    {
        public int Id { get; set; }
        [Required]
        public string Content { get; set; }
    }
}