using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Razor.TagHelpers;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.TagHelpers
{
    public class RetweetTagHelper: TagHelper
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        
        public int TweetId { get; set; }
        
        public string UserName { get; set; }
        
        public string RedirectUrl { get; set; }
        
        public RetweetTagHelper(TwitterDBContext context, UserManager<User> userManager)
        {
            _context = context;
            _userManager = userManager;
        }
        
        public override async Task ProcessAsync(TagHelperContext context, TagHelperOutput output)
        {
            base.Process(context, output);
            output.TagName = "a";
            output.TagMode = TagMode.StartTagAndEndTag;
            
            var tweet = await _context.Tweets
                .FindAsync(TweetId);
            var user = await _userManager
                .FindByNameAsync(UserName);
            var retweets = await _context.Retweets
                .Where(r => r.Tweet.Id == tweet.Id)
                .ToListAsync();
            if (_context.Retweets.Any(r =>
                r.Tweet.Id == tweet.Id && r.RetweetedBy.Id == user.Id))
            {
                output.Attributes.SetAttribute("style", "color: rgb(23, 191, 99); font-weight: bolt;");
                output.Attributes.SetAttribute(
                    "href", 
                    $"/Home/DeleteRetweet/{TweetId}/?redirectUrl={RedirectUrl}"
                );
            }
            else
            {
                output.Attributes.SetAttribute("style", "color: rgb(101, 119, 134);");
                output.Attributes.SetAttribute(
                    "href", 
                    $"/Home/Retweet/{TweetId}/?redirectUrl={RedirectUrl}"
                );
            }
            output.Content.SetContent($"🔃 {retweets.Count}");
        }
    }
}