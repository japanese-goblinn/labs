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
                var currentUser = await _userManager
                    .FindByNameAsync(User.Identity.Name);
                var myTweets = _context.Tweets
                    .Where(t => t.Author.Id == currentUser.Id)
                    .Include(t => t.Author);
                var feed = _context.Subscriptions
                    .Where(x => x.User.Id == currentUser.Id)
                    .Select(u => u.SubscribedOnUser)
                    .SelectMany(t => t.Tweets)
                    .Include(t => t.Author);
                var feedConcat = await feed
                    .Concat(myTweets)
                    .OrderByDescending(t => t.Date)
                    .ToListAsync();
                return View(feedConcat);
            }
            else
            {
                return RedirectToAction("Login", "Account");
            }
        }

        public async Task<IActionResult> Comments(int id)
        {
            var tweet = await _context.Tweets
                .Where(t => t.Id == id)
                .Include(t => t.Author)
                .FirstAsync();
            return View(tweet);
        }

        public async Task<IActionResult> Retweet(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (tweet is null || user is null)
            {
                return NotFound();
            }
            await _context.Retweets.AddAsync(new Retweet
            {
                Tweet = tweet,
                RetweetedBy = user
            });
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Home");
        }

        public async Task<IActionResult> Like(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (tweet is null || user is null)
            {
                return NotFound();
            }
            await _context.Likes.AddAsync(new Like
            {
                WhoLiked = user,
                LikedTweet = tweet
            });
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Home");
        }
        
        public async Task<IActionResult> DeleteRetweet(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (tweet is null || user is null)
            {
                return NotFound();
            }
            var retweet = _context.Retweets
                .First(r =>
                    r.Tweet.Id == tweet.Id && r.RetweetedBy.Id == user.Id);
            _context.Retweets.Remove(retweet);
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Home");
        }
        
        public async Task<IActionResult> DeleteLike(int id)
        {
            var tweet = await _context.Tweets
                .FindAsync(id);
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (tweet is null || user is null)
            {
                return NotFound(); 
            }
            var like = _context.Likes
                .First(l => 
                    l.LikedTweet.Id == tweet.Id && l.WhoLiked.Id == user.Id);
            _context.Likes.Remove(like);
            await _context.SaveChangesAsync();
            return RedirectToAction("Index", "Home");
        }

        public async Task<IActionResult> Tweet(string content)
        {
            if (content != null)
            {
                await _context.Tweets.AddAsync(new Tweet
                {
                    Content = content,
                    Author = await _userManager.FindByNameAsync(User.Identity.Name),
                    Date = DateTime.Now
                });
                await _context.SaveChangesAsync();
            }
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