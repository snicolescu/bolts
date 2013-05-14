#include "BParticleSystem.h"

using namespace Bolts;

BParticleSystem::BParticleSystem( glm::vec3 pos, PDescription desc ): system_position( pos ), description( desc ), particles_active( 0 )
{
	if ( description.color != glm::vec3( 0.0 ) ) {
		color_data.resize( description.max_number );
	}
	if ( description.accel != glm::vec3( 0.0 ) ) {
		speed_data.resize( description.max_number );
	}
	life_data.resize( description.max_number );
	position_data.resize( description.max_number );
}

void BParticleSystem::update( float_ dt )
{
	uint_ i = 0;
	while ( i < particles_active ) {
		life_data[i] -= dt;
		if ( life_data[i] <= 0 ) { //Particle is dead
			_particleDead( i );
		} else {
			++i;
		}
	}

	if ( description.accel != glm::vec3( 0.0 ) ) {
		for ( i = 0; i < particles_active; ++i ) {
			speed_data[i] += description.accel * dt;
			position_data[i] += speed_data[i] * dt;
		}
	} else {
		if ( description.speed != glm::vec3( 0.0 ) ) {
			for ( i = 0; i < particles_active; ++i ) {
				position_data[i] += description.speed * dt;
			}
		}
	}
}

void BParticleSystem::_particleDead( uint_ i )
{
	assert( i < particles_active );
	particles_active--;
	if ( particles_active == 0 ) {
		return;
	}

	//Switch between the last particle in the list and this one
	if ( color_data.size() != 0 ) {
		color_data[i] = color_data[ particles_active ];
	}
	if ( speed_data.size() != 0 ) {
		speed_data[i] = speed_data[ particles_active ];
	}

	position_data[i] = position_data[ particles_active ];
	life_data[i] = life_data[ particles_active ];
}

void BParticleSystem::spawn_particles( uint_ num )
{
	uint_ max_particles_active = std::min( description.max_number, num + particles_active );

	for ( ; particles_active < max_particles_active ; ++particles_active ) {
		//TODO: Also generate random color data
		if ( description.color != glm::vec3( 0.0 ) ) {
			color_data[ particles_active ] =  BRng::get()->randVec3();    //description.color;
		}

		life_data[ particles_active ] = description.life;
		//TODO: Lookup boost_random, to see what it can do
		float_ spread = description.spread;

		//boost::random::uniform_real_distribution<> dist( 0, description.spread);
		position_data[ particles_active ] = system_position; //+ glm::vec3 ( dist(random_gen) ,dist(random_gen) ,dist(random_gen));

		if ( description.accel != glm::vec3( 0.0 ) ) {
			speed_data[ particles_active ] =  description.speed;
		}
	}
}