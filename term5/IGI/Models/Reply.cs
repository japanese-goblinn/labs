namespace Twitter.Models
{
    public class Reply: Tweet
    {
        public Tweet ToUser { get; set; }
    }
}