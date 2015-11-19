using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;
using Newtonsoft.Json;

namespace DotaMining
{
    class Program
    {
        static void GetIds(string idPath, string webPath, int MAX)
        {
            int id = 1815098661;
            try
            {
                StreamReader ids = new StreamReader(File.OpenRead(idPath));
                string str = ids.ReadLine();
                while (str != null)
                {
                    id = int.Parse(str);
                    str = ids.ReadLine();
                }
                ids.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            id++;
            writer = File.AppendText(idPath);
            int sleepTime = 60 * 60 * 1000;
            while (true)
            {
                HttpWebRequest request = (HttpWebRequest)HttpWebRequest.CreateHttp(webPath + id);
                request.UserAgent = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0";
                request.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
                request.Headers.Add(HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5");
                try
                {
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                    if (response.StatusCode == HttpStatusCode.OK)
                    {
                        sleepTime = 60 * 60 * 1000;
                        writer.WriteLine(id);
                        Console.WriteLine(id);
                    }

                    response.Close();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    if (e.Message.Contains("429"))
                    {
                        System.Threading.Thread.Sleep(sleepTime);
                        sleepTime = Math.Max(sleepTime / 2, 15 * 60 * 1000);
                    }
                }
                writer.Flush();
                id++;
            }
            //writer.Close();
        }
        static void GetDatas(string idPath, string dataPath, string webPath)
        {
            StreamReader ids = new StreamReader(File.OpenRead(idPath));
            string str = "";
            int id = 0;
            try
            {
                StreamReader dataIds = new StreamReader(File.OpenRead(dataPath));

                string[] split;
                char[] splitParam = { ',' };

                str = dataIds.ReadLine();
                while (str != null)
                {
                    split = str.Split(splitParam, 2);
                    if (split.Length == 2)
                        id = int.Parse(split[0]);
                    str = dataIds.ReadLine();
                }
                dataIds.Close();

                str = ids.ReadLine();
                while (str != null)
                {
                    if (id == int.Parse(str))
                    {
                        str = ids.ReadLine();
                        break;
                    }
                    str = ids.ReadLine();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                str = ids.ReadLine();
            }

            writer = File.AppendText(dataPath);

            int sleepTime = 60 * 60 * 1000;

            while (str != null)
            {
                id = int.Parse(str);
                HttpWebRequest request = (HttpWebRequest)HttpWebRequest.CreateHttp(webPath + id);
                request.UserAgent = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0";
                request.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
                request.Headers.Add(HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5");
                try
                {
                    HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                    if (response.StatusCode == HttpStatusCode.OK)
                    {
                        sleepTime = 60 * 60 * 1000;
                        StreamReader htmlStream = new StreamReader(response.GetResponseStream());
                        string html = htmlStream.ReadToEnd();
                        html = html.Replace("\n", "").Replace("\r", "").Replace("-", " ");
                        Regex regexMatchResult = new Regex("<div class=\"match result team (\\w*)\">");
                        Regex regexAbandon = new Regex("<div class=\"[\\w ]*abandoned\">");
                        Regex regexSkill = new Regex("<dd>([\\w ]*)</dd><dt>Skill Bracket</dt>");
                        Regex regexLobbyType = new Regex("<dd>([\\w ]*)</dd><dt>Lobby Type</dt>");
                        Regex regexGameMode = new Regex("<dd>([\\w ]*)</dd><dt>Game Mode</dt>");
                        Regex regexRadiantHero = new Regex("<tr class=\"faction radiant\"><td><div class=\"[\\w ]*\"><a href=\"\\/heroes\\/([\\w \\/]*)\">");
                        Regex regexDireHero = new Regex("<tr class=\"faction dire\"><td><div class=\"[\\w ]*\"><a href=\"\\/heroes\\/([\\w \\/]*)\">");
                        Match matchResult = regexMatchResult.Match(html);
                        Match abandon = regexAbandon.Match(html);
                        Match skillBracket = regexSkill.Match(html);
                        Match lobbyType = regexLobbyType.Match(html);
                        Match gameMode = regexGameMode.Match(html);
                        MatchCollection radiantHeroes = regexRadiantHero.Matches(html);
                        MatchCollection direHeroes = regexDireHero.Matches(html);
                        bool radiantVictory = false;
                        bool fail = false;

                        if (abandon.Success)
                        {
                            Console.WriteLine(id + " ABANDON");
                            writer.WriteLine(id + ",ABANDON");
                        }
                        else if (matchResult.Success && matchResult.Groups.Count > 1)
                        {
                            if (matchResult.Groups[1].Value.Equals("radiant"))
                                radiantVictory = true;

                            int heroCount = 0;
                            int[] radiantHeroesPos = { 0, 0, 0, 0, 0 };
                            int[] direHeroesPos = { 0, 0, 0, 0, 0 };

                            foreach (Match hero in radiantHeroes)
                            {
                                if (hero.Success && hero.Groups.Count > 1)
                                {
                                    radiantHeroesPos[heroCount] = heroesPos[hero.Groups[1].Value];
                                    heroCount++;
                                }
                            }
                            if (heroCount == 5)
                            {
                                Array.Sort(radiantHeroesPos);
                                heroCount = 0;
                                foreach (Match hero in direHeroes)
                                {
                                    if (hero.Success && hero.Groups.Count > 1)
                                    {
                                        direHeroesPos[heroCount] = heroesPos[hero.Groups[1].Value];
                                        heroCount++;
                                    }
                                }
                                if (heroCount == 5)
                                {

                                    string skill = "";
                                    string lobby = "";
                                    string mode = "";
                                    if (skillBracket.Success && skillBracket.Groups.Count > 1)
                                        skill = skillBracket.Groups[1].Value;
                                    if (lobbyType.Success && lobbyType.Groups.Count > 1)
                                        lobby = lobbyType.Groups[1].Value;
                                    if (gameMode.Success && gameMode.Groups.Count > 1)
                                        mode = gameMode.Groups[1].Value;

                                    writer.Write(id + ",OK," + skill + "," + lobby + "," + mode + ",");

                                    Array.Sort(direHeroesPos);
                                    heroCount = 0;
                                    for (int i = 0; i < heroes.Length; i++)
                                    {
                                        if (heroCount < 5 && i == radiantHeroesPos[heroCount])
                                        {
                                            writer.Write("1,");
                                            heroCount++;
                                        }
                                        else
                                            writer.Write("0,");
                                    }
                                    if (radiantVictory)
                                        writer.WriteLine("1");
                                    else
                                        writer.WriteLine("0");

                                    writer.Write(id + ",OK," + skill + "," + lobby + "," + mode + ",");
                                    heroCount = 0;
                                    for (int i = 0; i < heroes.Length; i++)
                                    {
                                        if (heroCount < 5 && i == direHeroesPos[heroCount])
                                        {
                                            writer.Write("1,");
                                            heroCount++;
                                        }
                                        else
                                            writer.Write("0,");
                                    }
                                    if (!radiantVictory)
                                        writer.WriteLine("1");
                                    else
                                        writer.WriteLine("0");
                                    Console.WriteLine("OK");
                                }
                                else
                                    fail = true;
                            }
                            else
                                fail = true;
                        }
                        else
                            fail = true;
                        if (fail)
                        {
                            Console.WriteLine(id + " FAIL");
                            writer.WriteLine(id + ",FAIL");
                        }
                    }
                    writer.Flush();
                    response.Close();
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                    if (e.Message.Contains("429"))
                    {
                        System.Threading.Thread.Sleep(sleepTime);
                        sleepTime = Math.Max(sleepTime / 2, 15 * 60 * 1000);
                    }
                }
                writer.Flush();
                str = ids.ReadLine();
            }
            ids.Close();
            writer.Close();
        }

        static void GetHeroes(string webPath)
        {
            HttpWebRequest request = (HttpWebRequest)HttpWebRequest.CreateHttp(webPath);
            request.UserAgent = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0";
            request.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
            request.Headers.Add(HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5");
            try
            {
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                if (response.StatusCode == HttpStatusCode.OK)
                {
                    StreamReader htmlStream = new StreamReader(response.GetResponseStream());
                    string json = htmlStream.ReadToEnd();
                    dynamic jsonObject = JsonConvert.DeserializeObject(json);
                    foreach (dynamic hero in jsonObject.result.heroes)
                    {
                        string heroName = hero.name;
                        heroName = heroName.Replace("npc_dota_hero_", "").Replace('_', ' ');
                        heroesId.Add((int)hero.id, heroName);
                        StreamWriter heroFile = new StreamWriter(File.OpenWrite(heroName));
                        heroFile.WriteLine("id,k,d,a,gpm,xpm,win");
                        heroFile.Close();
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void GetItems(string webPath)
        {
            HttpWebRequest request = (HttpWebRequest)HttpWebRequest.CreateHttp(webPath);
            request.UserAgent = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0";
            request.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
            request.Headers.Add(HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5");
            try
            {
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                if (response.StatusCode == HttpStatusCode.OK)
                {
                    StreamReader htmlStream = new StreamReader(response.GetResponseStream());
                    string json = htmlStream.ReadToEnd();
                    dynamic jsonObject = JsonConvert.DeserializeObject(json);
                    foreach (dynamic item in jsonObject.result.items)
                    {
                        itemsId.Add((int)item.id, (string)item.localized_name);
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void GetDetails(string idPath, string webPath)
        {
            string id;
            try
            {
                StreamReader ids = new StreamReader(File.OpenRead(idPath));
                id = ids.ReadLine();
                while (id != null)
                {
                    HttpWebRequest request = (HttpWebRequest)HttpWebRequest.CreateHttp(webPath + id);
                    request.UserAgent = "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:31.0) Gecko/20100101 Firefox/31.0";
                    request.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
                    request.Headers.Add(HttpRequestHeader.AcceptLanguage, "en-us,en;q=0.5");
                    try
                    {
                        HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                        if (response.StatusCode == HttpStatusCode.OK)
                        {
                            StreamReader htmlStream = new StreamReader(response.GetResponseStream());
                            string json = htmlStream.ReadToEnd();
                            dynamic jsonObject = JsonConvert.DeserializeObject(json);
                            foreach (dynamic player in jsonObject.result.players)
                            {
                                int hero = player.hero_id;
                                int kill = player.kills;
                                int death = player.deaths;
                                int assist = player.assists;
                                int gpm = player.gold_per_min;
                                int xpm = player.xp_per_min;
                                string heroFilename = heroesId[hero];
                                int playerSlot = player.player_slot;
                                bool win = (bool)jsonObject.result.radiant_win;
                                if(playerSlot > 127)
                                    win = !win;
                                StreamWriter heroFile = File.AppendText(heroFilename);
                                heroFile.WriteLine(id + "," + kill + "," + death + "," + assist + "," + gpm + "," + xpm + "," + (win == true ? 1 : 0) );
                                heroFile.Close();

                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(e.Message);
                    }
                    id = ids.ReadLine();
                    System.Threading.Thread.Sleep(500);
                }
                ids.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            writer.Close();
        }

        static void Main(string[] args)
        {
            Array.Sort(heroes);
            for (int i = 0; i < heroes.Length; i++)
            {
                heroesPos.Add(heroes[i], i);
            }
            string heroWebPath = "https://api.steampowered.com/IEconDOTA2_570/GetHeroes/v0001/?key={0}";
            string itemWebPath = "https://api.steampowered.com/IEconDOTA2_570/GetGameItems/v0001/?key={0}&language={1}";
            string detailWebPath = "https://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/V001/?key={0}&match_id=";

            GetHeroes(string.Format(heroWebPath, KEY));
            GetItems(string.Format(itemWebPath, KEY, LANG));
            GetDetails("idsAP.txt", string.Format(detailWebPath, KEY));
            //CABEÇALHO
            //Console.Write("id,status,skill,lobby,mode,");
            //for (int i = 0; i < heroes.Length; i++)
            //{
            //    Console.Write(heroes[i] + ",");
            //}
            //Console.WriteLine("win");

            //GetDatas("ids.txt", "data.csv", "http://www.dotabuff.com/matches/");
            if (args.Length > 0)
                switch (args[0])
                {
                    case "-id":
                        if (args.Length > 1)
                            GetIds(args[1], "http://www.dotabuff.com/matches/", 100000);
                        break;
                    case "-data":
                        if (args.Length > 2)
                            GetDatas(args[1], args[2], "http://www.dotabuff.com/matches/");
                        break;
                    case "-detail":
                        if (args.Length > 1)
                            GetDetails(args[1], string.Format(detailWebPath, KEY));
                        break;
                    default:
                        Console.WriteLine("Invalid commands. Try : -id <file_name>, -data <id_file_name> <data_file_name>");
                        break;
                }
        }

        static StreamWriter writer = null;
        static Dictionary<string, int> heroesPos = new Dictionary<string, int>();
        static Dictionary<int, string> heroesId = new Dictionary<int, string>();
        static Dictionary<int, string> itemsId = new Dictionary<int, string>();
        static string KEY = "90C289139A71A2D83269DBF40C1AEDD1";
        static string LANG = "en";
        static string[] heroes = {"anti mage",
        "axe",
        "crystal maiden",
        "dazzle",
        "drow ranger",
        "earthshaker",
        "lich",
        "lina",
        "lion",
        "mirana",
        "morphling",
        "necrophos",
        "puck",
        "pudge",
        "razor",
        "sand king",
        "shadow shaman",
        "storm spirit",
        "sven",
        "tidehunter",
        "vengeful spirit",
        "windranger",
        "witch doctor",
        "zeus",
        "slardar",
        "enigma",
        "faceless void",
        "tiny",
        "viper",
        "venomancer",
        "clockwerk",
        "natures prophet",
        "dark seer",
        "sniper",
        "pugna",
        "beastmaster",
        "enchantress",
        "leshrac",
        "shadow fiend",
        "tinker",
        "weaver",
        "night stalker",
        "ancient apparition",
        "spectre",
        "doom",
        "chen",
        "juggernaut",
        "bloodseeker",
        "kunkka",
        "riki",
        "queen of pain",
        "wraith king",
        "broodmother",
        "huskar",
        "jakiro",
        "batrider",
        "omniknight",
        "dragon knight",
        "warlock",
        "alchemist",
        "lifestealer",
        "death prophet",
        "ursa",
        "bounty hunter",
        "silencer",
        "spirit breaker",
        "invoker",
        "clinkz",
        "outworld devourer",
        "bane",
        "shadow demon",
        "lycan",
        "lone druid",
        "brewmaster",
        "phantom lancer",
        "treant protector",
        "ogre magi",
        "gyrocopter",
        "chaos knight",
        "phantom assassin",
        "rubick",
        "luna",
        "io",
        "undying",
        "disruptor",
        "templar assassin",
        "naga siren",
        "nyx assassin",
        "keeper of the light",
        "visage",
        "meepo",
        "magnus",
        "centaur warrunner",
        "slark",
        "timbersaw",
        "medusa",
        "troll warlord",
        "tusk",
        "bristleback",
        "skywrath mage",
        "elder titan",
        "abaddon",
        "ember spirit",
        "earth spirit",
        "legion commander",
        "phoenix",
        "terrorblade",
        "techies",
        "oracle",
        "winter wyvern"};
    }
}
