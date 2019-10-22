using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.Components
{
    public class Message: ViewComponent
    {
        private readonly TwitterDBContext _context;
        
        public Message(TwitterDBContext context)
        {
            _context = context;
        }
        
        public async Task<IViewComponentResult> InvokeAsync(int id)
        {
            var tweet = await _context.Messages
                .FindAsync(id);
            return View(tweet);
        }
    }
}