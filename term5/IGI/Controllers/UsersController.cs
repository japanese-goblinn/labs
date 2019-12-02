using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Localization;
using Twitter.Models;
using Twitter.ViewModels;

namespace Twitter.Controllers
{
    [Authorize(Roles = "admin")]
    public class UsersController: Controller
    {
        private readonly UserManager<User> _userManager;
        private readonly IStringLocalizer<UsersController> _localizer;

        public UsersController(UserManager<User> userManager, IStringLocalizer<UsersController> localizer)
        {
            _userManager = userManager;
            _localizer = localizer;
        }
 
        public async Task<IActionResult> Index() => View(await _userManager.Users.ToListAsync());
        
        
        public IActionResult Create() => View();
 
        [HttpPost]
        public async Task<IActionResult> Create(CreateUserViewModel model)
        {
        
            if (ModelState.IsValid)
            {
                var user = new User
                {
                    Email = model.Email,
                    Name = model.Name,
                    UserName = model.UserName
                };
                var result = await _userManager.CreateAsync(user, model.Password);
                if (result.Succeeded)
                {
                    return RedirectToAction("Index");
                }
                else
                {
                    foreach (var error in result.Errors)
                    {
                        ModelState.AddModelError(string.Empty, error.Description);
                    }
                }
            }
            return View(model);
        }
 
        public async Task<IActionResult> Edit(string id)
        {
            User user = await _userManager.FindByIdAsync(id);
            if (user == null)
            {
                return NotFound();
            }
            return View(new EditUserViewModel
            {
                Id = user.Id,
                Name = user.Name,
                Email = user.Email,
                UserName = user.UserName
            });
        }
        
        [HttpPost]
        public async Task<IActionResult> Edit(EditUserViewModel model)
        {
            if (ModelState.IsValid)
            {
                var user = await _userManager.FindByIdAsync(model.Id);
                if (user != null)
                {
                    user.Name = model.Name;
                    user.Email = model.Email;
                    user.UserName = model.UserName;
                    var result = await _userManager.UpdateAsync(user);
                    if (result.Succeeded)
                    {
                        return RedirectToAction("Index");
                    }
                    else
                    {
                        foreach (var error in result.Errors)
                        {
                            ModelState.AddModelError(string.Empty, error.Description);
                        }
                    }
                }
            }
            return View(model);
        }
        
        public async Task<ActionResult> Delete(string id)
        {
            var user = await _userManager.FindByIdAsync(id);
            if (user != null && user.UserName != User.Identity.Name)
            {
                var result = await _userManager.DeleteAsync(user);
                if (!result.Succeeded)
                {
                    return NotFound();
                }
            }
            return RedirectToAction("Index");
        }
        
        public async Task<IActionResult> ChangePassword(string id)
        {
            var user = await _userManager.FindByIdAsync(id);
            if (user == null)
            {
                return NotFound();
            }
            return View(new ChangePasswordViewModel
            {
                Id = user.Id,
                UserName = user.UserName
            });
        }
 
        [HttpPost]
        public async Task<IActionResult> ChangePassword(ChangePasswordViewModel model)
        {
            if (ModelState.IsValid)
            {
                var user = await _userManager.FindByIdAsync(model.Id);
                if (user != null)
                {
                    var passwordValidator = HttpContext.RequestServices
                        .GetService(typeof(IPasswordValidator<User>)) as IPasswordValidator<User>;
                    var passwordHasher =
                        HttpContext.RequestServices
                            .GetService(typeof(IPasswordHasher<User>)) as IPasswordHasher<User>;
                    if (model.NewPassword == null)
                    {
                        ModelState.AddModelError(string.Empty, _localizer["EmptyPasswordError"]);
                        return View(model);
                    }
                    var result = await passwordValidator.ValidateAsync(_userManager, user, model.NewPassword);
                    if (result.Succeeded)
                    {
                            user.PasswordHash = passwordHasher.HashPassword(user, model.NewPassword);
                            await _userManager.UpdateAsync(user);
                            return RedirectToAction("Index");
                    }
                    else
                    {
                        foreach (var error in result.Errors)
                        {
                            ModelState.AddModelError(string.Empty, error.Description);
                        }
                    }
                }
                else
                {
                    ModelState.AddModelError(string.Empty, _localizer["NotFoundError"]);
                }
            }
            return View(model);
        }
    }
}