#include "SphericalMapRenderer.h"
#include <kvs/Camera>
#include <kvs/ImageObject>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>
#include <kvs/ShaderSource>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new SphericalMapRenderer class.
 *  @param  type [in] rendering type
 */
/*==========================================================================*/
SphericalMapRenderer::SphericalMapRenderer( const SphericalMapRenderer::Type& type )
{
    m_type = type;
}

/*==========================================================================*/
/**
 *  @brief  Destruct the SphericalMapRenderer class.
 */
/*==========================================================================*/
SphericalMapRenderer::~SphericalMapRenderer()
{
}

/*==========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*==========================================================================*/
void SphericalMapRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::ImageObject* image = kvs::ImageObject::DownCast( object );
    if ( !image ) return;

    if ( !m_texture.isCreated() )
    {
        this->create_texture( image );
    }

    if ( !m_shader_program.isCreated() )
    {
        this->create_shader_program();
    }

//    if ( m_type == Centering )
//    {
//        this->center_alignment( camera->windowWidth(), camera->windowHeight() );
//    }

    BaseClass::startTimer();

    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    {
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Disable( GL_TEXTURE_1D );
        kvs::OpenGL::Disable( GL_TEXTURE_3D );
        kvs::OpenGL::Enable( GL_TEXTURE_2D );

        kvs::ProgramObject::Binder shader( m_shader_program );
        kvs::Texture::Binder unit( m_texture, 0 );

        m_shader_program.setUniform( "spherical_map", 0 );
        m_shader_program.setUniform( "image_size", kvs::Vec2( image->width(), image->height() ) );
        m_shader_program.setUniform( "screen_size", kvs::Vec2( camera->windowWidth(), camera->windowHeight() ) );

        kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
        p1.loadIdentity();
        {
            kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
            p2.loadIdentity();
            {
                kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0, 0.0 ), kvs::Vec2( 0.0, 1.0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0, 1.0 ), kvs::Vec2( 0.0, 0.0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0, 1.0 ), kvs::Vec2( 1.0, 0.0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0, 0.0 ), kvs::Vec2( 1.0, 1.0 ) );
                kvs::OpenGL::End();
            }
        }
    }

    BaseClass::stopTimer();
}

void SphericalMapRenderer::create_shader_program()
{
    kvs::ShaderSource vert("spherical_map.vert");
    kvs::ShaderSource frag("spherical_map.frag");
    m_shader_program.build( vert, frag );
}

/*==========================================================================*/
/**
 *  @brief  Creates the texture region on the GPU.
 *  @param  image [in] pointer to the image object
 */
/*==========================================================================*/
void SphericalMapRenderer::create_texture( const kvs::ImageObject* image )
{
    const double width  = image->width();
    const double height = image->height();
    m_initial_aspect_ratio = width / height;
    m_left = 0.0;
    m_right = 1.0;
    m_bottom = 0.0;
    m_top = 1.0;

    if ( image->pixelType() == kvs::ImageObject::Gray8 )
    {
        const size_t nchannels = 1;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
    }
    else if ( image->pixelType() == kvs::ImageObject::Gray16 )
    {
        const size_t nchannels = 1;
        const size_t bytes_per_channel = 2;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
    }
    else if ( image->pixelType() == kvs::ImageObject::Color24 )
    {
        const size_t nchannels = 3;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
    }
    else if ( image->pixelType() == kvs::ImageObject::Color32 )
    {
        const size_t nchannels = 4;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
    }
    else
    {
        kvsMessageError("Unknown pixel color type.");
    }

    kvs::Texture::SetEnv( GL_TEXTURE_ENV_MODE, GL_REPLACE );
    m_texture.create( image->width(), image->height(), image->pixels().data() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates centering parameters.
 *  @param  width [in] image width
 *  @param  height [in] image height
 */
/*==========================================================================*/
void SphericalMapRenderer::center_alignment( const double width, const double height )
{
    const double current_aspect_ratio = width / height;
    const double aspect_ratio = current_aspect_ratio / m_initial_aspect_ratio;
    if( aspect_ratio >= 1.0 )
    {
        m_left = ( 1.0 - aspect_ratio ) * 0.5;
        m_right = ( 1.0 + aspect_ratio ) * 0.5;
        m_bottom = 0.0;
        m_top = 1.0;
    }
    else
    {
        m_left = 0.0;
        m_right = 1.0;
        m_bottom = ( 1.0 - 1.0 / aspect_ratio ) * 0.5;
        m_top = ( 1.0 + 1.0 / aspect_ratio ) * 0.5;
    }
}

} // end of namespace kvs
