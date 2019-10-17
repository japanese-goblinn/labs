using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using System.Linq;
using System;
using Microsoft.AspNetCore.Identity;

namespace Twitter.Models
{
    public class SeedData
    {
        public static async void Initilize(IServiceProvider serviceProvider) 
        {
            using (var context = serviceProvider.GetRequiredService<UserManager<User>>())
            {
                if (!context.Users.Any())
                {
                    using (var roleContext = serviceProvider.GetRequiredService<RoleManager<IdentityRole>>())
                    {
                       await roleContext.CreateAsync(new IdentityRole("admin"));
                       await roleContext.CreateAsync(new IdentityRole("user"));
                       await roleContext.CreateAsync(new IdentityRole("moderator"));
                    }
                    
                    var user = new User
                    {
                        UserName = "japanese_goblin",
                        Name = "Kirill",
                        Email = "cool45akol@gmail.com"
                    };
                    var moder = new User
                    {
                        UserName = "moderator",
                        Name = "CoolHacker",
                        Email = "moderator@twitter.com"
                    };
                    var admin = new User
                    {
                        UserName = "admin",
                        Name = "PapaLinux",
                        Email = "admin@twitter.com"
                    };
                    var userResult = await context.CreateAsync(user, "Test_1234");
                    var moderatorResult = await context.CreateAsync(moder, "Moderator_1234");
                    var adminResult = await context.CreateAsync(admin, "Admin_1234");
                    if (userResult.Succeeded && moderatorResult.Succeeded && adminResult.Succeeded)
                    {
                        await context.AddToRoleAsync(user, "user");
                        await context.AddToRoleAsync(moder, "moderator");
                        await context.AddToRoleAsync(admin, "admin");
                    }
                }
                
            }
            using (var context = new TwitterDBContext(serviceProvider
                    .GetRequiredService<DbContextOptions<TwitterDBContext>>()))
            {
                if (!context.Tweets.Any())
                {
                    var tag = new Tag
                    {
                        Text = "Hot"
                    };
                    var tweet = new Tweet
                    {
                        Content = "Welcome to Twitter",
                        Date = DateTime.Now,
                        Author = context.Users.First(),
                    };
                    context.Tweets.AddRange(tweet);
                    context.Tags.Add(tag);
                    context.TagsToTweets.Add(new TagsToTweets
                    {
                        Tag = tag,
                        OnTweet = tweet
                    });
                    context.SaveChanges();
                }
            }
            
        }
    }
}
