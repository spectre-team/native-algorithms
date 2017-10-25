using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Spectre.GaSvmScenarioRunner
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var options = new CommandLineOptions();
            if (CommandLine.Parser.Default.ParseArguments(args, options))
            {
                // @gmrukwa: TODO logic here.
            }
            else
            {
                Console.WriteLine(options.GetUsage());
            }
        }
    }
}
