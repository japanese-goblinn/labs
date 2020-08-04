using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Twitter.Models;
using Microsoft.EntityFrameworkCore;
using Microsoft.AspNetCore.Localization;
using System.Globalization;
using Microsoft.AspNetCore.Routing.Constraints;
using Microsoft.Extensions.Options;

namespace Twitter
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.Configure<CookiePolicyOptions>(options =>
            {
                // This lambda determines whether user consent for non-essential cookies is needed for a given request.
                options.CheckConsentNeeded = context => true;
                options.MinimumSameSitePolicy = SameSiteMode.None;
            });
            
            services.AddDbContext<TwitterDBContext>(
                options => options
                    .UseSqlite(Configuration.GetConnectionString("TwitterContext"))
            );

            services.AddIdentity<User, IdentityRole>()
                .AddEntityFrameworkStores<TwitterDBContext>();
    
            services.AddSignalR();
            
            services.AddLocalization(options => options.ResourcesPath = "Resources");
            
            services.AddMvc()
                .AddDataAnnotationsLocalization()
                .AddViewLocalization();
            
            services.Configure<RequestLocalizationOptions>(options =>
            {
                var supportedCultures = new []
                {
                    new CultureInfo("en"),
                    new CultureInfo("ru")
                };
 
                options.DefaultRequestCulture = new RequestCulture("en");
                options.SupportedCultures = supportedCultures;
                options.SupportedUICultures = supportedCultures;
            });
            
            services.AddMvc().SetCompatibilityVersion(CompatibilityVersion.Version_2_2);
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }
            else
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            
            var locOptions = app.ApplicationServices.GetService<IOptions<RequestLocalizationOptions>>();
            app.UseRequestLocalization(locOptions.Value);

            app.UseHttpsRedirection();
            app.UseStaticFiles();
            app.UseAuthentication();
            app.UseCookiePolicy();

            app.UseSignalR(routes =>
            {
                routes.MapHub<ChatHub>("/chat");
            });

            app.UseMvc(routes =>
            {
                routes.MapRoute(name: "Index", template: "/", new  
                {
                    controller = "Home", action = "Index"
                });
                routes.MapRoute(name: "Retweet", template: "Home/Retweet/{id}", new
                {
                    id = new IntRouteConstraint(), controller = "Home", action = "Retweet"
                });
                routes.MapRoute(name: "DeleteRetweet", template: "Home/DeleteRetweet/{id}", new
                {
                    id = new IntRouteConstraint(), controller = "Home", action = "DeleteRetweet"
                });
                routes.MapRoute(name: "Like", template: "Home/Like/{id}", new
                {
                    id = new IntRouteConstraint(), controller = "Home", action = "Like"
                });
                routes.MapRoute(name: "DeleteLike", template: "Home/DeleteLike/{id}", new
                {
                    id = new IntRouteConstraint(), controller = "Home", action = "DeleteLike"
                });
                routes.MapRoute(name: "Replies", template: "Home/Replies/{id}", new
                {
                    id = new IntRouteConstraint(), controller = "Home", action = "Replies"
                });
                routes.MapRoute(name: "Reply", template: "Home/Reply", new
                {
                    controller = "Home", action = "Reply"
                });
                routes.MapRoute(name: "Tweet", template: "Home/Tweet", new
                {
                    controller = "Home", action = "Tweet"
                });
                routes.MapRoute(name: "Privacy", template: "Home/Privacy", new
                {
                    controller = "Home", action = "Privacy"
                });
                routes.MapRoute(name: "Register", template: "Account/Register", new
                {
                    controller = "Account", action = "Register"
                });
                routes.MapRoute(name: "Login", template: "Account/Login", new
                {
                    controller = "Account", action = "Login"
                });
                routes.MapRoute(name: "Logout", template: "Account/LogOff", new
                {
                    controller = "Account", action = "LogOff"
                });
                routes.MapRoute(name: "ChangeLanguage", template: "Language/SetLanguage", new
                {
                    controller = "Language", action = "SetLanguage"
                });
                routes.MapRoute(name: "MessagesIndex", template: "Messages", new
                {
                    controller = "Messages", action = "Index"
                });
                routes.MapRoute(name: "Chat", template: "Messages/{id?}", new
                {
                    controller = "Messages", action = "ChatMessages"
                });
                routes.MapRoute(name: "ProfileIndex", template: "Profile/{userName?}", new
                {
                    controller = "Profile", action = "Index"
                });
                routes.MapRoute(name: "Follow", template: "Profile/Follow/{userName?}", new
                {
                    controller = "Profile", action = "Follow"
                });
                routes.MapRoute(name: "Unfollow", template: "Profile/Unfollow/{userName?}", new
                {
                    controller = "Profile", action = "Unfollow"
                });
                routes.MapRoute(name: "WriteMessage", template: "Profile/WriteMessage/{userName?}", new
                {
                    controller = "Profile", action = "WriteMessage"
                });
                routes.MapRoute(name: "UsersIndex", template: "Users", new
                {
                    controller = "Users", action = "Index"
                });
                routes.MapRoute(name: "CreateUser", template: "Users/Create", new
                {
                    controller = "Users", action = "Create"
                });
                routes.MapRoute(name: "EditUsers", template: "Users/Edit", new
                {
                    controller = "Users", action = "Edit"
                });
                routes.MapRoute(name: "DeleteUsers", template: "Users/Delete/{id?}", new
                {
                    controller = "Users", action = "Delete"
                });
                routes.MapRoute(name: "ChangeUsersPasswords", template: "Users/ChangePassword", new
                {
                    controller = "Users", action = "ChangePassword"
                });
                routes.MapRoute(name: "TweetsIndex", template: "Tweets", new
                {
                    controller = "Tweets", action = "Index"
                });
                routes.MapRoute(name: "EditTweet", template: "Tweets/Edit", new
                {
                    controller = "Tweets", action = "Edit"
                });
                routes.MapRoute(name: "DeleteTweet", template: "Tweets/Delete", new
                {
                    controller = "Tweets", action = "Delete"
                });
                routes.MapRoute(name: "RolesIndex", template: "Roles", new
                {
                    controller = "Roles", action = "Index"
                });
                routes.MapRoute(name: "CreateRole", template: "Roles/Create", new
                {
                    controller = "Roles", action = "Create"
                });
                routes.MapRoute(name: "DeleteRole", template: "Roles/Delete/{id?}", new
                {
                    controller = "Roles", action = "Delete"
                });
                routes.MapRoute(name: "EditRole", template: "Roles/Edit", new
                {
                    controller = "Roles", action = "Edit"
                });
            });
        }
    }
}