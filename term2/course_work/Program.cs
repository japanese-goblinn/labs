using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Threading;
using System.Collections.Specialized;
using System.Net.Http;
using System.IO;
using Telegram.Bot;
using Telegram.Bot.Types;
using Telegram.Bot.Types.InlineQueryResults;
using Telegram.Bot.Types.ReplyMarkups;
using Telegram.Bot.Types.Enums;
using System.Text.RegularExpressions;
using Newtonsoft.Json;

namespace telegram_bot
{

	class Program
	{
		private static readonly TelegramBotClient botClient = new TelegramBotClient(""); //поле с токеном бота
		public static Schedule scheduleObj = null;
		public static int messageControlForToday = 0;
		public static int messageControlForTomorrow = 0;
		public static int messageControlForWeek = 0;
		public static int[] fromId = {0, 0, 0};
        

		public partial class MemesToSend //классы для отправки мемов с reddit
        {
            public string Kind { get; set; }
            public MemesToSendData Data { get; set; }
        }

        public partial class MemesToSendData
        {
            public string Modhash { get; set; }
            public long? Dist { get; set; }
            public Child[] Children { get; set; }
            public string After { get; set; }
            public object Before { get; set; }
        }

        public partial class Child
        {
            public string Kind { get; set; }
            public ChildData Data { get; set; }
        }

        public partial class ChildData
        {
            public bool? IsCrosspostable { get; set; }
            public string SubredditId { get; set; }
            public object ApprovedAtUtc { get; set; }
            public long? Wls { get; set; }
            public object ModReasonBy { get; set; }
            public object BannedBy { get; set; }
            public object NumReports { get; set; }
            public object RemovalReason { get; set; }
            public long? ThumbnailWidth { get; set; }
            public string Subreddit { get; set; }
            public object SelftextHtml { get; set; }
            public object AuthorFlairTemplateId { get; set; }
            public string Selftext { get; set; }
            public object Likes { get; set; }
            public object SuggestedSort { get; set; }
            public object[] UserReports { get; set; }
            public object SecureMedia { get; set; }
            public bool? IsRedditMediaDomain { get; set; }
            public bool? Saved { get; set; }
            public string Id { get; set; }
            public object BannedAtUtc { get; set; }
            public object ModReasonTitle { get; set; }
            public object ViewCount { get; set; }
            public bool? Archived { get; set; }
            public bool? Clicked { get; set; }
            public bool? NoFollow { get; set; }
            public string Author { get; set; }
            public long? NumCrossposts { get; set; }
            public object LinkFlairText { get; set; }
            public bool? CanModPost { get; set; }
            public bool? SendReplies { get; set; }
            public bool? Pinned { get; set; }
            public long? Score { get; set; }
            public object ApprovedBy { get; set; }
            public bool? Over18 { get; set; }
            public object ReportReasons { get; set; }
            public string Domain { get; set; }
            public bool? Hidden { get; set; }
            public Preview Preview { get; set; }
            public long? Pwls { get; set; }
            public string Thumbnail { get; set; }
            public bool? Edited { get; set; }
            public object LinkFlairCssClass { get; set; }
            public object AuthorFlairCssClass { get; set; }
            public bool? ContestMode { get; set; }
            public long? Gilded { get; set; }
            public bool? Locked { get; set; }
            public long? Downs { get; set; }
            public object[] ModReports { get; set; }
            public long? SubredditSubscribers { get; set; }
            public MediaEmbed SecureMediaEmbed { get; set; }
            public MediaEmbed MediaEmbed { get; set; }
            public string PostHint { get; set; }
            public bool? Stickied { get; set; }
            public bool? Visited { get; set; }
            public bool? CanGild { get; set; }
            public long? ThumbnailHeight { get; set; }
            public string Name { get; set; }
            public bool? Spoiler { get; set; }
            public string Permalink { get; set; }
            public string SubredditType { get; set; }
            public string ParentWhitelistStatus { get; set; }
            public bool? HideScore { get; set; }
            public long? Created { get; set; }
            public string Url { get; set; }
            public object AuthorFlairText { get; set; }
            public bool? Quarantine { get; set; }
            public string Title { get; set; }
            public long? CreatedUtc { get; set; }
            public string SubredditNamePrefixed { get; set; }
            public long? Ups { get; set; }
            public long? NumComments { get; set; }
            public object Media { get; set; }
            public bool? IsSelf { get; set; }
            public string WhitelistStatus { get; set; }
            public object ModNote { get; set; }
            public bool? IsVideo { get; set; }
            public object Distinguished { get; set; }
            public object PostCategories { get; set; }
        }

        public partial class MediaEmbed
        {
        }

        public partial class Preview
        {
            public Image[] Images { get; set; }
            public bool? Enabled { get; set; }
        }

        public partial class Image
        {
            public Source Source { get; set; }
            public Source[] Resolutions { get; set; }
            public MediaEmbed Variants { get; set; }
            public string Id { get; set; }
        }

        public partial class Source
        {
            public string Url { get; set; }
            public long? Width { get; set; }
            public long? Height { get; set; }
        }

		public partial class Schedule //класс для работы с рассписанием
        {
            public object Employee { get; set; }
            public StudentGroup StudentGroup { get; set; }
            public ExamScheduleElement[] Schedules { get; set; }
            public ExamScheduleElement[] ExamSchedules { get; set; }
            public string TodayDate { get; set; }
            public PurpleSchedule[] TodaySchedules { get; set; }
            public string TomorrowDate { get; set; }
            public PurpleSchedule[] TomorrowSchedules { get; set; }
            public long? CurrentWeekNumber { get; set; }
        }

        //различные классы для расписаний
        public partial class ExamScheduleElement
        {
            public string WeekDay { get; set; }
            public PurpleSchedule[] Schedule { get; set; }
        }

		public partial class PurpleSchedule
		{
			public long[] WeekNumber { get; set; }
			public string[] StudentGroup { get; set; }
			public string[] StudentGroupInformation { get; set; }
			public long? NumSubgroup { get; set; }
			public string[] Auditory { get; set; }
			public string LessonTime { get; set; }
			public string StartLessonTime { get; set; }
			public string EndLessonTime { get; set; }
			public string Subject { get; set; }
			public string Note { get; set; }
			public string LessonType { get; set; }
			public Employee[] Employee { get; set; }
			public object StudentGroupModelList { get; set; }
			public bool? Zaoch { get; set; }
        }

        public partial class Employee
        {
            public string FirstName { get; set; }
            public string LastName { get; set; }
            public string MiddleName { get; set; }
            public string Rank { get; set; }
            public string PhotoLink { get; set; }
            public string CalendarId { get; set; }
            public string[] AcademicDepartment { get; set; }
            public long? Id { get; set; }
            public string Fio { get; set; }
        }

        public partial class StudentGroup
        {
            public string Name { get; set; }
            public long? FacultyId { get; set; }
            public object FacultyName { get; set; }
            public long? SpecialityDepartmentEducationFormId { get; set; }
            public object SpecialityName { get; set; }
            public long? Course { get; set; }
            public long? Id { get; set; }
            public string CalendarId { get; set; }
        }
       
		class MenuSendCommands //класс для обработки команд бота
        {
			
            public static string Start(Message message)
            {
                if (message.From.Username == "thicccboy")
                {
                    return $"Приветствую вас, хозяин {message.Chat.FirstName}!";
                }
                return $"Приветствую тебя, {message.Chat.FirstName}!";
            }

            public static string Stop(Message message)
            {
				if (message.From.Username == "thicccboy")
                {
					return $"Рад служить хояин {message.Chat.FirstName}!\nЗавершаем сеанс приклонения вашей великой личности...";
                }
                return $"Было приятно пообщаться, жаль что ты уходишь {message.Chat.FirstName} :(";
            }

            public static string FlipCoin()
            {
                Random random = new Random();
                int whatSide = random.Next(2);
                if (whatSide == 0)
                {
                    return $"Выпала Решка";
                }
                return $"Выпал Орёл";
            }

            public static string GetChangelog()
            {
				string changelog = "ЧТО НОВОГО v.5.0(реализ)\n";
				changelog += "- Мемы с реддит\n";
				changelog += "- Добавлена рассылка сегоднешнего расписания\n";
				changelog += "- Добавлена рассылка расписания на завтра\n";
				changelog += "- Добавлена рассылка расписания на всю неделю\n";
				changelog += "- Добавлены красивости в отправку расписания\n";
				changelog += "- Добавлена функция /help\n";
				changelog += "- Добавлена функция changelog\n";
				changelog += "- Добавлена возможность испытать удачу\n";
				changelog += "- Добавлена возможность остановить скайнет(бота)\n";
				changelog += "- Hmmmmmmmm ответ на неизвестные команды\n";
				changelog += "- Курсач готов\n";
                return changelog;
            }

			public static string Help() 
			{
				string helpString =
					"/help - Справка о существующих командах\n" +
					"/changelog - Что нового?\n" +
					"/flipcoin - 'Испытай удачу'\n" +
					"/sendtodaysch - Отправить расписание на сегодня\n" +
					"/sendtomorrowsch - Отправить расписание на завтра\n" +
					"/sendweeksch - Отправить расписание на текущую неделю\n" +
					"/sendmeme - отправить мем с reddit\n" +
					"/stop - Остановить бота\n";
				return helpString;
			}

			public static string TodaysSchedule(Schedule schedule)
			{            
				int amountOfPr = schedule.TodaySchedules.Length;

                string reply = null;
				string subReply = null;

				reply += $"\uD83D\uDCC5Расписание группы №{schedule.StudentGroup.Name}, дата {schedule.TodayDate}:\n";
				for (int i = 0; i < amountOfPr; i++) 
				{
					subReply += $"\n🕘 {schedule.TodaySchedules[i].LessonTime}\n📕 {schedule.TodaySchedules[i].Subject}({schedule.TodaySchedules[i].LessonType})\n";
					if (schedule.TodaySchedules[i].Subject != "ФизК") 
					{
						subReply += $"\uD83C\uDF93 {schedule.TodaySchedules[i].Employee[0].Fio}\n";
						subReply += $"🚪 {schedule.TodaySchedules[i].Auditory[0]}\n";
					}
				}
				if (subReply == null) 
				{
					return "Пар нет🎉";
				}

				reply += subReply;
				return reply;
			}

			public static string WeekSchedule(Schedule schedule)
			{
				int amountOfDays = schedule.Schedules.Length;
				long? currentWeekNum = schedule.CurrentWeekNumber;

				string reply = null;
				string subReply = null;
                
				reply += $"\uD83D\uDCC5Расписание группы №{schedule.StudentGroup.Name}, на текущую неделю:\n";

				for (int i = 0; i < amountOfDays; i++)
				{
					subReply += $"\n🔺 День '{schedule.Schedules[i].WeekDay}':\n";
					int amountOfPrs = schedule.Schedules[i].Schedule.Length;
					for (int j = 0; j < amountOfPrs; j++)
					{
						bool isCurrentWeek = false;
						foreach (long? weekNum in schedule.Schedules[i].Schedule[j].WeekNumber)
						{
							if (weekNum == currentWeekNum)
							{
								isCurrentWeek = true;
								break;
							}
						}
						if (isCurrentWeek == false) 
						{
							continue;
						}
						subReply += $"🕘 {schedule.Schedules[i].Schedule[j].LessonTime}\n📕 {schedule.Schedules[i].Schedule[j].Subject}" +
							$"({schedule.Schedules[i].Schedule[j].LessonType})\n";
						if (schedule.Schedules[i].Schedule[j].Subject != "ФизК")
						{
							subReply += $"\uD83C\uDF93 {schedule.Schedules[i].Schedule[j].Employee[0].Fio}\n";
							subReply += $"🚪 {schedule.Schedules[i].Schedule[i].Auditory[0]}\n";
						}
					}
				}
				if (subReply == null)
                {
                    subReply += "Пар нет\uD83C\uDF89";
                }
				reply += subReply;
				return reply;
			}

			public static string TomorrowsSchedule(Schedule schedule) 
			{
				int amoutOfPr = schedule.TomorrowSchedules.Length;

                string reply = null;
                string subReply = null;

                reply += $"\uD83D\uDCC5Расписание группы №{schedule.StudentGroup.Name}, дата {schedule.TomorrowDate}:\n";

                for (int i = 0; i < amoutOfPr; i++)
                {
					subReply += $"\n🕘 {schedule.TomorrowSchedules[i].LessonTime}\n📕 {schedule.TomorrowSchedules[i].Subject}({schedule.TomorrowSchedules[i].LessonType})\n";
                    if (schedule.TomorrowSchedules[i].Subject != "ФизК")
                    {
						subReply += $"\uD83C\uDF93 {schedule.TomorrowSchedules[i].Employee[0].Fio}\n";
						subReply += $"🚪 {schedule.TomorrowSchedules[i].Auditory[0]}\n";
                    }
                }
                if (subReply == null)
                {
                    return "Пар нет🎉";
                }

				reply += subReply;
                return reply;
			}
		}

		public static void Main(string[] args)
        {
			while (true)
			{
				Console.Title = botClient.GetMeAsync().Result.Username;
            
				botClient.StartReceiving(); //начало получения ботом сообщений

				botClient.OnMessage += BotClient_OnMessage;
            
				if (Console.ReadLine() == "/stop")
				{
					botClient.StopReceiving();
					return;
				}
			}
        }
      
		static async void BotClient_OnMessage(object sender, Telegram.Bot.Args.MessageEventArgs e) //обработчик события для основного меню 
		{
			Message message = e.Message;
                   
			if (message == null || message.Type != Telegram.Bot.Types.Enums.MessageType.Text) 
			{
				return;
			}

			if (message.Text == "/start") 
			{
				await botClient.SendTextMessageAsync(message.Chat.Id, MenuSendCommands.Start(message));
             
				await Task.Delay(500);

				ReplyKeyboardMarkup ReplyKeyboard = new[]
                   {
					    new[] { "/changelog", "/flipcoin", "/sendmeme"},
                        new[] { "/sendtodaysch", "/sendtomorrowsch", "/sendweeksch"},
					    new[] {"/stop", "/help" },
                   };

				await botClient.SendTextMessageAsync( message.Chat.Id, "Высылаю меню выбора команд...", replyMarkup: ReplyKeyboard);
				return;
			}
         
			else if (messageControlForToday > 0 && message.From.Id == fromId[0])
            {
                if (message == null || message.Type != Telegram.Bot.Types.Enums.MessageType.Text)
                {
                    return;
                }
				SendTodaysSchedule();
                messageControlForToday--;
                fromId[0] = 0;
                return;
            }
			else if (messageControlForTomorrow > 0 && message.From.Id == fromId[1]) 
			{
				if (message == null || message.Type != Telegram.Bot.Types.Enums.MessageType.Text)
                {
                    return;
                }
				SendTomorrowsSchedule();
				messageControlForTomorrow--;
				fromId[1] = 0;
				return;
			}
			else if (messageControlForWeek > 0 && message.From.Id == fromId[2]) 
			{
				if (message == null || message.Type != Telegram.Bot.Types.Enums.MessageType.Text)
                {
                    return;
                }
				SendWeekSchedule();
				messageControlForWeek--;
                fromId[2] = 0;
                return;
			}
			else if (message.Text == "/help") 
			{
				await botClient.SendTextMessageAsync(message.Chat.Id, MenuSendCommands.Help());
				return;
			}
			else if (message.Text == "/changelog")
			{
				await botClient.SendTextMessageAsync(message.Chat.Id, MenuSendCommands.GetChangelog());
				return;
			}
			else if (message.Text == "/stop")
			{
				await botClient.SendTextMessageAsync(message.Chat.Id, MenuSendCommands.Stop(message));
				await botClient.SendTextMessageAsync(message.Chat.Id, "Убираю меню...", replyMarkup: new ReplyKeyboardRemove());
				botClient.StopReceiving();
				return;
			}
			else if (message.Text == "/flipcoin") 
			{
				await botClient.SendTextMessageAsync(message.Chat.Id, MenuSendCommands.FlipCoin());
				return;
			}
			else if (message.Text == "/sendtodaysch") 
			{
				RequestSchedule(1);
				fromId[0] = message.From.Id;
				return;
			}
			else if (message.Text == "/sendtomorrowsch") 
			{
				RequestSchedule(2);
				fromId[1] = message.From.Id;
				return;
			}
			else if (message.Text == "/sendweeksch") 
			{
				RequestSchedule(3);
				fromId[2] = message.From.Id;
				return;
			}
			else if (message.Text == "/sendmeme") 
			{
				SendMemes();
				return;
			}
			else 
			{
                await botClient.SendDocumentAsync(message.Chat.Id, "https://media1.tenor.com/images/b8b44874c2fc5a040a7c97a22cbbbee2/tenor.gif?itemid=11508616");
				await botClient.SendTextMessageAsync(message.Chat.Id, "hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm...");
				await botClient.SendTextMessageAsync(message.Chat.Id, "Не знаю такой команды😔");
				return;
			}
                        
			async void RequestSchedule(int chooseRequest)
            {
				await botClient.SendTextMessageAsync(message.From.Id, "Введите группу:");
				switch (chooseRequest) 
				{
					case 1: messageControlForToday++; break;
					case 2: messageControlForTomorrow++; break;
					case 3: messageControlForWeek++; break;
				}
				return;
            }

			async void SendWeekSchedule() 
			{
                WebClient client = new WebClient();
                string toParse = client.DownloadString("https://students.bsuir.by/api/v1/studentGroup/schedule?studentGroup=" + message.Text);
                scheduleObj = JsonConvert.DeserializeObject<Schedule>(toParse);

                if (scheduleObj != null)
                {
					await botClient.SendTextMessageAsync(message.From.Id, MenuSendCommands.WeekSchedule(scheduleObj));
                }
                else
                {
                    await botClient.SendTextMessageAsync(message.From.Id, "Ошибка Номера Группы");
                }
			}

			async void SendTomorrowsSchedule() 
			{
                WebClient client = new WebClient();
                string toParse = client.DownloadString("https://students.bsuir.by/api/v1/studentGroup/schedule?studentGroup=" + message.Text);
                scheduleObj = JsonConvert.DeserializeObject<Schedule>(toParse);
              
                if (scheduleObj != null)
                {
					await botClient.SendTextMessageAsync(message.From.Id, MenuSendCommands.TomorrowsSchedule(scheduleObj));
                }
                else
                {
                    await botClient.SendTextMessageAsync(message.From.Id, "Ошибка Номера Группы");
                }
			}

			async void SendTodaysSchedule() 
			{
			    WebClient client = new WebClient();
                string toParse = client.DownloadString("https://students.bsuir.by/api/v1/studentGroup/schedule?studentGroup=" + message.Text);
				scheduleObj = JsonConvert.DeserializeObject<Schedule>(toParse);
		
				if (scheduleObj != null)
                {
					await botClient.SendTextMessageAsync(message.From.Id, MenuSendCommands.TodaysSchedule(scheduleObj));
                }
                else
                {
					await botClient.SendTextMessageAsync(message.From.Id, "Ошибка Номера Группы");
                }
			}

			async void SendMemes()
			{
				WebClient webClient = new WebClient(); 
            
				string[] subreddits = 
				{
					"/dankmemes", "/memes", "/Dark_memes", "/PewdiepieSubmissions",
					"/dank_memes_archive", "/Spicy_Memes", "/ProgrammerHumor", "/tumblr", "/dank_meme"
				};

				Random randomForSubs = new Random();
				int whatSub = randomForSubs.Next(0, subreddits.Length);
				string getMemes = webClient.DownloadString($"https://www.reddit.com/r{subreddits[whatSub]}.json"); 
                
				MemesToSend memesToSend = JsonConvert.DeserializeObject<MemesToSend>(getMemes);
				if (memesToSend == null)
				{
					await botClient.SendTextMessageAsync(message.Chat.Id, "сабреддит не найден");
					return;
				}
				int amount = memesToSend.Data.Children.Length; 
				Random random = new Random();
				int iMeme = random.Next(0, amount);
				if (memesToSend.Data.Children[iMeme].Data.Url != null) 
				{
					await botClient.SendPhotoAsync(message.Chat.Id, memesToSend.Data.Children[iMeme].Data.Url);
				}
				else 
				{
					await botClient.SendTextMessageAsync(message.From.Id, "картинки в посте нет😔");
				}
			}
		}
	}
}