using Microsoft.EntityFrameworkCore;

namespace Twitter.Models
{
    public class TweetContext: DbContext
    {
        public TweetContext(DbContextOptions<TweetContext> options) : base(options)
        {
            
        }
        
        public DbSet<Tweet> Tweet { get; set; }
    }
}