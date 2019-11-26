namespace Twitter.Models
{
    public class Like
    {
        public int Id { get; set; }

        public User WhoLiked { get; set; }

        public Tweet LikedTweet { get; set; }
    }
}
