// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_GRID_YASPGRIDLEVELITERATOR_HH
#define DUNE_GRID_YASPGRIDLEVELITERATOR_HH

/** \file
 * \brief The YaspLevelIterator class
 */
#include "../../../../dune-common/dune/common/binomialcoeff.hh"


namespace Dune {


  /** \brief Iterates over entities of one grid level
   */
  template<int codim, PartitionIteratorType pitype, class GridImp>
  class YaspLevelIterator :
    public YaspEntityPointer<codim,GridImp>
  {
    //! know your own dimension
    enum { dim=GridImp::dimension };
    //! know your own dimension of world
    enum { dimworld=GridImp::dimensionworld };
    typedef typename GridImp::ctype ctype;
  public:
    typedef typename GridImp::template Codim<codim>::Entity Entity;
    typedef typename GridImp::YGridLevelIterator YGLI;
    typedef typename GridImp::YGrid::Iterator I;

    //! constructor
    YaspLevelIterator (const GridImp * yg, const YGLI & g,
                       typename array<typename GridImp::YGrid, Binomial<dim,codim>::val>::const_iterator ygrid_begin, typename array<typename GridImp::YGrid, Binomial<dim,codim>::val>::const_iterator ygrid_end) :
      YaspEntityPointer<codim,GridImp>(yg, g, ygrid_begin, ygrid_end)
    {
        current_ygrid = ygrid_begin;
    }

    //! copy constructor
    //YaspLevelIterator (const YaspLevelIterator& i) :
        // YaspEntityPointer<codim,GridImp>(i) {}

    //! increment
    void increment()
    {
       if ( this->_it == this->_it.end() )
       {
          if ( ++current_ygrid  == this->_it.end() )
          {
            DUNE_THROW(GridError, "impossible to increment: last element reached");
          }
          this->_it = current_ygrid.begin();
       }
       else
       {
          ++(this->_it);
       }
     }

typename array<typename GridImp::YGrid, Binomial<dim, codim>::val>::const_iterator current_ygrid;
  };

}

#endif   // DUNE_GRID_YASPGRIDLEVELITERATOR_HH
