using Microsoft.AspNetCore.Mvc;
using System.Linq;
using System.Collections;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;

namespace Twitter.Controllers
{
    public class UsersController : Controller
    {
        private readonly TwitterDBContext _context;

        public UsersController(TwitterDBContext context)
        {
            _context = context;
        }

        // GET
        public async Task<IActionResult> Index()
        {
            return View(await _context.Users.ToListAsync());
            
        }
    }
}