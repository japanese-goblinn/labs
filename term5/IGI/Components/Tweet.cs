using System.Linq;
using System.Reflection.Metadata;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;
using Twitter.ViewModels;

namespace Twitter.Components
{
    public class Tweet: ViewComponent
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        
        public Tweet(TwitterDBContext context, UserManager<User> userManager)
        {
            _context = context;
            _userManager = userManager;
        }
        
        public async Task<IViewComponentResult> InvokeAsync(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            return View(tweet);
        }
    }
}