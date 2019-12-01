using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Newtonsoft.Json;
using Twitter.Models;
using Twitter.ViewModels;

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
                return View(await GetFeed());
            }
            else
            {
                return RedirectToAction("Login", "Account");
            }
        }

        private async Task<List<Tweet>> GetFeed()
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
            return feedConcat;
        }

        [Route("/home/json/{id?}/{amount?}/{page?}")]
        public async Task<JsonResult> GetJsonReplies(int id, int amount, int page)
        {
            var tweet = await _context.Tweets
                .Where(t => t.Id == id)
                .Include(t => t.Author)
                .FirstAsync();
            var replies = await _context.Replies
                .Where(r => r.OnTweet == tweet)
                .Include(r => r.User)
                .OrderByDescending(r => r.Date)
                .Skip((page - 1) * amount)
                .Take(amount)
                .ToListAsync();
            return Json(JsonConvert.SerializeObject(replies.Select(r => new
            {
                r.Content,
                r.Date,
                TweetAuthorUsername = r.OnTweet.Author.UserName,
                AuthorName = r.User.Name,
                AuthorUsername = r.User.UserName
            })));
        }
        
        public async Task<IActionResult> Replies(int id)
        {
            var tweet = await _context.Tweets
                .Where(t => t.Id == id)
                .Include(t => t.Author)
                .FirstAsync();
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            if (tweet is null || user is null)
            {
                return NotFound();
            }
            var likers = await _context.Likes
                .Where(l => l.LikedTweet.Id == id)
                .Select(l => l.WhoLiked)
                .ToListAsync();
            var retweeters = await _context.Retweets
                .Where(r => r.Tweet.Id == id)
                .Select(r => r.RetweetedBy)
                .ToListAsync();
            var isUserLiked = await _context.Likes
                .Where(l => l.LikedTweet == tweet)
                .AnyAsync(t => t.WhoLiked == user);
            var isUserRetweeted = await _context.Retweets
                .Where(r => r.Tweet == tweet)
                .AnyAsync(r => r.RetweetedBy == user);
            return View(new RepliesViewModel
            {
                Tweet = tweet,
                Likers = likers,
                Retweeters = retweeters,
                isCurrentUserLiked = isUserLiked,
                isCurrentUserRetweeted = isUserRetweeted
            });
        }

        public async Task<IActionResult> Reply(string content, int? tweetId)
        {
            if (content is null || tweetId is null)
            {
                return NotFound();
            }
            var tweet = await _context.Tweets
                .FindAsync(tweetId);
            await _context.Replies.AddAsync(new Reply
            {
                Content = content,
                OnTweet = tweet,
                User = await _userManager.FindByNameAsync(User.Identity.Name),
                Date = DateTime.Now
            });
            await _context.SaveChangesAsync();
            return RedirectToAction("Replies", "Home", new
            {
                id = tweet.Id
            });
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