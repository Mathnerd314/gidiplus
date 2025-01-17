# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>

from __future__ import print_function

import os

from numericalFunctions import pointwiseXY_C

if( 'CHECKOPTIONS' in os.environ ) :
    options = os.environ['CHECKOPTIONS'].split( )
    if( '-e' in options ) : print( __file__ )

CPATH = '../../../../Test/UnitTesting/integrate'

os.system( 'cd %s; make -s clean; ./integrationXY -v > v' % CPATH )

def skipBlankLines( ls ) :

    i = 0
    for i, l in enumerate( ls ) :
        if( l.strip( ) != '' ) : break
    ls = ls[i:]
    if( ( len( ls ) == 1 ) and ( ls[0].strip( ) == '' ) ) : ls = []
    return( ls )

def getIntegerValue( name, ls ) :

    s = "# %s = " % name
    n = len( s )
    if( ls[0][:n] != s ) : raise Exception( '%s: missing %s info: "%s"' % ( __file__, name, ls[0][:-1] ) )
    value = int( ls[0].split( '=' )[1] )
    return( ls[1:], value )

def getDoubleValue( name, ls ) :

    s = "# %s = " % name
    n = len( s )
    if( ls[0][:n] != s ) : raise Exception( '%s: missing %s info: "%s"' % ( __file__, name, ls[0][:-1] ) )
    value = float( ls[0].split( '=' )[1] )
    return( ls[1:], value )

def compareValues( label, i, v1, v2 ) :

    sv1, sv2 = '%.12e' % v1, '%.12e' % v2
    sv1, sv2 = '%.7e' % float( sv1 ), '%.7e' % float( sv2 )
    if( sv1 != sv2 ) : print( '<%s> <%s>' % ( sv1, sv2 ) )
    if( sv1 != sv2 ) : raise Exception( '%s: values %e and %e diff by %e at %d for label = %s' % ( __file__, v1, v2, v2 - v1, i, label ) )

def getXYData( ls ) :

    ls, length = getIntegerValue( 'length', ls )
    data = [ list( map( float, ls[i].split( ) ) ) for i in range( length ) ]
    data = pointwiseXY_C.pointwiseXY_C( data, initialSize = len( data ), overflowSize = 10 )
    ls = ls[length:]
    ls = skipBlankLines( ls )
    return( ls, data )

def checkIntegration( count, xMin, xMax, data, normedData, sums ) :

    V = data.integrate( xMin, xMax )
    sum = float( sums.split( 'invSum' )[0].split( '=' )[1] )
    compareValues( 'sum', count, V, sum )
    invV = data.integrate( xMax, xMin )
    if( V != -invV ) : raise Exception( '%s: at %d V = %g != -invV = %g' % ( __file__, count, V, invV ) )
    norm = data.normalize( )
    if( len( norm ) != len( normedData ) ) : raise Exception( '%s: at %d len( norm ) = %d != len( normedData ) = %d' % \
        ( __file__, count, len( norm ), len( normedData ) ) )
    for i, xy in enumerate( norm ) :
        compareValues( "x norm", count, xy[0], normedData[i][0] )
        compareValues( "x norm", count, xy[1], normedData[i][1] )
    v = norm.integrate( )
    if( abs( v - 1. ) > 1e-14 ) : raise Exception( '%s: at %d norm = %e != 1.' % ( __file__, count, v ) )

f = open( os.path.join( CPATH, 'v' ) )
ls = f.readlines( )
f.close( )

count = 0
while( len( ls ) ) :
    count += 1
    if( count == 9 ) :
        ls, dummy = getXYData( ls )
        ls, dummy = getXYData( ls )
    ls, xMin = getDoubleValue( 'xMin', ls )
    ls, xMax = getDoubleValue( 'xMax', ls )
    ls, data = getXYData( ls )
    ls, sums = ls[1:], ls[0]
    ls, normedData = getXYData( ls )
    checkIntegration( count, xMin, xMax, data, normedData, sums )
