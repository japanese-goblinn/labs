using System.Collections.Generic;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Newtonsoft.Json;

namespace Twitter.Models
{
    public class User: IdentityUser
    { 
        public string Name { get; set; }
        
        public ICollection<Tweet> Tweets { get; set; }

        public ICollection<Reply> Replies { get; set; }

        public override string ToString()
        {
            return UserName;
        }

    }
}