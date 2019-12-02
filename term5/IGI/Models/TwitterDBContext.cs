using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;

namespace Twitter.Models
{
    public class TwitterDBContext: IdentityDbContext<User>
    {
        public DbSet<Tweet> Tweets { get; set; }
        
        public DbSet<Reply> Replies { get; set; }

        public DbSet<Retweet> Retweets { get; set; }

        public DbSet<Subscription> Subscriptions { get; set; }
        
        public DbSet<Message> Messages { get; set; }
        
        public DbSet<Like> Likes { get; set; }

        public TwitterDBContext(DbContextOptions<TwitterDBContext> options) : base(options)
        {
            Database.Migrate();
        }
    }
}