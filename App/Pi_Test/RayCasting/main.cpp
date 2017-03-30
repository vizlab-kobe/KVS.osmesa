#include <iostream>
#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/HydrogenVolumeData>
#include <kvs/RayCastingRenderer>

int main( int argc, char** argv )
{
    std::cout << "OSMesa version: " << kvs::osmesa::Version() << std::endl;

    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vec3u( 64, 64, 64 ) );

    kvs::StructuredVolumeObject* object = volume;
    kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object, renderer );

    screen.draw();
    screen.capture().write( "output.bmp" );

    return 0;
}
