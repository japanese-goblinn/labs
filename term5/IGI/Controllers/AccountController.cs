using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Localization;
using Twitter.Models;
using Twitter.ViewModels;

namespace Twitter.Controllers
{
    public class AccountController : Controller
    {
        private readonly UserManager<User> _userManager;
        private readonly SignInManager<User> _signInManager;
        private readonly IStringLocalizer<AccountController> _localizer;

        public AccountController(
            UserManager<User> userManager,
            SignInManager<User> signInManager,
            IStringLocalizer<AccountController> localizer
        )
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _localizer = localizer;
        }

        public IActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> Register(RegisterViewModel model)
        {
            var isUserAlreadyExist = _userManager.Users.Any(x => x.UserName == model.UserName);
            if (isUserAlreadyExist)
            {
                ModelState.AddModelError("Username", _localizer["UserAlreadyExists"]);
            }
            if (ModelState.IsValid)
            {
                User user = new User
                {
                    Email = model.Email,
                    Name = model.Name,
                    UserName = model.UserName,
                };
                var result = await _userManager.CreateAsync(user, model.Password);
                if (result.Succeeded)
                {
                    await _signInManager.SignInAsync(user, false);
                    return RedirectToAction("Index", "Home");
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

        public IActionResult Login(string returnUrl = null)
        {
            return View(new LoginViewModel
            {
                ReturnUrl = returnUrl
            });
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Login(LoginViewModel model)
        {
            if (ModelState.IsValid)
            {
                var result =
                    await _signInManager.PasswordSignInAsync(model.UserName , model.Password, model.RememberMe, false);
                if (result.Succeeded)
                {
                    if (!string.IsNullOrEmpty(model.ReturnUrl) && Url.IsLocalUrl(model.ReturnUrl))
                    {
                        return Redirect(model.ReturnUrl);
                    }
                    else
                    {
                        return RedirectToAction("Index", "Home");
                    }
                }
                else
                {
                    ModelState.AddModelError("", _localizer["LoginError"]);
                }
            }

            return View(model);
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> LogOff()
        { 
            await _signInManager.SignOutAsync();
            return RedirectToAction("Index", "Home");
        }
    }
}