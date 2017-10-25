using System.Collections.Generic;
using CommandLine;
using CommandLine.Text;

namespace Spectre.GaSvmScenarioRunner
{
    public class CommandLineOptions
    {
        [Option(Required = true, HelpText = "destination of the report")]
        public string Destination { get; set; }

        [Option(DefaultValue=0.7f, HelpText = "training set split rate")]
        public double TrainingSetSplitRate { get; set; }

        [Option(DefaultValue = 0.1f, HelpText = "mutation rate")]
        public double MutationRate { get; set; }

        [Option(DefaultValue = 0.1f, HelpText = "rate of bit swaps")]
        public double BitSwapRate { get; set; }

        [Option(DefaultValue = 30, HelpText = "number of time the experiment is repeated")]
        public uint NumberOfRestarts { get; set; }

        [Option(DefaultValue = 0, HelpText = "seed for the RNG")]
        public ulong Seed { get; set; }

        [Option(DefaultValue = new uint[] { 10 }, HelpText = "population sizes used in the experiment")]
        public IEnumerable<uint> PopulationSizes { get; set; }

        [Option(DefaultValue = new uint[] { 4 }, HelpText = "number of observations considered at the beginning of the experiment")]
        public IEnumerable<uint> InitialFillups { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(options: this,
                onError: current => HelpText.DefaultParsingErrorsHandler(options: this,
                                                                         current: current));
        }
    }
}
