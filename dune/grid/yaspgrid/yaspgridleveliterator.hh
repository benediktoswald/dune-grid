// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_GRID_YASPGRIDLEVELITERATOR_HH
#define DUNE_GRID_YASPGRIDLEVELITERATOR_HH

/** \file
 * \brief The YaspLevelIterator class
 */

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
    YaspLevelIterator (const GridImp * yg, const YGLI & g, const I& it) :
      YaspEntityPointer<codim,GridImp>(yg,g,it) {}

    //! copy constructor
    YaspLevelIterator (const YaspLevelIterator& i) :
      YaspEntityPointer<codim,GridImp>(i) {}

    //! increment
    /*  void increment()
    {
      if (this->_it == _end)
        {
          if (this->_grid_index == 1)
            {
              DUNE_THROW(GridError, "impossible to increment: grid_index == 1  ");
            }
          ++this->_grid_index;
          _end = _grids[i].end();
          this->_it = _grids[i].begin();
        }
      else
        {
          ++(this->_it);
        }
        }*/

    I _end;
  };

}

#endif   // DUNE_GRID_YASPGRIDLEVELITERATOR_HH
