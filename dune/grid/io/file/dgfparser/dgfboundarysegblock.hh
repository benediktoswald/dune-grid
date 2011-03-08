// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_DGF_BOUNDARYSEGBLOCK_HH
#define DUNE_DGF_BOUNDARYSEGBLOCK_HH

#include <cassert>
#include <iostream>
#include <vector>
#include <map>

#include <dune/grid/io/file/dgfparser/dgfbasicblock.hh>


namespace Dune
{

  namespace dgf
  {
    class BoundarySegBlock
      : public BasicBlock
    {
      int dimworld;                    // the dimension of the vertices (is given  from user)
      bool goodline;                   // active line describes a vertex
      std :: vector< unsigned int > p; // active vertex
      int bndid;
      bool simplexgrid;

    public:
      // initialize vertex block and get first vertex
      BoundarySegBlock ( std :: istream &in, int pnofvtx,
                         int pdimworld, bool psimplexgrid );

      // some information
      int get( std :: map< DGFEntityKey< unsigned int>, int > &facemap,
               bool fixedsize,
               int vtxoffset );

      bool ok()
      {
        return goodline;
      }

      int nofbound()
      {
        return noflines();
      }

    private:
      bool next();

      // get coordinates of active vertex
      int operator[] (int i)
      {
        assert(ok());
        assert(linenumber()>=0);
        assert(0<=i && i<dimworld+1);
        return p[i];
      }

      int size()
      {
        return p.size();
      }

    };

  } // end namespace dgf

} // end namespace Dune

#endif
