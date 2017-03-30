#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/HydrogenVolumeData>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>

int main( int argc, char** argv )
{
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vec3u( 64, 64, 64 ) );

    volume->updateMinMaxValues();
    const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
    const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
    const bool d = false;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::Isosurface( volume, i, n, d, t );

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object );

    screen.draw();
    screen.capture().write( "output.bmp" );

    return 0;
}
