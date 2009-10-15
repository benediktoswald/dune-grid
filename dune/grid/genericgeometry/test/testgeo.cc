// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#include <config.h>

//#define DUNE_THROW(E, m) assert(0)
#include <dune/common/exceptions.hh>

#include "../conversion.hh"
#include "../geometry.hh"
#include <dune/common/fmatrix.hh>
#include <dune/common/mpihelper.hh>

#if HAVE_GRAPE
#include <dune/grid/io/visual/grapegriddisplay.hh>
#endif

#include <dune/grid/io/file/dgfparser/dgfgridtype.hh>

#include "testgeo.hh"

using namespace Dune;





// ****************************************************************
//
int phiErr;
int jTinvJTErr;
int volumeErr;
int detErr;
int volErr;
int normalErr;

typedef Topology<GridType> ConversionType;
template <class Geo1,class Geo2>
void testGeo(const Geo1& geo1, const Geo2& geo2) {
  typedef FieldVector<double, Geo1::mydimension> LocalType;
  typedef FieldVector<double, Geo1::coorddimension> GlobalType;
  typedef FieldMatrix<double, Geo1::mydimension, Geo1::coorddimension> JacobianTType;
  typedef FieldMatrix<double, Geo1::coorddimension, Geo1::mydimension> JacobianType;
  typedef FieldMatrix<double, Geo1::mydimension, Geo1::mydimension> SquareType;

  LocalType x1(0.1);
  LocalType x2(geo2.local(geo1.global(x1)));  // avoid problems with twists
  // LocalType x2(0.1);
  // LocalType x1(geo1.local(geo2.global(x2)));  // avoid problems with twists

  // test global
  GlobalType y1=geo1.global(x1);
  GlobalType y2=geo2.global(x2);
  if ((y1-y2).two_norm2()>1e-10) {
    phiErr++;
    std::cout << "Error in PHI: "
              << " G(" << x1 << ") = " << y1
              << " M(" << x2 << ") = " << y2 << " "
              << x1-x2 << " -> " << y1-y2
              << std::endl;
  }

#if 0 // only if geometry also has jacobian method
  // test jacobian inverse transpose
  if (ConversionType::correctJacobian)
  {
    const JacobianType&  JTInv1  = geo1.jacobianInverseTransposed(x1);
    const JacobianType&  JTInv2 =  geo2.jacobianInverseTransposed(x2);
    JacobianType testJTInv(JTInv1);
    testJTInv -= JTInv2;
    if (testJTInv.frobenius_norm2()>1e-10) {
      jTinvJTErr++;
      std::cout << "JTINVJT: \n" << JTInv1 << " " << JTInv2 << std::endl;
    }
    {
      // test jacobian
      const JacobianTType& JTM    = map.jacobianT(x);
      SquareType JTInvJT;
      const int n = GeometryType::coorddimension;
      const int m = GeometryType::mydimension;
      for( int i = 0; i < m; ++i ) {
        for( int j = 0; j < m; ++j ) {
          JTInvJT[ i ][ j ] = 0;
          for( int k = 0; k < n; ++k )
            JTInvJT[ i ][ j ] += JTM[i][k] * JTInvM[k][j];
        }
      }
      if (std::abs(JTInvJT[0][0]-1.)+std::abs(JTInvJT[1][1]-1.)+
          std::abs(JTInvJT[1][0])+std::abs(JTInvJT[0][1])>1e-10) {
        jTinvJTErr++;
        std::cout << "JTINVJT: \n" << JTInvJT << std::endl;
        // std::cout << "***\n" << JT << std::endl;
        // std::cout << "***\n" << JTInv << std::endl;
      }
    }
  }
#endif
  // test integration element
  double det1 = geo1.integrationElement(x1);
  double det2 = geo2.integrationElement(x2);
  if (std::abs(det1-det2)>1e-10) {
    std::cout << " Error in det: G = " << det1 << " M = " << det2 << std::endl;
    detErr++;
  }

  // test volume
  double vol1  = geo1.volume();
  double vol2 =  geo2.volume();
  if (std::abs(vol1-vol2)>1e-10) {
    volErr++;
    std::cout << "volume : G = " << vol1 << " M = " << vol2 << std::endl;
  }
}
// ****************************************************************
//
template <class GridViewType>
void test(const GridViewType& view) {
  typedef typename GridViewType::Grid GridType;
  typedef typename ConversionType::Type TopologyType;
  typedef typename GridViewType ::template Codim<0>::Iterator ElementIterator;

  typedef typename GridViewType :: IntersectionIterator IntersectionIterator;
  typedef typename GridViewType :: Intersection IntersectionType;
  typedef typename ElementIterator::Entity EntityType;
  typedef typename EntityType::Geometry GeometryType;
  typedef FieldVector<double, GeometryType::coorddimension> GlobalType;

  phiErr = 0;
  jTinvJTErr = 0;
  volumeErr = 0;
  detErr = 0;
  volErr = 0;

  ElementIterator eEndIt = view.template end<0>();
  ElementIterator eIt    = view.template begin<0>();
  for (; eIt!=eEndIt; ++eIt) {
    const GeometryType& geoDune = eIt->geometry();
    // GenericGeometryType genericMap(geoDune,typename GenericGeometryType::CachingType(geoDune) );
    GenericGeometry::Geometry< GridType::dimension, GridType::dimensionworld, GridType >
    genericMap( geoDune.type(),geoDune );
    testGeo(geoDune,genericMap);

    // typedef typename GenericGeometryType :: template Codim< 1 > :: SubMapping
    //  SubGeometryType;
    typedef typename GenericGeometry :: Geometry
    < GridType::dimension-1,GridType::dimensionworld,GridType >
    SubGeometryType;

    const IntersectionIterator iend = view.iend( *eIt );
    for( IntersectionIterator iit = view.ibegin( *eIt ); iit != iend; ++ iit )
    {
      typedef FieldVector<double, GeometryType::mydimension> LocalType;
      typedef FieldVector<double, GeometryType::mydimension-1> LocalFaceType;

      const int faceNr = iit->numberInSelf();

      //iit->intersectionSelfLocal();
      LocalFaceType xf( 0.1 );
      LocalType xx( iit->intersectionSelfLocal().global( xf ) );
      const GlobalType &nG = iit->integrationOuterNormal( xf );
      const GlobalType &nM = genericMap.normal( faceNr, xx );
      if( (nG - nM).two_norm2() > 1e-10 )
      {
        std :: cout << "Error in normal: "
                    << "|nG| = " << nG.two_norm() << ", "
                    << "|nM| = " << nM.two_norm() << std :: endl;
        std :: cout << "                 "
                    << "nG = ( " << nG << " ), nM = ( " << nM << " )"
                    << std :: endl;
        ++normalErr;
      }

      SubGeometryType subMap( genericMap, faceNr );
      testGeo( iit->intersectionGlobal(), subMap );
    }
  }
}

int main(int argc, char ** argv, char ** envp)
try
{
  // this method calls MPI_Init, if MPI is enabled
  // MPIHelper & mpiHelper = MPIHelper::instance(argc,argv);
  // int myrank = mpiHelper.rank();

  if (argc<2) {
    std::cerr << "supply grid file as parameter!" << std::endl;
    return 1;
  }

  // create Grid from DGF parser
  GridPtr<GridType> grid( argv[ 1 ] );
  test(grid->leafView());

  if ( phiErr>0) {
    std::cout << phiErr << " errors occured in mapping.phi?" << std::endl;
  }
  else std::cout << "ZERO ERRORS in mapping.phi!!!!!!!" << std::endl;
  if ( detErr>0) {
    std::cout << detErr << " errors occured in mapping.det?" << std::endl;
  }
  else std::cout << "ZERO ERRORS in mapping.det!!!!!!!" << std::endl;
  if ( jTinvJTErr>0) {
    std::cout << jTinvJTErr
              << " errors occured in mapping.jacobianT?" << std::endl;
  }
  else std::cout << "ZERO ERRORS in mapping.jacobianT!!!!!!!" << std::endl;
  if ( volErr>0) {
    std::cout << volErr
              << " errors occured in mapping.volume?" << std::endl;
  }
  else std::cout << "ZERO ERRORS in mapping.volume!!!!!!!" << std::endl;
  if ( normalErr>0) {
    std::cout << normalErr
              << " errors occured in mapping.normal?" << std::endl;
  }
  else std::cout << "ZERO ERRORS in mapping.normal!!!!!!!" << std::endl;
}
catch( const Exception &e )
{
  std :: cerr << e << std :: endl;
  return 1;
}
catch( ... )
{
  std :: cerr << "Unknown exception raised." << std :: endl;
  return 1;
}