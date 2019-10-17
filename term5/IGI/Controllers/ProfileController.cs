using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.Controllers
{
    [Authorize]
    public class ProfileController: Controller
    {
        private readonly TwitterDBContext _context;

        public ProfileController(TwitterDBContext context)
        {
            _context = context;
        }
        
        public IActionResult Index()
        {
            return View();
        }
    }
}