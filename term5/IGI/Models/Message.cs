namespace Twitter.Models
{
    public class Message
    {
        public string Text { get; set; }
        
        public User SendedTo { get; set; }
        
        public User SendedBy { get; set; }
    }
}