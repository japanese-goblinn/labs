using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Twitter.Models;
using Twitter.ViewModels;

namespace Twitter.Controllers
{
    [Authorize]
    public class MessagesController: Controller
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;

        public MessagesController(UserManager<User> userManager, TwitterDBContext context)
        {
            _userManager = userManager;
            _context = context;
        }
        
        public async Task<IActionResult> ChatMessages(string id)
        {
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var userChattingWith = await _userManager
                .FindByIdAsync(id);
            if (user is null)
            {
                return NotFound();
            }
            var messages = await _context.Messages
                .Where(m =>
                    m.SendedTo.Id == userChattingWith.Id && m.SendedBy.Id == user.Id ||
                    m.SendedTo.Id == user.Id && m.SendedBy.Id == userChattingWith.Id)
                .OrderBy(m => m.DateTime)
                .ToListAsync() ?? new List<Message>();
            return View(new MessagesViewModel
            {
                Messages = messages,
                ChatWith = userChattingWith
            });
        }
        
        public async Task<IActionResult> Index()
        {
            var user = await _userManager
                .FindByNameAsync(User.Identity.Name);
            var chatsWithUser = _context.Messages
                .Where(m => m.SendedTo.Id == user.Id)
                .Include(m => m.SendedBy)
                .Select(m => m.SendedBy);
            var userChats = await _context.Messages
                .Where(m => m.SendedBy.Id == user.Id)
                .Include(m => m.SendedTo)
                .Select(m => m.SendedTo)
                .Union(chatsWithUser)
                .ToListAsync();
            return View(userChats);
        }
    }
}