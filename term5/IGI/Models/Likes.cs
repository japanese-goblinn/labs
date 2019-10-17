namespace Twitter.Models
{
    public class Likes
    {
        public int Id { get; set; }

        public User WhoLiked { get; set; }

        public Tweet LikedTweet { get; set; }
    }
}
