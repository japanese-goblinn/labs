using System;
namespace Twitter.Models
{
    public class Subscriptions
    {
        public int Id { get; set; }

        public int UserId { get; set; }

        public int SubscribedOnUserId { get; set; }
    }
}
