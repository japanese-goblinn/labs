using Microsoft.EntityFrameworkCore;

namespace Twitter.Models
{
    public class ReplyContext: DbContext
    {
        public ReplyContext(DbContextOptions<ReplyContext> options) : base(options)
        {
            
        }
        
        public DbSet<Reply> Replies { get; set; }
    }
}