#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/ImageObject>
#include <kvs/ImageRenderer>
#include <kvs/ImageImporter>
#include "SphericalMapRenderer.h"

int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    kvs::glut::Screen screen1( &app );
    screen1.show();
    {
        kvs::ImageObject* object = new kvs::ImageImporter( argv[1] );
        kvs::ImageRenderer* renderer = new kvs::ImageRenderer();
        screen1.registerObject( object, renderer );
        object->print( std::cout );
    }

    kvs::glut::Screen screen2( &app );
    screen2.setPosition( screen1.x() + screen1.width(), screen1.y() );
    screen2.show();
    {
        kvs::ImageObject* object = new kvs::ImageImporter( argv[1] );
        kvs::SphericalMapRenderer* renderer = new kvs::SphericalMapRenderer();
        screen2.registerObject( object, renderer );
    }

    return app.run();
}
