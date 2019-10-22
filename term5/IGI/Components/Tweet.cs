using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.Components
{
    public class Tweet: ViewComponent
    {
        private readonly TwitterDBContext _context;
        
        public Tweet(TwitterDBContext context)
        {
            _context = context;
        }
        
        public async Task<IViewComponentResult> InvokeAsync(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            return View(tweet);
        }
    }
}