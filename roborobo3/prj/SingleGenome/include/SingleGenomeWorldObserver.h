/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 *
 */


#ifndef SINGLEGENOMEWORLDOBSERVER_H
#define SINGLEGENOMEWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "Observers/Observer.h"
#include "Observers/WorldObserver.h"
#include "WorldModels/RobotWorldModel.h"
#include "SingleGenome/include/SingleGenomeSharedData.h"

//class World;

class SingleGenomeWorldObserver : public WorldObserver
{
protected:
    virtual void updateEnvironment();
    virtual void updateMonitoring();
    virtual void monitorPopulation( bool localVerbose = true );
    
    int _generationCount;
    int _generationItCount;
    
    std::ofstream _statsLogFile;
    std::ofstream _genomeLogFile;
    LogManager *_statsLogManager;
    LogManager *_genomeLogManager;
    
public:
    SingleGenomeWorldObserver(World *world);
    ~SingleGenomeWorldObserver();
    
    virtual void reset();
    virtual void step();
    void stepEvaluation();
    
    virtual int getGenerationItCount() { return _generationItCount; }

};

#endif