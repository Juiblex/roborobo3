#include "World/CircleObject.h"

#include "RoboroboMain/roborobo.h"
#include "Utilities/Misc.h"
#include "World/World.h"

#include <iomanip>
#include <algorithm>

CircleObject::CircleObject( int __id ) : PhysicalObject( __id ) // a unique and consistent __id should be given as argument
{
	std::string s = "";
	std::stringstream out;
	out << getId();
    
    s = "physicalObject[";
	s += out.str();
	s += "].radius";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_radius, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type CircleObject) missing radius (integer, >=0). Check default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultRadius", &_radius, true);
    }
    
    s = "physicalObject[";
	s += out.str();
	s += "].footprintRadius";
	if ( gProperties.hasProperty( s ) )
		convertFromString<int>(_footprintRadius, gProperties.getProperty( s ), std::dec);
	else
    {
        if ( gVerbose )
            std::cerr << "[MISSING] Physical Object #" << _id << " (of super type CircleObject) missing footprint radius (integer, >=0). Check default value.\n";
        gProperties.checkAndGetPropertyValue("gPhysicalObjectDefaultFootprintRadius", &_footprintRadius, true);
    }

    double x = 0.0, y = 0.0;
    x = _xReal;
	y = _yReal;
    
    int tries = 0;
    bool randomLocation = false;

    if ( x == -1.0 || y == -1.0 )
    {
        tries = tries + findRandomLocation();
        randomLocation = true;
    }
    else
    {
        if ( canRegister() == false )  // i.e. user-defined location, but cannot register. Pick random.
        {
            std::cerr << "[CRITICAL] cannot use user-defined initial location (" << x << "," << y << ") for physical object #" << getId() << " (localization failed). EXITING.";
            exit(-1);
        }
        randomLocation = false;
    }
    
    if ( gVerbose )
    {
        std::cout << "[INFO] Physical Object #" << getId() << " (of super type CircleObject) positioned at ( "<< std::setw(5) << _xReal << " , " << std::setw(5) << _yReal << " ) -- ";
        if ( randomLocation == false )
            std::cout << "[user-defined position]\n";
        else
        {
            std::cout << "[random pick after " << tries;
            if ( tries <= 1 )
                std::cout << " try]";
            else
                std::cout << " tries]";
            std::cout << "\n";
        }
    }
    
    if ( _visible )
    {
        registerObject();
    }
    
    registered = true;
}


void CircleObject::show() // display on screen (called in the step() method if gDisplayMode=0 and _visible=true)
{
    Sint16 _xCenterPixel = getXCenterPixel();
    Sint16 _yCenterPixel = getYCenterPixel();
    Sint16 sqFootprintRadius = _footprintRadius*_footprintRadius;
    Sint16 sqRadius = _radius*_radius;

    //  draw footprint
    
    Uint8 r = 0xF0;
    Uint8 g = 0xF0;
    Uint8 b = 0xF0;
    Uint32 color = SDL_MapRGBA(gScreen->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = -_footprintRadius; xColor < _footprintRadius; xColor++)
    {
        for (Sint16 yColor = -_footprintRadius; yColor < _footprintRadius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqFootprintRadius)
            {
                putPixel32_secured(gScreen, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA(gScreen->format,_displayColorRed,_displayColorGreen,_displayColorBlue,SDL_ALPHA_OPAQUE);
    
	for (Sint16 xColor = -_radius; xColor < _radius; xColor++)
	{
		for (Sint16 yColor = -_radius; yColor < _radius; yColor++)
		{
			if (xColor*xColor + yColor*yColor < sqRadius)
			{
                putPixel32(gScreen, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
			}
		}
	}
}

void CircleObject::hide()
{
    Sint16 _xCenterPixel = getXCenterPixel();
    Sint16 _yCenterPixel = getYCenterPixel();
    Sint16 sqFootprintRadius = _footprintRadius*_footprintRadius;
    Sint16 sqRadius = _radius*_radius;
    
    //  hide footprint (draw white)
    
    Uint8 r = 0xFF;
    Uint8 g = 0xFF;
    Uint8 b = 0xFF;
    
    Uint32 color = SDL_MapRGBA(gScreen->format,r,g,b,SDL_ALPHA_OPAQUE);
    
    for (Sint16 xColor = -_footprintRadius; xColor < _footprintRadius; xColor++)
    {
        for (Sint16 yColor = -_footprintRadius; yColor < _footprintRadius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqFootprintRadius)
            {
                putPixel32_secured(gScreen, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
    
    // hide object (draw white)
    
    for (Sint16 xColor = -_radius; xColor < _radius; xColor++)
    {
        for (Sint16 yColor = -_radius; yColor < _radius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqRadius)
            {
                putPixel32(gScreen, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
}

bool CircleObject::canRegister( Sint16 __x, Sint16 __y )
{
    Sint16 sqFootprintRadius = _footprintRadius*_footprintRadius;
    Sint16 sqRadius = _radius*_radius;
    // test shape
    for (Sint16 xColor = -_radius; xColor < _radius; xColor++)
    {
        for (Sint16 yColor = -_radius; yColor < _radius; yColor ++)
        {
            if (xColor*xColor + yColor*yColor < sqRadius)
            {
                Uint32 pixel = getPixel32_secured( gEnvironmentImage, xColor+__x, yColor+__y);
                if ( pixel != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) {
                    // if we touched an object, tell it
                    Uint8 r, g, b;
                    SDL_GetRGB(pixel,gEnvironmentImage->format,&r,&g,&b);
                    
                    int targetIndex = (r<<16)+(g<<8)+b;
                    
                    if ( targetIndex >= gPhysicalObjectIndexStartOffset && targetIndex < gRobotIndexStartOffset && gMovableObjects)   // physical object
                    {
                        targetIndex = targetIndex - gPhysicalObjectIndexStartOffset;
                        gPhysicalObjects[targetIndex]->isPushed(_id, Point2d(_desiredLinearSpeed, _desiredSpeedOrientation));
                    } else if (targetIndex < gRobotIndexStartOffset) {
                        _hitWall = true;
                    }
                    return false; // collision!
                }
            }
        }
    }
    
    // We don't want to do the footprint check if objects can move
    if (gMovableObjects)
        return true;
    
    //  test footprint (pixels from both ground image and environment image must be empty)
    for (Sint16 xColor = -_footprintRadius; xColor < _footprintRadius; xColor++)
    {
        for (Sint16 yColor = -_footprintRadius; yColor < _footprintRadius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqFootprintRadius)
            {
                Uint32 pixelFootprint = getPixel32_secured( gFootprintImage, xColor + __x, yColor + __y);
                Uint32 pixelEnvironment = getPixel32_secured( gEnvironmentImage, xColor + __x, yColor + __y);
                if (
                    pixelEnvironment != SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ||
                    ( gFootprintImage_restoreOriginal == true  && pixelFootprint != getPixel32_secured( gFootprintImageBackup, xColor + __x, yColor + __y) ) || // case: ground as initialized or rewritten (i.e. white)
                    ( gFootprintImage_restoreOriginal == false && pixelFootprint != SDL_MapRGBA( gFootprintImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE ) ) // case: only white ground
                    )
                    return false; // collision!
            }
        }
    }
    
    return true;
}

bool CircleObject::canRegister()
{
    return canRegister(getXCenterPixel(), getYCenterPixel());
}

void CircleObject::registerObject()
{
    int id_converted = _id + gPhysicalObjectIndexStartOffset;
    Sint16 _xCenterPixel = getXCenterPixel();
    Sint16 _yCenterPixel = getYCenterPixel();
    Sint16 sqFootprintRadius = _footprintRadius*_footprintRadius;
    Sint16 sqRadius = _radius*_radius;
    
    //  draw footprint
    
    Uint32 color = SDL_MapRGBA( gFootprintImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = -_footprintRadius; xColor < _footprintRadius; xColor++)
    {
        for (Sint16 yColor = -_footprintRadius; yColor < _footprintRadius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqFootprintRadius)
            {
                putPixel32_secured(gFootprintImage, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
    
    // draw object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, (Uint8)((id_converted & 0xFF0000)>>16), (Uint8)((id_converted & 0xFF00)>>8), (Uint8)(id_converted & 0xFF), SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = -_radius; xColor < _radius; xColor++)
    {
        for (Sint16 yColor = -_radius; yColor < _radius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqRadius)
            {
                putPixel32_secured(gEnvironmentImage, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
}

void CircleObject::unregisterObject()
{
    Sint16 _xCenterPixel = getXCenterPixel();
    Sint16 _yCenterPixel = getYCenterPixel();
    Sint16 sqFootprintRadius = _footprintRadius*_footprintRadius;
    Sint16 sqRadius = _radius*_radius;
    
    //  clear footprint
    
    Uint32 color = SDL_MapRGBA( gFootprintImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    

    for (Sint16 xColor = -_footprintRadius; xColor < _footprintRadius; xColor++)
    {
        for (Sint16 yColor = -_footprintRadius; yColor < _footprintRadius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqFootprintRadius)
            {
                if ( gFootprintImage_restoreOriginal == true )
                {
                    color = getPixel32_secured( gFootprintImageBackup, xColor+_xCenterPixel, yColor+_yCenterPixel);
                    putPixel32_secured(gFootprintImage, xColor+_xCenterPixel, yColor+_yCenterPixel, color);
                }
                else
                    putPixel32_secured(gFootprintImage, xColor+_xCenterPixel, yColor+_yCenterPixel, color);
            }
        }
    }
    
    // clear object
    
    color = SDL_MapRGBA( gEnvironmentImage->format, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE );
    
    for (Sint16 xColor = -_radius; xColor < _radius; xColor++)
    {
        for (Sint16 yColor = -_radius; yColor < _radius; yColor++)
        {
            if (xColor*xColor + yColor*yColor < sqRadius)
            {
                putPixel32_secured(gEnvironmentImage, xColor + _xCenterPixel, yColor + _yCenterPixel, color);
            }
        }
    }
}

void CircleObject::step()
{
    _hitWall = false;
    _didMove = false;
    if (_impulses.size() > 0)
    {
        //       printf("[DEBUG] Moving object %d\n", _id);
        double impXtot = 0, impYtot = 0, vx, vy, ux, uy;
        
        for (auto& imp : _impulses) {
            // We only want the component of the speed normal to the centers of mass
            // v: agent speed vector, u: agent-object vector
            // impulses are in polar form
            vx = imp.second.x*cos(imp.second.y * M_PI / 180.0);
            vy = imp.second.x*sin(imp.second.y * M_PI / 180.0);
            
            if (imp.first >= gRobotIndexStartOffset) { // robot
                Robot *robot = gWorld->getRobot(imp.first-gRobotIndexStartOffset);
                ux = _xReal - robot->getWorldModel()->getXReal();
                uy = _yReal - robot->getWorldModel()->getYReal();
                
            }
            else // other object
            {
                PhysicalObject *object = gPhysicalObjects[imp.first];
                ux = _xReal - object->getXReal();
                uy = _yReal - object->getYReal();
            }
            double sqnorm = ux*ux + uy*uy;
            impXtot += (vx*ux+vy*uy)*ux/sqnorm;
            impYtot += (vx*ux+vy*uy)*uy/sqnorm;
        }
        
        _desiredLinearSpeed = sqrt(impXtot*impXtot + impYtot*impYtot);
        _desiredSpeedOrientation = atan2(impYtot, impXtot) * 180 / M_PI;
        
        Sint16 newX = _xReal+impXtot;
        Sint16 newY = _yReal+impYtot;
        
        if (newX != getXCenterPixel() || newY != getYCenterPixel()) // we're going to try to move onscreen
        {
            unregisterObject();
            hide();
            
            if (canRegister(newX, newY))
            {
                _xReal += impXtot;
                _yReal += impYtot;
                _didMove = true;
            }
            
            if (_hitWall) { // reappear somewhere else
                registered = false;
                _visible = false;
            }
            else {
                registerObject();
            }
            
        }
        else // silently move offscreen by less than a pixel
        {
            _xReal += impXtot;
            _yReal += impYtot;
        }
        _impulses.clear();
    }
    stepPhysicalObject();
}

void CircleObject::isPushed(int __idAgent, Point2d __speed)
{
    if (_impulses.count(__idAgent) == 0) {
        _impulses.insert(std::pair<int, Point2d>(__idAgent, __speed));
    }
}

void CircleObject::isTouched( int __idAgent )
{
    //do nothing
}

void CircleObject::isWalked( int __idAgent )
{
    //do nothing
}
