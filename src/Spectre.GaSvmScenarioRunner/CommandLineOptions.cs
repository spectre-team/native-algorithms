﻿using System.Collections.Generic;
using CommandLine;
using CommandLine.Text;

namespace Spectre.GaSvmScenarioRunner
{
    public class CommandLineOptions
    {
        [Option(Required = true, HelpText = "destination of the report")]
        public string Destination { get; set; }

        [Option(Required = true, HelpText = "location of the input dataset")]
        public string Source { get; set; }

        [Option("TrainingSetSplitRate", DefaultValue=0.7f, HelpText = "training set split rate")]
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

        [Option("PopulationSizes", DefaultValue = new [] { 10u }, HelpText = "population sizes used in the experiment. Default: 10.")]
        public IEnumerable<uint> PopulationSizes { get; set; }

        [Option("InitialFillups", DefaultValue = new [] { 4u }, HelpText = "number of observations considered at the beginning of the experiment. Default: 4.")]
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
