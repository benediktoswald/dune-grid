// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_ALU3DGRID_CAPABILITIES_HH
#define DUNE_ALU3DGRID_CAPABILITIES_HH

// only include this code, if ENABLE_ALUGRID is defined
#ifdef ENABLE_ALUGRID

#include <dune/grid/common/capabilities.hh>

/** @file
 *  @author Robert Kloefkorn
 *  @brief Capabilities for 3d ALUGrids
 */

namespace Dune
{

  template< int dim, int dimworld >
  class ALUCubeGrid;

  template< int dim, int dimworld >
  class ALUSimplexGrid;



  namespace Capabilities
  {

    // Capabilities for ALUCubeGrid
    // ----------------------------

    /** \struct isLeafwiseConforming
       \ingroup ALUCubeGrid
     */

    /** \struct IsUnstructured
       \ingroup ALUCubeGrid
     */

    /** \brief ALUCubeGrid has entities for all codimension
       \ingroup ALUCubeGrid
     */
    template< int cdim >
    struct hasEntity< ALUCubeGrid< 3, 3 >, cdim >
    {
      static const bool v = true;
    };

    /** \brief ALUCubeGrid is parallel
       \ingroup ALUCubeGrid
     */
    template<>
    struct isParallel< ALUCubeGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUCubeGrid has conforming level grids
       \ingroup ALUCubeGrid
     */
    template<>
    struct isLevelwiseConforming< ALUCubeGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUCubeGrid has supports hanging nodes
       \ingroup ALUCubeGrid
     */
    template<>
    struct hasHangingNodes< ALUCubeGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUCubeGrid has backup and restore facilities
       \ingroup ALUCubeGrid
     */
    template<>
    struct hasBackupRestoreFacilities< ALUCubeGrid< 3, 3 > >
    {
      static const bool v = true;
    };



    // Capabilities for ALUSimplexGrid
    // -------------------------------

    /** \struct isLeafwiseConforming
       \ingroup ALUSimplexGrid
     */

    /** \struct IsUnstructured
       \ingroup ALUSimplexGrid
     */

    /** \brief ALUSimplexGrid has entities for all codimension
       \ingroup ALUSimplexGrid
     */
    template< int cdim >
    struct hasEntity< ALUSimplexGrid< 3, 3 >, cdim >
    {
      static const bool v = true;
    };

    /** \brief ALUSimplexGrid is parallel
       \ingroup ALUSimplexGrid
     */
    template<>
    struct isParallel< const ALUSimplexGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUSimplexGrid has conforming level grids
       \ingroup ALUSimplexGrid
     */
    template<>
    struct isLevelwiseConforming< ALUSimplexGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUSimplexGrid has supports hanging nodes
       \ingroup ALUSimplexGrid
     */
    template<>
    struct hasHangingNodes< ALUSimplexGrid< 3, 3 > >
    {
      static const bool v = true;
    };

    /** \brief ALUSimplexGrid has backup and restore facilities
       \ingroup ALUSimplexGrid
     */
    template<>
    struct hasBackupRestoreFacilities< ALUSimplexGrid< 3, 3 > >
    {
      static const bool v = true;
    };

  } // end namespace Capabilities

} //end  namespace Dune

#endif // #ifdef ENABLE_ALUGRID

#endif