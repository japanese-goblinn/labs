using System.ComponentModel.DataAnnotations;

namespace Twitter.ViewModels
{
    public class ChangePasswordViewModel
    {
        public string Id { get; set; }
        
        [Required, Display(Name="Username")]
        public string UserName { get; set; }
        
        [Required, Display(Name="NewPassword")]
        public string NewPassword { get; set; }
    }
}