/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */



#include "SingleGenome/include/SingleGenomeSharedData.h"

double SingleGenomeSharedData::gSigmaMin = 0.0;
double SingleGenomeSharedData::gProbaMutation = 0.0;
double SingleGenomeSharedData::gUpdateSigmaStep = 0.0;
double SingleGenomeSharedData::gSigmaRef = 0.0; // reference value of sigma
double SingleGenomeSharedData::gSigmaMax = 0.0; // maximal value of sigma
int SingleGenomeSharedData::gEvaluationTime = 0; // how long a controller will be evaluated on a robot

bool SingleGenomeSharedData::gSynchronization = true;

bool SingleGenomeSharedData::gEnergyRequestOutput = 1;

double SingleGenomeSharedData::gMonitorPositions;

bool SingleGenomeSharedData::gPropertiesLoaded = false; // global variable local to file -- TODO: move specific properties loader in dedicated WorldObserver

int SingleGenomeSharedData::gNbHiddenLayers = 1;
int SingleGenomeSharedData::gNbNeuronsPerHiddenLayer = 5;
int SingleGenomeSharedData::gNeuronWeightRange = 800;

bool SingleGenomeSharedData::gSnapshots = true; // take snapshots
int SingleGenomeSharedData::gSnapshotsFrequency = 50; // every N generations

int SingleGenomeSharedData::gControllerType = -1; // cf. header for description

bool SingleGenomeSharedData::gLimitGenomeTransmission = false; // default: do not limit.
int SingleGenomeSharedData::gMaxNbGenomeTransmission = 65535; // default: arbitrarily set to 65535.

int SingleGenomeSharedData::gSelectionMethod = 0; // default: random selection

int SingleGenomeSharedData::gNotListeningStateDelay = 0;    // -1: infinite ; 0: no delay ; >0: delay
int SingleGenomeSharedData::gListeningStateDelay = -1;      // -1: infinite ; 0: no delay ; >0: delay (ignored if gNotListeningStateDelay=-1)

bool SingleGenomeSharedData::gLogGenome = false;
bool SingleGenomeSharedData::gLogGenomeSnapshot = false;

double SingleGenomeSharedData::gIndividualMutationRate = 1.0;

int SingleGenomeSharedData::gMutationOperator = 1; // 0: uniform, 1: gaussian

double SingleGenomeSharedData::gSigma = 0.01; // 0.01 is just some random value.

int SingleGenomeSharedData::gMemorySize = 20;

bool SingleGenomeSharedData::gTotalEffort = true;

int SingleGenomeSharedData::gFakeRobotsPerObject = 0; // maximum number of ...
double SingleGenomeSharedData::gFakeCoopValue = 0; // maximum value of ...
int SingleGenomeSharedData::gFakeCoopSteps = 30; // how many values do we want to sample
int SingleGenomeSharedData::gNbReplicas = 10;
bool SingleGenomeSharedData::gOnlyOneRobot = true;

double SingleGenomeSharedData::gConstantK = 0;
double SingleGenomeSharedData::gConstantA = 0;

int SingleGenomeSharedData::gGenerationLog = 5000;
bool SingleGenomeSharedData::gTakeVideo = false;

std::string SingleGenomeSharedData::gGenomeFilename = "";
