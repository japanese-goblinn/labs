#pragma checksum "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml" "{ff1816ec-aa5e-4d10-87f7-6f4963833460}" "c9512ca0fa760757c00676f2d94a968a28f3dd08"
// <auto-generated/>
#pragma warning disable 1591
[assembly: global::Microsoft.AspNetCore.Razor.Hosting.RazorCompiledItemAttribute(typeof(AspNetCore.Views_Home_Index), @"mvc.1.0.view", @"/Views/Home/Index.cshtml")]
[assembly:global::Microsoft.AspNetCore.Mvc.Razor.Compilation.RazorViewAttribute(@"/Views/Home/Index.cshtml", typeof(AspNetCore.Views_Home_Index))]
namespace AspNetCore
{
    #line hidden
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading.Tasks;
    using Microsoft.AspNetCore.Mvc;
    using Microsoft.AspNetCore.Mvc.Rendering;
    using Microsoft.AspNetCore.Mvc.ViewFeatures;
#line 1 "/Users/saisuicied/Downloads/Twitter/Views/_ViewImports.cshtml"
using Twitter;

#line default
#line hidden
#line 2 "/Users/saisuicied/Downloads/Twitter/Views/_ViewImports.cshtml"
using Twitter.Models;

#line default
#line hidden
#line 1 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
using Microsoft.AspNetCore.Mvc.Localization;

#line default
#line hidden
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"c9512ca0fa760757c00676f2d94a968a28f3dd08", @"/Views/Home/Index.cshtml")]
    [global::Microsoft.AspNetCore.Razor.Hosting.RazorSourceChecksumAttribute(@"SHA1", @"d8f43db5ec98e635fa58aaa0469028b9920c5dc4", @"/Views/_ViewImports.cshtml")]
    public class Views_Home_Index : global::Microsoft.AspNetCore.Mvc.Razor.RazorPage<IEnumerable<Tweet>>
    {
        #pragma warning disable 1998
        public async override global::System.Threading.Tasks.Task ExecuteAsync()
        {
            BeginContext(105, 1, true);
            WriteLiteral("\n");
            EndContext();
#line 5 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
  
    ViewData["Title"] = Localizer["Title"];

#line default
#line hidden
            BeginContext(155, 56, true);
            WriteLiteral("\n<div class=\"text-left mt-3\">\n    <h3 class=\"display-6\">");
            EndContext();
            BeginContext(212, 17, false);
#line 10 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
                     Write(ViewData["Title"]);

#line default
#line hidden
            EndContext();
            BeginContext(229, 68, true);
            WriteLiteral("</h3>\n</div>\n<hr/>\n<div style=\"overflow-y: scroll; height: 730px;\">\n");
            EndContext();
#line 14 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
      foreach (var tweet in Model)
        {
            

#line default
#line hidden
            BeginContext(355, 59, false);
#line 16 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
       Write(await Component.InvokeAsync("Tweet", new { id = tweet.Id }));

#line default
#line hidden
            EndContext();
            BeginContext(415, 18, true);
            WriteLiteral("            <hr/>\n");
            EndContext();
#line 18 "/Users/saisuicied/Downloads/Twitter/Views/Home/Index.cshtml"
        }

#line default
#line hidden
            BeginContext(443, 7, true);
            WriteLiteral("</div>\n");
            EndContext();
        }
        #pragma warning restore 1998
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public IViewLocalizer Localizer { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.ViewFeatures.IModelExpressionProvider ModelExpressionProvider { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.IUrlHelper Url { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.IViewComponentHelper Component { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.Rendering.IJsonHelper Json { get; private set; }
        [global::Microsoft.AspNetCore.Mvc.Razor.Internal.RazorInjectAttribute]
        public global::Microsoft.AspNetCore.Mvc.Rendering.IHtmlHelper<IEnumerable<Tweet>> Html { get; private set; }
    }
}
#pragma warning restore 1591
