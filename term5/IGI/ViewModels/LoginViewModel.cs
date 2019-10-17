using System.ComponentModel.DataAnnotations;

namespace Twitter.ViewModels
{
    public class LoginViewModel
    {
        [Required, Display(Name = "Username")]
        public string UserName { get; set; }
         
        [Required, DataType(DataType.Password), Display(Name = "Password")]
        public string Password { get; set; }
         
        [Display(Name = "Remember Me?")]
        public bool RememberMe { get; set; }
         
        public string ReturnUrl { get; set; }
    }
}