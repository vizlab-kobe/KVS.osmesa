#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Streamline>
#include <kvs/TornadoVolumeData>

int main( int argc, char** argv )
{
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::TornadoVolumeData( kvs::Vec3u( 32, 32, 32 ) );

    std::vector<kvs::Real32> v;
    kvs::Vector3i min_coord( 0,  0,  0 );
    kvs::Vector3i max_coord( 20, 20, 30 );
    for ( int k = min_coord.z(); k < max_coord.z(); k+=3 )
    {
        for ( int j = min_coord.y(); j < max_coord.y(); j+=3 )
        {
            for ( int i = min_coord.x(); i < max_coord.x(); i+=3 )
            {
                v.push_back( static_cast<kvs::Real32>(i) );
                v.push_back( static_cast<kvs::Real32>(j) );
                v.push_back( static_cast<kvs::Real32>(k) );
            }
        }
    }
    kvs::PointObject* point = new kvs::PointObject;
    point->setCoords( kvs::ValueArray<kvs::Real32>( v ) );

    const kvs::TransferFunction transfer_function( 256 );
    kvs::LineObject* object = new kvs::Streamline( volume, point, transfer_function );

    delete volume;
    delete point;

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object );

    screen.draw();
    screen.capture().write( "output.bmp" );

    return 0;
}
