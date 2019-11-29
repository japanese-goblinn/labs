using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.Components
{
    public class Reply: ViewComponent
    {
        private readonly TwitterDBContext _context;

        public Reply(TwitterDBContext context)
        {
            _context = context;
        }

        public async Task<IViewComponentResult> InvokeAsync(int id)
        {
            var reply = await _context.Replies
                .FindAsync(id);
            return View(reply);
        }
    }
}