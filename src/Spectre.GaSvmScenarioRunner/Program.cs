using System.Linq;
using Spectre.GASVMCli;
using Spectre.Data.Datasets;

namespace Spectre.GaSvmScenarioRunner
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var options = new CommandLineOptions();
            if (CommandLine.Parser.Default.ParseArguments(args, options))
            {
                var scenario = new GaSvmScenario(
                    options.TrainingSetSplitRate,
                    options.MutationRate,
                    options.BitSwapRate,
                    options.PreservationRate,
                    options.GenerationsNumber,
                    options.PopulationSizes.ToArray(),
                    options.InitialFillups.ToArray(),
                    options.Destination,
                    options.NumberOfRestarts,
                    options.NumberOfCores,
                    options.Seed,
                    options.MinimalFillup,
                    options.MaximalFillup,
                    options.SvmIterations,
                    options.SvmTolerance
                    );
                var data = new BasicTextDataset(options.Source);
                scenario.execute(data);
            }
        }
    }
}
