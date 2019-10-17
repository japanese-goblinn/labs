using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;
using Twitter.ViewModels;

namespace Twitter.Controllers
{
    [Authorize]
    public class ProfileController: Controller
    {
        private readonly UserManager<User> _userManager;
        private readonly TwitterDBContext _context;

        public ProfileController(UserManager<User> userManager, TwitterDBContext context)
        {
            _userManager = userManager;
            _context = context;
        }

        public async Task<List<User>> Followers(User user)
        {
            return await _context.Subscriptions
                .Where(x => x.SubscribedOnUser.Id == user.Id)
                .Select(u => u.User)
                .ToListAsync();
        } 
        
        public async Task<List<User>> Following(User user)
        {
            return await _context.Subscriptions
                .Where(x => x.User.Id == user.Id)
                .Select(u => u.User)
                .ToListAsync();
        }
        
        public async Task<IActionResult> Index()
        {
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (user != null)
            {
                var userTweets = await _context.Tweets
                    .Select(t => t)
                    .Where(t => t.Author.Id == user.Id)
                    .OrderByDescending(t => t.Date)
                    .ToListAsync();
                var followers = await Followers(user);
                var following = await Following(user);
                var viewModel = new ProfileViewModel
                {
                    User = user,
                    UserTweets = userTweets,
                    AmountOfFollowers = followers.Count,
                    AmountOfFollowing = following.Count
                };
                return View(viewModel);
            }
            else
            {
                return NotFound();
            }
        }
    }
}