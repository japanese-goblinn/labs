using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.Controllers
{
    [Authorize]
    public class MessagesController: Controller
    {
        private readonly TwitterDBContext _context;

        public MessagesController(TwitterDBContext context)
        {
            _context = context;
        }
        
        public IActionResult Index()
        {
            return View();
        }
    }
}