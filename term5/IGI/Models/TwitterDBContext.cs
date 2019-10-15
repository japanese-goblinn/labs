using Microsoft.EntityFrameworkCore;

namespace Twitter.Models
{
    public class TwitterDBContext: DbContext
    {
        public DbSet<Tweet> Tweets { get; set; }
        
        public DbSet<Reply> Replies { get; set; }

        public DbSet<Retweets> Retweets { get; set; }

        public DbSet<Subscriptions> Subscriptions { get; set; }

        public DbSet<Tag> Tags { get; set; }

        public DbSet<TagsToTweets> TagsToTweets { get; set; }

        public DbSet<Message> Messages { get; set; }
        public DbSet<User> Users { get; set; }

        public DbSet<Likes> Likes { get; set; }
        public TwitterDBContext(DbContextOptions<TwitterDBContext> options): base(options) {}
    }
}