using System.ComponentModel.DataAnnotations;

namespace Twitter.ViewModels
{
    public class EditUserViewModel
    {
        public string Id { get; set; }
        
        [Required, Display(Name="Name")]
        public string Name { get; set; }
        
        [Required, Display(Name="Username")]
        public string UserName { get; set; }
        
        [RegularExpression
            (
                @"^(?("")("".+?(?<!\\)""@)|(([0-9a-z]((\.(?!\.))|[-!#\$%&'\*\+/=\?\^`\{\}\|~\w])*)(?<=[0-9a-z])@))" +
                @"(?(\[)(\[(\d{1,3}\.){3}\d{1,3}\])|(([0-9a-z][-0-9a-z]*[0-9a-z]*\.)+[a-z0-9][\-a-z0-9]{0,22}[a-z0-9]))$",
                ErrorMessage = "Email is not valid"
            )
        ]
        [Required, Display(Name="Email")]
        public string Email { get; set; }
    }
}