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

        public async Task<IActionResult> Follow(string userName)
        {
            var currentUser = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var user = await _userManager
                .FindByNameAsync(userName);
            if (currentUser is null || user is null)
            {
                return NotFound(); 
            }
            await _context.Subscriptions.AddAsync(new Subscription
            {
                User = currentUser,
                SubscribedOnUser = user
            });
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Profile", new
            {
                userName = user.UserName
            });
        }

        public async Task<IActionResult> Unfollow(string userName)
        {
            var currentUser = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var user = await _userManager
                .FindByNameAsync(userName);
            if (currentUser is null || user is null)
            {
               return NotFound(); 
            }
            var subscription = await _context.Subscriptions
                .FirstAsync(s => s.User == currentUser && s.SubscribedOnUser == user);
            if (subscription is null)
            {
                return NotFound();
            }
            _context.Subscriptions.Remove(subscription);
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Profile", new
            {
                userName = user.UserName
            });
        }

        public async Task<IActionResult> WriteMessage(string userName)
        {
            var user = await _userManager
                .FindByNameAsync(userName);
            if (user is null)
            {
                return NotFound();
            }
            return RedirectToAction("ChatMessages", "Messages", new
            {
                id = user.Id
            });
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
                .Select(u => u.SubscribedOnUser)
                .ToListAsync();
        }

        public async Task<List<Retweet>> Retweets(User user)
        {
            return await _context.Retweets
                .Where(r => r.RetweetedBy == user)
                .Include(r => r.Tweet.Author)
                .ToListAsync();
        }

        public async Task<List<Like>> Likes(User user)
        {
            return await _context.Likes
                .Where(l => l.WhoLiked == user)
                .Include(l => l.LikedTweet.Author)
                .ToListAsync();
        }

        public async Task<List<Reply>> Replies(User user)
        {
            return await _context.Replies
                .Where(r => r.User == user)
                .Include(r => r.OnTweet.Author)
                .ToListAsync();
        }
        
        public async Task<IActionResult> Index(string userName)
        {
            var currentUser = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var user = await _userManager
                .FindByNameAsync(userName);
            if (user is null || currentUser is null)
            {
                return NotFound();
            }
            var isFollowing = await _context.Subscriptions
                .Where(s => s.User == currentUser)
                .AnyAsync(s => s.SubscribedOnUser == user);
            var userTweets = await _context.Tweets
                .Where(t => t.Author.Id == user.Id)
                .OrderByDescending(t => t.Date)
                .ToListAsync();
            var followers = await Followers(user);
            var following = await Following(user);
            var likes = await Likes(user);
            var retweets = await Retweets(user);
            var replies = await Replies(user);
            return View(new ProfileViewModel
            {
                User = user,
                UserTweets = userTweets,
                IsCurrentUserFollowing = isFollowing,
                Followers = followers,
                Following = following,
                Likes = likes,
                Retweets = retweets,
                Replies = replies
            });
        }
    }
}