using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Razor.TagHelpers;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.TagHelpers
{
    public class LikeTagHelper: TagHelper
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        
        public int TweetId { get; set; }
        
        public string UserName { get; set; }
        
        public string RedirectUrl { get; set; }
        
        public LikeTagHelper(TwitterDBContext context, UserManager<User> userManager)
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
            var likes = await _context.Likes
                .Where(l => l.LikedTweet.Id == tweet.Id)
                .ToListAsync();
            if (_context.Likes.Any(l =>
                l.LikedTweet.Id == tweet.Id && l.WhoLiked.Id == user.Id))
            {
                output.Attributes.SetAttribute("style", "color: rgb(224, 36, 94);");
                output.Attributes.SetAttribute(
                    "href", 
                    $"/Home/DeleteLike/{TweetId}/?redirectUrl={RedirectUrl}"
                );
            }
            else
            {
                output.Attributes.SetAttribute("style", "color: rgb(101, 119, 134);");
                output.Attributes.SetAttribute(
                    "href", 
                    $"/Home/Like/{TweetId}/?redirectUrl={RedirectUrl}"
                );
            }
            output.Content.SetContent($"💟 {likes.Count}");
        }
    }
}