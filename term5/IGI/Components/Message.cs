using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Remotion.Linq.Parsing.ExpressionVisitors.Transformation.PredefinedTransformations;
using Twitter.Models;

namespace Twitter.Components
{
    public class Message: ViewComponent
    {
        private readonly TwitterDBContext _context;
        
        public Message(TwitterDBContext context)
        {
            _context = context;
        }
        
        public async Task<IViewComponentResult> InvokeAsync(int id)
        {
            var message = await _context.Messages
                .FindAsync(id);
            return View(message);
        }
    }
}