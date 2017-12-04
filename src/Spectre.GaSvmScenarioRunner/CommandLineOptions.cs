using System.Collections.Generic;
using System.Linq;
using CommandLine;
using CommandLine.Text;

namespace Spectre.GaSvmScenarioRunner
{
    public class CommandLineOptions
    {
        [Option("Scenario", Required = true, HelpText = "type of scenario to run")]
        public string Scenario { get; set; }

        [Option("Destination", Required = true, HelpText = "destination of the report")]
        public string Destination { get; set; }

        [Option("Source", Required = true, HelpText = "location of the input dataset")]
        public string Source { get; set; }

        [Option("Validation", HelpText = "location of the validation dataset")]
        public string Validation { get; set; }

        [Option("TrainingSetSplitRate", DefaultValue = 0.7f, HelpText = "training set split rate")]
        public double TrainingSetSplitRate { get; set; }

        [Option("MutationRate", DefaultValue = 0.1f, HelpText = "mutation rate")]
        public double MutationRate { get; set; }

        [Option("BitSwapRate", DefaultValue = 0.1f, HelpText = "rate of bit swaps")]
        public double BitSwapRate { get; set; }

        [Option("PreservationRate", DefaultValue = 0.3f, HelpText = "percentage of individuals treated as elite")]
        public double PreservationRate { get; set; }

        [Option("GenerationsNumber", DefaultValue = 50u, HelpText = "number of generations")]
        public uint GenerationsNumber { get; set; }

        [Option("NumberOfRestarts", DefaultValue = 30u, HelpText = "number of time the experiment is repeated")]
        public uint NumberOfRestarts { get; set; }

        [Option("Seed", DefaultValue = 0ul, HelpText = "seed for the RNG")]
        public ulong Seed { get; set; }

        [Option("NumberOfCores", DefaultValue = 1u, HelpText = "Number of cores used for calculations")]
        public uint NumberOfCores { get; set; }

        [Option("MinimalFillup", DefaultValue = 1ul, HelpText = "minimal number of observations taken into account")]
        public ulong MinimalFillup { get; set; }

        [Option("MaximalFillup", DefaultValue = ulong.MaxValue, HelpText = "maximal number of observations taken into account")]
        public ulong MaximalFillup { get; set; }

        [Option("SvmIterations", DefaultValue = 100u, HelpText = "Number of SVM iterations")]
        public uint SvmIterations { get; set; }

        [Option("SvmTolerance", DefaultValue = 1e-6, HelpText = "Tolerance of SVM")]
        public double SvmTolerance { get; set; }

        [Option("DownsampleTrainingRate", DefaultValue = 0.5, HelpText = "Downsample training rate")]
        public double DownsampleTrainingRate { get; set; }

        [Option("MaximumSubsetSize", DefaultValue = 1000, HelpText = "Maximum subset size")]
        public ulong MaximumSubsetSize { get; set; }

        [OptionArray("PopulationSizes", HelpText = "population sizes used in the experiment. Default: 10.")]
        public string[] OptionPopulationSizes
        {
            get
            {
                return PopulationSizes.Select(v => v.ToString()).ToArray();
            }
            set
            {
                PopulationSizes = value.Select(v => uint.Parse(v)).ToArray();
            }
        }

        public IEnumerable<uint> PopulationSizes { get; set; } = new[] { 10u };

        [OptionArray("InitialFillups", HelpText = "number of observations considered at the beginning of the experiment. Default: 4.")]
        public string[] OptionInitialFillups
        {
            get
            {
                return InitialFillups.Select(v => v.ToString()).ToArray();
            }
            set
            {
                InitialFillups = value.Select(v => uint.Parse(v)).ToArray();
            }
        }
        
        public IEnumerable<uint> InitialFillups { get; set; } = new[] { 4u };
    }
}
