using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.Controllers
{
    [Authorize]
    public class HomeController : Controller
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        public HomeController(TwitterDBContext context, UserManager<User> userManager)
        {
            _context = context;
            _userManager = userManager;
        }
        public async Task<IActionResult> Index()
        {
            if (User.Identity.IsAuthenticated)
            {
                var userTweets = await _context.Tweets
                    .Include(t => t.Author)
                    .ToListAsync();
                return View(userTweets);
            }
            else
            {
                return RedirectToAction("Login", "Account");
            }
        }

        public async Task<IActionResult> Tweet()
        {
            var newTweet = new Tweet
            {
                Content = "New tweet!",
                Author = await _userManager.FindByNameAsync(User.Identity.Name),
                Date = DateTime.Now
            };
            await _context.Tweets.AddAsync(newTweet);
            await _context.SaveChangesAsync();
            return RedirectToAction("Index");
        }
        
        public string Privacy()
        {
            return "Privacy";
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel {RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier});
        }
    }
}