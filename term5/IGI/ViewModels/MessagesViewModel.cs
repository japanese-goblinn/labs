using System.Collections;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.ViewModels
{
    public class MessagesViewModel
    {
        public ICollection<User> ChatsWith { get; set; }
    }
}