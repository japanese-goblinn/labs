using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Razor.TagHelpers;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.TagHelpers
{
    public class RepliesTagHelper: TagHelper
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        
        public int TweetId { get; set; }
        
        public RepliesTagHelper(TwitterDBContext context, UserManager<User> userManager)
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
            var replies = await _context.Replies
                .Where(r => r.OnTweet == tweet)
                .ToListAsync();

            if (replies.Count != 0)
            {
                output.Attributes.SetAttribute("style", "color: rgb(29, 161, 242);");
            }
            else
            {
                output.Attributes.SetAttribute("style", "color: rgb(101, 119, 134)");
            }
            output.Attributes.SetAttribute("href", $"/Home/Replies/{TweetId}");
            output.Content.SetContent($"💬 {replies.Count}");
        }
    }
}