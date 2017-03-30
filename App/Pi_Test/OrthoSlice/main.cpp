#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/HydrogenVolumeData>
#include <kvs/OrthoSlice>
#include <kvs/PolygonObject>

int main( int argc, char** argv )
{
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vec3u( 64, 64, 64 ) );

    const float p = volume->resolution().z() * 0.5f;
    const kvs::OrthoSlice::AlignedAxis a = kvs::OrthoSlice::ZAxis;
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::OrthoSlice( volume, p, a, t );

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object );

    screen.draw();
    screen.capture().write( "output.bmp" );

    return 0;
}
