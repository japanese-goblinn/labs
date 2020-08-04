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
    [Authorize(Roles = "moderator")]
    public class TweetsController: Controller
    {
        private readonly UserManager<User> _userManager;
        private readonly TwitterDBContext _context;

        public TweetsController(UserManager<User> userManager, TwitterDBContext context)
        {
            _context = context;
            _userManager = userManager;
        }
        
        public async Task<IActionResult> Edit(int id)
        {
            var tweet = await _context.Tweets
                .Where(t => t.Id == id)
                .Include(t => t.Author)
                .FirstAsync();
            if (tweet == null)
            {
                return NotFound();
            }
            return View(new EditTweetViewModel
            {
                Id = tweet.Id,
                Content = tweet.Content
            });
        }
        
        [HttpPost]
        public async Task<IActionResult> Edit(EditTweetViewModel model)
        {
            var tweet = await _context.Tweets
                    .Where(t => t.Id == model.Id)
                    .Include(t => t.Author)
                    .FirstAsync();
            if (tweet != null && model.Content != null)
            {
                tweet.Content = model.Content;
                _context.Tweets.Update(tweet);
                await _context.SaveChangesAsync();
                return RedirectToAction("Index");
            }
            return View(model);
        }
            
        public async Task<IActionResult> Delete(int id)
        {
            var tweet = await _context.Tweets
                .Where(t => t.Id == id)
                .Include(t => t.Author)
                .FirstAsync();
            if (tweet == null)
            {
                return NotFound();
            }
            _context.Tweets.Remove(tweet);
            await _context.SaveChangesAsync();
            return RedirectToAction("Index");
        }
        
        public async Task<IActionResult> Index()
        {
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var tweets = await _context.Tweets
                .Include(t => t.Author)
                .ToListAsync();
            return View(tweets);
        }
    }
}