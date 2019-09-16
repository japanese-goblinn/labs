using Microsoft.AspNetCore.Mvc;
using Twitter.Models;

namespace Twitter.Controllers
{
    public class UsersController : Controller
    {
        // GET
        public string Index()
        {
            var user = new User()
            {
                Username = "Lel"
            };
            return $"{user} Controller";
        }
    }
}