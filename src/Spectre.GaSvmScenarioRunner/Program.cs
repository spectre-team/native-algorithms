/*
* Program.cs
* Entry point for client responsible for running GaSvm scenarios.
*
Copyright 2017 Spectre Team

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
                var chosenScenario = options.Scenario;

                if (chosenScenario.Equals("tss"))
                {
                    var scenario = new TrainingSetSelection(
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
                    var validation = options.Validation == null ? null : new BasicTextDataset(options.Validation);
                    scenario.execute(data, validation);
                }
                else if (chosenScenario.Equals("rmv"))
                {
                    var scenario = new RandomMultipleValidationSelection(
                        options.TrainingSetSplitRate,
                        options.DownsampleTrainingRate,
                        options.MaximumSubsetSize,
                        options.MutationRate,
                        options.BitSwapRate,
                        options.PreservationRate,
                        options.GenerationsNumber,
                        options.PopulationSizes.First(),
                        options.InitialFillups.First(),
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
                    var validation = options.Validation == null ? null : new BasicTextDataset(options.Validation);
                    scenario.execute(data, validation);
                }
            }
        }
    }
}
