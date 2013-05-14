#pragma once
#include "BGlobals.h"
#include <glm/glm.hpp>

#include <vector>
#include "BRng.h"

namespace Bolts {
	class BParticleSystem {
		public:
			typedef ushort_ RID;

			/*	THOUGHTS ON PARTICLES
			*	All particle types have at least one common parameter, position.
			*	After that come parameters that can differ from type to type
			*	Color, Speed, Acceleration, etc.
			*		Each of these parameters can be static/dynamic and global/local.
			*	Support for custom parameters allows for a much greater variety of effects
			*	Based on this definition of their parameters, particles need different shaders.
			*
			*	Also to consider is where to perform calculations, on the CPU or GPU
			*		CPU pros:
						Can also take into account collision data
						More flexible system ( can add Attractors, Destructors, Converters, etc.)
						Frees GPU time
					CPU cons:
						Much more expensive, especially on embedded systems with no FPU

					GPU pros:
						Much much faster
					GPU cons:
						Not as flexible.

				Come to think of it, one could send simplified collision data to the GPU as a Custom Parameter
			*	TODO:[DESIGN] Particle systems and Particle description.
			*/
			struct PDescription {
				float_ life;//in seconds
				vec3 color;
				vec3 speed;
				vec3 accel;
				uint_ max_number;

				float_ spread;
			};

			BParticleSystem( vec3 position, PDescription desc );

			void update( float_ dt );
			void spawn_particles( uint_ num );

			uint_ activeParticles() const {
				return particles_active;
			};

			const std::vector< vec3 >  &positionData() const {
				return position_data;
			};

		private:
			//static boost::random::mt19937 random_gen;

			PDescription description;
			vec3 system_position;

			//Particle Pool
			std::vector< vec3 > color_data;
			std::vector< vec3 > position_data;
			std::vector< vec3 > speed_data;
			std::vector< float_ > life_data;
			uint_ particles_active;

			void _particleDead( uint_ i );
	};
};