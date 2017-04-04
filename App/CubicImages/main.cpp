#include <KVS.osmesa/Lib/OSMesa.h>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/StructuredVolumeObject>
#include <kvs/TornadoVolumeData>
#include <kvs/PolygonObject>
#include <kvs/LineObject>
#include <kvs/TransferFunction>
#include <kvs/Streamline>
#include <kvs/Tubeline>
#include <kvs/DivergingColorMap>
#include <kvs/Timer>
#include <KVS.osmesa/Lib/CubicImages.h>


kvs::PolygonObject* GenerateStreamlines()
{
    kvs::StructuredVolumeObject* volume = new kvs::TornadoVolumeData( kvs::Vec3u::All( 32 ) );

    std::vector<kvs::Real32> v;
    kvs::Vec3i min_coord( 17, 17, 10 );
    kvs::Vec3i max_coord( 19, 19, 25 );
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

    const kvs::TransferFunction tfunc( kvs::DivergingColorMap::CoolWarm( 256 ) );
    kvs::LineObject* line = new kvs::Streamline( volume, point, tfunc );
    line->setSize( 0.2 );

    delete point;
    delete volume;

    const size_t ndivs = 50;
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
//    kvs::Vec3 p( 0.0f, 0.0f, 2.0f );
//    kvs::Vec3 p( 0.0f, 0.0f, 3.0f );
    kvs::Vec3 p( 0.0f, 0.0f, 0.0f );
    {
        kvs::osmesa::CubicImages cubic_images;

        kvs::Timer timer( kvs::Timer::Start );
        cubic_images.draw( screen, p );
        timer.stop();
        std::cout << "Rendering cubic images: " << timer.sec() << " [sec]" << std::endl;

        timer.start();
        kvs::ColorImage cube = cubic_images.cubeMapImage();
        timer.stop();
        cube.write( "output_cube.bmp" );
        std::cout << "Generating a cube map image: " << timer.sec() << " [sec]" << std::endl;

        timer.start();
        kvs::ColorImage sphe = cubic_images.sphericalMapImage();
        timer.stop();
        sphe.write( "output_sphe.bmp" );
        std::cout << "Generating a spherical map image: " << timer.sec() << " [sec]" << std::endl;
    }

    return 0;
}
