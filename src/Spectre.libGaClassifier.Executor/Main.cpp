
#include "Spectre.libGaClassifier/GeneticTrainingSetSelectionScenario.h"

int main()
{
    const std::string CLASSIFIER = "svm";
    const double TRAINING_SET_SPLIT_RATE = 0.7;
    const double MUTATION_RATE = 0.5;
    const double BIT_SWAP_RATE = 0.5;
    const double PRESERVATION_RATE = 0.5;
    const unsigned int GENERATIONS_NUMBER = 30u;
    const unsigned int POPULATION_SIZE = 20u;
    const unsigned int INITIAL_FILLUP = 10u;
    const _ULonglong SEED = 454646;

    spectre::supervised::GeneticTrainingSetSelectionScenario scenario(CLASSIFIER, TRAINING_SET_SPLIT_RATE, MUTATION_RATE, BIT_SWAP_RATE,
        PRESERVATION_RATE, GENERATIONS_NUMBER, POPULATION_SIZE, INITIAL_FILLUP, SEED);

    scenario.execute("test_scenario_file.txt", "test_serious_report", POPULATION_SIZE);

    return 0;
}
