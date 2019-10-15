using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using System.Linq;
using System;

namespace Twitter.Models
{
    public class SeedData
    {
        public static void Initilize(IServiceProvider serviceProvider) 
        {
            using (
                var context = new TwitterDBContext(
                    serviceProvider
                    .GetRequiredService<DbContextOptions<TwitterDBContext>>()
                )
            )
            {
                if (context.Tweets.Any())
                {
                    return;
                }
                else
                {
                    context.Tweets.AddRange(
                        new Tweet
                        {
                            Content = "Welcome to Twitter",
                            Date = DateTime.Now,
                            User = new User
                            {
                                Name = "Kirill",
                                Username = "japanese_goblinn"
                            }
              
                        }
                    );
                    context.SaveChanges();
                }
                
            }
        }
    }
}
