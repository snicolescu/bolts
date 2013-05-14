#pragma once
#include "BTypes.h"
#include "BSmartPointers.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <tuple>

namespace Bolts {
	namespace Rendering {
		typedef ushort_ ShaderID;
		enum ShaderType { BS_VERTEX, BS_FRAGMENT, BS_GEOMETRY};

		//Constants for asociating vertex data in meshes to inputs in shaders
		enum AttributeBinding {BSA_POSITION = 0, BSA_COLOR, BSA_NORMAL, BSA_TEX0, BSA_TEX1, _BSA_COUNT};
		const char const *AttributeBindingNames[ _BSA_COUNT] = {
			"vPosition",
			"vColor",
			"vNormal",
			"vTexCoords0",
			"vTexCoords1"
		};

		//These parameters can be required by a material's shader, but they are submitted automatically by the Renderer
		enum BuiltinParameter { BSP_MVPMatrix = 0, BSP_MVMatrix, BSP_PMatrix, BSP_Time, BSP_DTime, _BSP_COUNT};
		const char const *BuiltinParameterNames[_BSP_COUNT] = {
			"pMVPMatrix",
			"pMVMatrix",
			"pPMatrix",
			"pTime",
			"pDTime"
		};
		typedef std::tuple< mat4, mat4, mat4, float, float> BuiltinHolder;

		//This class binds material parameters to shader programs
		//At the start of each new frame, or maybe viewport, w.e., the Renderer pipes in all the BuiltinParameter values.
		class BuiltinHolder {
			public:
				BuiltinHolder(): mBuiltins() {};

				template< BuiltinParameter Builtin, typename ValType>
				void setBuiltinValue( ValType val ) {
					mBuiltins.get<Builtin>() = val;
				};

				template< BuiltinParameter Builtin>
				const ValType getBuiltinValue() {
					return mBuiltins.get<Builtin>();
				};

			private:
				BuiltinHolder mBuiltins;
		};

		enum ShaderParamType {	BST_FLOAT, BST_VEC2, BST_VEC3, BST_VEC4,
		                        BST_MAT2, BST_MAT3, BST_MAT4,
		                        BST_UINT, BST_INT, BST_BOOL, BST_UNKNOWNTYPE, BST_NOTFOUND
		                     };

		//The ShaderDescription should contain enough data to fully describe de parameter requirements of a shader
		//When checking the validity of a materialInstance the Renderer uses this structure
		class ShaderDescription: public big_ref_counted_type {
			public:
				struct ParameterDescription {
					ShaderParamType mType;
					ushort_ mArraySize;
				};

				typedef const std::string &ParameterID;
				typedef std::map< std::string, ParameterDescription> ParameterMap;
				typedef std::vector< BuiltinParameter> BuiltinList;

				ShaderDescription( ShaderID sid ): mShaderID( sid ) {};

				const ShaderID getShaderID() const {
					return mShaderID;
				};

				const bool isParameterDefined( ParameterID paramName ) const {
					return ( mCustomParameters.count( paramName ) > 0 );
				};

				//NOTE:May be slow-ish. It currently is.
				const bool isBuiltinRequired( BuiltinParameter builtin ) const {
					return ( std::find( mBuiltins.begin(), mBuiltins.end(), builtin ) != mBuiltins.end() );
				};

				const ParameterDescription getParameterDefinition( ParameterID paramName ) const {
					if ( isParameterDefined( paramName ) ) {
						return mCustomParameters[ paramName];
					} else {
						ParameterDescription pNotFound = { BST_NOTFOUND, 0};
						return pNotFound;
					}
				};

				const ParameterMap &getParameters() const {
					return mCustomParameters;
				};

				const BuiltinList &getBuiltins() const {
					return mBuiltins;
				};

				void addParameterDefinition( ParameterID paramName, ParameterDescription paramDesc ) {
					mCustomParameters[ paramName] = paramDesc;
				};

				void addBuiltinRequirement( BuiltinParameter builtin ) {
					mBuiltins.push_back( builtin );
				};

			private:
				ShaderID mShaderID;

				//TODO: Add optional attribute checking to make sure meshes match shaders.
				//It should work just fine in correct cases right now.
				//std::map< AttributeBinding, bool> mRequiredAttributes;
				ParameterMap mCustomParameters;
				BuiltinList mBuiltins;
				//TODO: Add texture info and support
		};
	};
};