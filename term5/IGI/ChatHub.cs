
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.SignalR;
using Twitter.Models;

namespace Twitter
{
    public class ChatHub: Hub
    {
        private readonly TwitterDBContext _context;
        private readonly UserManager<User> _userManager;
        private static readonly HashSet<string> OnlineUsers = new HashSet<string>();

        public ChatHub(UserManager<User> userManager, TwitterDBContext context)
        {
            _userManager = userManager;
            _context = context;
        }

        public async Task MarkOnline(string user)
        {
            if (OnlineUsers.Contains(user))
            {
                return;
            }
            OnlineUsers.Add(user);
            await IsOnline(user);
        }

        public async Task MarkOffline(string user)
        {
            OnlineUsers.Remove(user);
            await IsOnline(user);
        }

        public async Task IsOnline(string user)
        {
            if (OnlineUsers.Contains(user))
            {
                await Clients.All.SendAsync("IsOnline", user, true);
            }
            else
            {
                await Clients.All.SendAsync("IsOnline", user, false);
            }
        }

        public async Task Send(string sendedByUsername, string sendedToUsername, string message)
        {
            var sendByUser = await _userManager.FindByNameAsync(sendedByUsername);
            var sendToUser = await _userManager.FindByNameAsync(sendedToUsername);
            var messageObj = new Message()
            {
                SendedBy = sendByUser,
                SendedTo = sendToUser,
                DateTime = DateTime.Now,
                Text = message
            };
            await _context.Messages.AddAsync(messageObj);
            await _context.SaveChangesAsync();
            await Clients
                .Users(sendByUser.Id, sendToUser.Id)
                .SendAsync("Send", sendByUser.Name, sendByUser.UserName,
                    sendToUser.UserName, message);
        }
    }
}