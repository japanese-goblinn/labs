using System;
namespace Twitter.Models
{
    public class Subscriptions
    {
        public int Id { get; set; }

        public User User { get; set; }

        public User SubscribedOnUserId { get; set; }
    }
}
