/*
# <<BEGIN-copyright>>
# Copyright 2019, Lawrence Livermore National Security, LLC.
# See the top-level COPYRIGHT file for details.
# 
# SPDX-License-Identifier: MIT
# <<END-copyright>>
*/

#include <iostream>
#include <math.h>

#include "GIDI.hpp"

namespace GIDI {

namespace Settings {

/*! \class Particles
 * Stores a list of Particle instances.
 */

/* *********************************************************************************************************//**
 ***********************************************************************************************************/

Particles::Particles( ) {

}

/* *********************************************************************************************************//**
 ***********************************************************************************************************/

Particles::~Particles( ) {

}

/* *********************************************************************************************************//**
 * Returns a pointer to the Particle in *this* with PoPs id *a_pid*.
 *
 * @param a_pid                     [in]    The PoPs id of the particle to return.
 *
 * @return                                  Pointer to particle if it exists or NULL otherwise.
 ***********************************************************************************************************/

Particle const *Particles::particle( std::string const &a_pid ) const {

    std::map<std::string, Particle>::const_iterator particle = m_particles.find( a_pid );

    if( particle == m_particles.end( ) ) return( NULL );
    return( &(particle->second) );
}

/* *********************************************************************************************************//**
 * Adds the Particle *a_particle* to *this*. If a particle with the same id as *a_particle* exists in *this*
 * the *a_particle* is not added and false is returned.
 *
 * @param a_particle                [in]    The Particle to add to *this* as a particle to transport.
 *
 * @return                                  Return true if particle is add and false otherwise.
 ***********************************************************************************************************/

bool Particles::add( Particle const &a_particle ) {

    std::string const &pid = a_particle.pid( );

    if( m_particles.find( pid ) != m_particles.end( ) ) return( false );
    m_particles.insert( std::pair<std::string, Particle>( pid, Particle( a_particle ) ) );
    return( true );
}

/* *********************************************************************************************************//**
 * Removes the Particle in *this* with PoPs id *a_pid*.
 *
 * @param a_pid                     [in]    The PoPs id of the particle to remove from *this*.
 *
 * @return                                  Return *true* if particle exist is remove and non-zero otherwise.
 ***********************************************************************************************************/

bool Particles::remove( std::string const &a_pid ) {

    std::map<std::string, Particle>::iterator particle = m_particles.find( a_pid );

    if( particle == m_particles.end( ) ) return( false );
    m_particles.erase( a_pid );
    return( true );
}

/* *********************************************************************************************************//**
 * Returns *true* if Particle with id *a_id* is in *this* and false otherwise.
 *
 * @param a_id                      [in]    The PoPs id of the particle to check for in *this*.
 *
 * @return                                  Return 0 if particle exist is remove and non-zero otherwise.
 ***********************************************************************************************************/

bool Particles::hasParticle( std::string const &a_id ) const {

    return( m_particles.find( a_id ) != m_particles.end( ) );
}

/* *********************************************************************************************************//**
 * Process all the data in *this*.
 * This includes determining the mapping between the uncollapsed (specified by *a_label*) and the collapsed multi-group boundaries,
 * and grouping the flux data for each Particle.
 *
 * @param a_protare                 [in]    The Protare whose multi-group data are to accessed.
 * @param a_label                   [in]    The label of the multi-group data to process.
 ***********************************************************************************************************/

void Particles::process( Protare const &a_protare, std::string const &a_label ) {

    if( a_label == "" ) return;

    Styles::MultiGroup const *multiGroupStyle = a_protare.multiGroup( a_label );
    if( multiGroupStyle == NULL ) throw std::runtime_error( "parameters style not found or wrong type." );

    for( std::map<std::string, Particle>::iterator iter = m_particles.begin( ); iter != m_particles.end( ); ++iter ) {
        std::string pid = iter->first;
        Particle *particle = &(iter->second);

        std::vector<double> const &groupBoundaries = multiGroupStyle->groupBoundaries( pid );

        particle->process( groupBoundaries );
    }
}

/* *********************************************************************************************************//**
 * Returns the particle IDs of *this* as a sorted *std::vector<std::string>* list.
 *
 * @param a_orderIsAscending    [in]    If *true* IDs are sorted in ascending order, otherwise IDs are in descending order.
 *
 * @return                              The list of sorted particle IDs.
 ***********************************************************************************************************/

std::vector<std::string> Particles::sortedIDs( bool a_orderIsAscending ) const {

    std::vector<std::string> keys;

    for( std::map<std::string, Particle>::const_iterator iter = m_particles.begin( ); iter != m_particles.end( ); ++iter ) keys.push_back( (*iter).first );

    return( sortedListOfStrings( keys, a_orderIsAscending ) );
}

/* *********************************************************************************************************//**
 * Prints the contents of *this* to std::cout. Mainly used for debugging.
 ***********************************************************************************************************/

void Particles::print( ) const {

    std::cout << "particles:" << std::endl;

    std::vector<std::string> IDs = sortedIDs( );
    for( std::vector<std::string>::const_iterator iter = IDs.begin( ); iter != IDs.end( ); ++iter ) {
        Particle const &particle = m_particles.at( *iter );

        particle.print( "  " );
    }
}

/*! \class Particle
 * Specifies particle information as mainly needed for multi-group transport. For example, the coarse and fine multi-group data.
 */

/* *********************************************************************************************************//**
 * @param a_pid             [in]    Particle id for the particle.
 * @param a_group           [in]    The multi-group boundaries.
 ***********************************************************************************************************/

Particle::Particle( std::string const &a_pid, MultiGroup const &a_group ) :
        m_pid( a_pid ),
        m_multiGroup( a_group ) {

}

/* *********************************************************************************************************//**
 * @param a_particle        [in]    The Particle instance to copy.
 ***********************************************************************************************************/

Particle::Particle( Particle const &a_particle ) :
        m_pid( a_particle.pid( ) ),
        m_multiGroup( a_particle.multiGroup( ) ),
        m_collapseIndices( a_particle.collapseIndices( ) ) {

    for( std::vector<Flux>::const_iterator iter = a_particle.m_fluxes.begin( ); iter != a_particle.m_fluxes.end( ); ++iter ) {
        m_fluxes.push_back( *iter );
    }

    for( std::vector<ProcessedFlux>::const_iterator iter = a_particle.m_processedFluxes.begin( ); iter != a_particle.m_processedFluxes.end( ); ++iter ) {
        m_processedFluxes.push_back( *iter );
    }
}

/* *********************************************************************************************************//**
 ***********************************************************************************************************/

Particle::~Particle( ) {

}

/* *********************************************************************************************************//**
 * Adds a flux at a specified temperature to the list of fluxes. Currently, the temperature of the flux must be 
 * greater than temperatures for the currently listed fluxes.
 *
 * @param a_flux            [in]    The flux to add.
 * @return                          An error code. If not error, returns 0, otherwise returns non-0 value.
 ***********************************************************************************************************/

int Particle::appendFlux( Flux const &a_flux ) {

    double temperature = a_flux.temperature( );
    std::vector<Flux>::iterator iter;

    for( iter = m_fluxes.begin( ); iter != m_fluxes.end( ); ++iter ) {
        if( temperature <= iter->temperature( ) ) break;
    }
    if( iter != m_fluxes.end( ) ) return( 1 );
    m_fluxes.insert( iter, a_flux );
    return( 0 );
}

/* *********************************************************************************************************//**
 * Returns the multi-group flux with its temperature closes to *a_temperature*.

 * @param a_temperature             [in]    The temperature of the desired flux.
 ***********************************************************************************************************/

ProcessedFlux const *Particle::nearestProcessedFluxToTemperature( double a_temperature ) const {

    double priorTemperature, lastTemperature;
    std::vector<ProcessedFlux>::const_iterator iter;

    if( m_processedFluxes.size( ) == 0 ) return( NULL );

    priorTemperature = m_processedFluxes[0].temperature( );
    for( iter = m_processedFluxes.begin( ); iter != m_processedFluxes.end( ); ++iter ) {
        lastTemperature = iter->temperature( );
        if( lastTemperature > a_temperature ) break;
    }
    if( iter == m_processedFluxes.end( ) ) {
        --iter; }
    else {
        if( fabs( lastTemperature - a_temperature ) < fabs( a_temperature - priorTemperature ) ) --iter;
    }
    return( &(*iter) );
}

/* *********************************************************************************************************//**
 * Determines the mapping of fine multi-group boundaries to coarse multi-group boundaries, and calculated multi-group fluxes from *m_fluxes*.
 *
 * @param a_boundaries              [in]    The fine group boundaries.
 * @param a_epsilon                 [in]    Specifies how close a coarse multi-group boundary must be to a fine multi-group boundary to be considered the same boundary.
 ***********************************************************************************************************/

void Particle::process( std::vector<double> const &a_boundaries, double a_epsilon ) {

    if( m_fineMultiGroup.size( ) > 0 ) {
        if( m_fineMultiGroup.size( ) != a_boundaries.size( ) ) throw std::runtime_error( "Redefining particle's fine multi-group of different size not allowed." );
        for( std::size_t i1 = 0; i1 < m_fineMultiGroup.size( ); ++i1 ) {
            if( fabs( m_multiGroup[i1] - a_boundaries[i1] ) > a_epsilon * m_multiGroup[i1] ) throw std::runtime_error( "Redefining particle's fine multi-group not allowed." );
        }
        return;                                 // Processing already done.
    }

    int i1 = 0, n1 = (int) a_boundaries.size( );

    while( i1 < n1 ) {
        if( fabs( m_multiGroup[0] - a_boundaries[i1] ) <= a_epsilon * a_boundaries[i1] ) break;
        ++i1;
    }
    if( i1 == n1 ) throw std::runtime_error( "groups not compatible: 1" );
    m_collapseIndices.push_back( i1 );
 
    for( int i2 = 1; i2 < (int) m_multiGroup.size( ); ++i2 ) {
        while( i1 < n1 ) {
            if( fabs( m_multiGroup[i2] - a_boundaries[i1] ) <= a_epsilon * a_boundaries[i1] ) break;
            ++i1;
        }
        if( i1 == n1 ) throw std::runtime_error( "groups not compatible: 2" );
        m_collapseIndices.push_back( i1 );
    }

    for( std::size_t i2 = 0; i2 < m_fluxes.size( ); ++i2 ) {
        ProcessedFlux __processedFlux( m_fluxes[i2].process( a_boundaries ) );
        m_processedFluxes.push_back( __processedFlux );
    }

    m_fineMultiGroup.set( "", a_boundaries );
}

/* *********************************************************************************************************//**
 * Print Particle data to std::cout. Mainly for debugging.
 *
 * @param a_indent          [in]    The indent for each printed line.
 ***********************************************************************************************************/

void Particle::print( std::string const &a_indent ) const {

    std::string indent2( "  " );
    indent2 += a_indent;

    std::cout << a_indent << "particle: id = " << m_pid << std::endl;
    m_multiGroup.print( indent2 );

    std::cout << indent2;
    for( std::vector<int>::const_iterator iter = m_collapseIndices.begin( ); iter != m_collapseIndices.end( ); ++iter ) std::cout << " " << *iter;
    std::cout << std::endl;

}

/*! \class ProcessedFlux
 * Stores multi-group flux data for a specified material temperature.
 *
 * Currently, only supports l=0 flux. This needs to be fixed.
 */

/* *********************************************************************************************************//**
 * @param a_temperature
 * @param a_multiGroupFlux
 ***********************************************************************************************************/

ProcessedFlux::ProcessedFlux( double a_temperature, std::vector<double> const &a_multiGroupFlux ) :
    m_temperature( a_temperature ), 
    m_multiGroupFlux( a_multiGroupFlux ) {

}

/* *********************************************************************************************************//**
 * @param a_processedFlux   [in]    The ProcessedFlux instance to copy.
 ***********************************************************************************************************/

ProcessedFlux::ProcessedFlux( ProcessedFlux const &a_processedFlux ) :
    m_temperature( a_processedFlux.temperature( ) ), 
    m_multiGroupFlux( a_processedFlux.multiGroupFlux( ) ) {

}

/* *********************************************************************************************************//**
 ***********************************************************************************************************/

/*
=========================================================
*/
ProcessedFlux::~ProcessedFlux( ) {

}

}

}