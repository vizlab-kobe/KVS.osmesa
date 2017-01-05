#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/TornadoVolumeData>
#include <kvs/PolygonObject>
#include <kvs/LineObject>
#include <kvs/TransferFunction>
#include <kvs/Streamline>
#include <kvs/Tubeline>
#include "CubicImages.h"
#include "CubeMapImage.h"
#include "SphericalMapImage.h"

kvs::PolygonObject* GenerateStreamlines()
{
    kvs::StructuredVolumeObject* volume = new kvs::TornadoVolumeData( kvs::Vec3u( 16, 16, 16 ) );

    std::vector<kvs::Real32> v;
    kvs::Vec3i min_coord( 7, 7,  0 );
    kvs::Vec3i max_coord( 10, 10, 15 );
    for ( int k = min_coord.z(); k < max_coord.z(); k++ )
    {
        for ( int j = min_coord.y(); j < max_coord.y(); j++ )
        {
            for ( int i = min_coord.x(); i < max_coord.x(); i++ )
            {
                v.push_back( static_cast<kvs::Real32>(i) );
                v.push_back( static_cast<kvs::Real32>(j) );
                v.push_back( static_cast<kvs::Real32>(k) );
            }
        }
    }
    kvs::PointObject* point = new kvs::PointObject;
    point->setCoords( kvs::ValueArray<kvs::Real32>( v ) );

    const kvs::TransferFunction tfunc( 256 );
    kvs::LineObject* line = new kvs::Streamline( volume, point, tfunc );
    line->setSize( 0.2 );

    delete point;
    delete volume;

    const size_t ndivs = 20;
    kvs::PolygonObject* object = new kvs::Tubeline( line, ndivs );
    delete line;

    return object;
}

int main( int argc, char** argv )
{
    kvs::PolygonObject* object = GenerateStreamlines();

    kvs::osmesa::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.registerObject( object );

    // Camera position in world coordinate system
//    kvs::Vec3 p( 0.0f, 0.0f, 12.0f );
    kvs::Vec3 p( 0.0f, 0.0f, 2.5f );
    {
        CubicImages cubic_images;
        cubic_images.draw( screen, p );
        cubic_images.write( "output" );

//        kvs::ColorImage image = CubeMapImage( cubic_images );
        kvs::ColorImage image = SphericalMapImage( cubic_images );
        image.write( "output.bmp" );
    }

    return 0;
}
