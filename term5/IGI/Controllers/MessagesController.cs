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
        
        [HttpPost]
        public async Task<IActionResult> SendMessage(string sendToUsername, string sendByUsername, string message)
        {
            var sendByUser = await _userManager.FindByNameAsync(sendByUsername);
            var sendToUser = await _userManager.FindByNameAsync(sendToUsername);
            if (sendByUser is null || sendToUser is null || message is null)
            {
                if (message is null && sendToUser != null)
                {
                    return RedirectToAction("ChatMessages", "Messages", new
                    {
                        id = sendToUser.Id
                    });
                }
                return NotFound();
            }
            var messageObj = new Message()
            {
                SendedBy = sendByUser,
                SendedTo = sendToUser,
                DateTime = DateTime.Now,
                Text = message
            };
            await _context.Messages.AddAsync(messageObj);
            await _context.SaveChangesAsync();
            return RedirectToAction("ChatMessages", "Messages", new
            {
                id = sendToUser.Id
            });
        }

        public async Task<IActionResult> ChatMessages(string id)
        {
            var user = await _userManager.FindByNameAsync(User.Identity.Name);
            var userChattingWith = await _userManager.FindByIdAsync(id);
            if (user == null)
            {
                return NotFound();
            }
            var messages = await _context.Messages
                .Where(m =>
                    m.SendedTo.Id == userChattingWith.Id && m.SendedBy.Id == user.Id ||
                    m.SendedTo.Id == user.Id && m.SendedBy.Id == userChattingWith.Id)
                .OrderBy(m => m.DateTime)
                .ToListAsync();
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