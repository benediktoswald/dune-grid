// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_GEOGRID_DATAHANDLE_HH
#define DUNE_GEOGRID_DATAHANDLE_HH

#include <dune/grid/common/datahandleif.hh>

namespace Dune
{

  // GeometryGridDataHandle
  // ----------------------

  template< class Grid, class WrappedCommDataHandle >
  class GeometryGridCommDataHandle
    : public CommDataHandleIF
      < GeometryGridCommDataHandle< Grid, WrappedCommDataHandle >,
          typename WrappedCommDataHandle :: DataType >
  {
    typedef typename remove_const< Grid > :: type :: Traits Traits;

    const Grid &grid_;
    WrappedCommDataHandle &wrappedHandle_;

  public:
    GeometryGridCommDataHandle ( const Grid &grid,
                                 WrappedCommDataHandle &handle )
      : grid_( grid ),
        wrappedHandle_( handle )
    {}

    bool contains ( int dim, int codim ) const
    {
      return wrappedHandle_.contains( dim, codim );
    }

    bool fixedsize ( int dim, int codim ) const
    {
      return wrappedHandle_.fixedsize( dim, codim );
    }

    template< class HostEntity >
    size_t size ( const HostEntity &hostEntity ) const
    {
      const int codimension = HostEntity :: codimension;
      typedef typename Traits :: template Codim< codimension > :: Entity Entity;
      typedef MakeableInterfaceObject< Entity > MakeableEntity;
      typedef typename MakeableEntity :: ImplementationType EntityImpl;

      EntityImpl impl( grid_ );
      impl.setToTarget( hostEntity );
      MakeableEntity entity( impl );

      return wrappedHandle_.size( (const Entity &)entity );
    }

    template< class MessageBuffer, class HostEntity >
    void gather ( MessageBuffer &buffer, const HostEntity &hostEntity ) const
    {
      const int codimension = HostEntity :: codimension;
      typedef typename Traits :: template Codim< codimension > :: Entity Entity;
      typedef MakeableInterfaceObject< Entity > MakeableEntity;
      typedef typename MakeableEntity :: ImplementationType EntityImpl;

      EntityImpl impl( grid_ );
      impl.setToTarget( hostEntity );
      MakeableEntity entity( impl );

      wrappedHandle_.gather( buffer, (const Entity &)entity );
    }

    template< class MessageBuffer, class HostEntity >
    void scatter ( MessageBuffer &buffer, const HostEntity &hostEntity, size_t size )
    {
      const int codimension = HostEntity :: codimension;
      typedef typename Traits :: template Codim< codimension > :: Entity Entity;
      typedef MakeableInterfaceObject< Entity > MakeableEntity;
      typedef typename MakeableEntity :: ImplementationType EntityImpl;

      EntityImpl impl( grid_ );
      impl.setToTarget( hostEntity );
      MakeableEntity entity( impl );

      wrappedHandle_.scatter( buffer, (const Entity &)entity, size );
    }
  };

}

#endif