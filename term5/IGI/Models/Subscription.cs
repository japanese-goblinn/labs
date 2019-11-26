using System;
namespace Twitter.Models
{
    public class Subscription
    {
        public int Id { get; set; }

        public User User { get; set; }

        public User SubscribedOnUser { get; set; }
    }
}
