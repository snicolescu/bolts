#pragma once
#include "BGlobals.h"
#include "BLogger.h"
#include <map>
#include <string>

namespace Bolts {

	/*
	 *	A material is basically an association between a shader description and values for its parameters.
	 */
	//HACK: This may not be the best implementation. May prove very memory inefficient
	class BMaterial {
		public:

			void setParameter( const std::string &paramName, float value ) {
				mParamsF[paramName] = value;
			};
			void setParameter( const std::string &paramName, vec2 value ) {
				mParamsV2[paramName] = value;
			};
			void setParameter( const std::string &paramName, const vec3 &value ) {
				mParamsV3[paramName] = value;
			};
			void setParameter( const std::string &paramName, const vec4 &value ) {
				mParamsV4[paramName] = value;
			};
			void setParameter( const std::string &paramName, const mat3 &value ) {
				mParamsM3[paramName] = value;
			};
			void setParameter( const std::string &paramName, const mat4 &value ) {
				mParamsM4[paramName] = value;
			};
			void setParameter( const std::string &paramName, uint_ value ) {
				mParamsUI[paramName] = value;
			};
			void setParameter( const std::string &paramName, int value ) {
				mParamsI[paramName] = value;
			};
			void setParameter( const std::string &paramName, bool value ) {
				mParamsB[paramName] = value;
			};

			//TODO: I'll have to implement a "isParameterSet" function(s)

			const bool getParameterB( const std::string &paramName ) const {
				return ( mParamsB.count( paramName ) == 1 ? mParamsB[paramName] : false );
			};
			const float getParameterF( const std::string &paramName ) const {
				return ( mParamsF.count( paramName ) == 1 ? mParamsF[paramName] : 0. );
			};
			const uint_ getParameterUI( const std::string &paramName ) const {
				return ( mParamsUI.count( paramName ) == 1 ? mParamsUI[paramName] : 0 );
			};
			const int getParameterI( const std::string &paramName ) const {
				return ( mParamsI.count( paramName ) == 1 ? mParamsI[paramName] : 0 );
			};
			const vec2 getParameterV2( const std::string &paramName ) const {
				return ( mParamsV2.count( paramName ) == 1 ? mParamsV2[paramName] : vec2() );
			};
			const vec3 getParameterV3( const std::string &paramName ) const {
				return ( mParamsV3.count( paramName ) == 1 ? mParamsV3[paramName] : vec3() );
			};
			const vec4 getParameterV4( const std::string &paramName ) const {
				return ( mParamsV4.count( paramName ) == 1 ? mParamsV4[paramName] : vec4() );
			};
			const mat3 getParameterM3( const std::string &paramName ) const {
				return ( mParamsM3.count( paramName ) == 1 ? mParamsM3[paramName] : mat3() );
			};
			const mat4 getParameterM4( const std::string &paramName ) const {
				return ( mParamsM4.count( paramName ) == 1 ? mParamsM4[paramName] : mat4() );
			};

		private:
			std::map< std::string, float> mParamsF;
			std::map< std::string, vec2> mParamsV2;
			std::map< std::string, vec3> mParamsV3;
			std::map< std::string, vec4> mParamsV4;
			std::map< std::string, mat3> mParamsM3;
			std::map< std::string, mat4> mParamsM4;
			std::map< std::string, uint_> mParamsUI;
			std::map< std::string, int> mParamsI;
			std::map< std::string, bool> mParamsB;
	};

	/*
	class BMaterial{
	public:
		static BMaterial* create(boost::shared_ptr< Shaders::BShaderDescription > sdesc){
			if (sdesc){
				return new BMaterial(sdesc);
			}else return NULL;
		};
		//TODO: Add copy constructor

		void setParam( std::string const& paramName, vec2 const& data) { _setParam< vec2, Shaders::B_SD_VEC2>(paramName, data); };
		void setParam( std::string const& paramName, vec3 const& data) { _setParam< vec3, Shaders::B_SD_VEC3>(paramName, data); };
		void setParam( std::string const& paramName, vec4 const& data) { _setParam< vec4, Shaders::B_SD_VEC4>(paramName, data); };
		void setParam( std::string const& paramName, mat4 const& data) { _setParam< mat4, Shaders::B_SD_MAT4>(paramName, data); };
		void setParam( std::string const& paramName, uint_	data) { _setParamByValue< uint_, Shaders::B_SD_TEXTUREID>(paramName, data); };
		void setParam( std::string const& paramName, float_ data) { _setParamByValue< float_, Shaders::B_SD_FLOAT>(paramName, data); };

		//  Returns the value of the specified parameter( by name)
		//	Please note that there's no check
		void* getParam( std::string const& paramName){
			return mShaderData[ paramName];
		};

		boost::shared_ptr< Shaders::BShaderDescription > getShaderDescription() {return mShaderDescription; };

		bool isComplete() const{
			return (mShaderData.size() == mShaderDescription->parameters.size());
		};

		//TODO: Add destructor to remove shaderData
	private:
		BMaterial( boost::shared_ptr<Shaders::BShaderDescription> sdesc):mShaderDescription(sdesc) {};

		//HACK: Make parameters typesafe. Maybe with RTTI
		//		Confirmed. AndroidNDK includes GNU stl, which has RTTI. Doable
		template<typename Data, Shaders::ShaderDataType DataCode>
		void _setParam( std::string const& paramName, Data const& data);

		template<typename Data, Shaders::ShaderDataType DataCode>
		void _setParamByValue( std::string const& paramName, Data data);

		boost::shared_ptr< Shaders::BShaderDescription > mShaderDescription;
		std::map< std::string, void*> mShaderData;
	};
	*/
};
/*
using namespace Bolts;

template<typename Data, Shaders::ShaderDataType DataCode>
void BMaterial::_setParam( std::string const& paramName, Data const& data){

	Shaders::ShaderDataType paramType = mShaderDescription->getParamType( paramName);

	switch( paramType ){
	case _B_SD_NOTFOUND:
		BLogger::error( "[MaterialInstance] Parameter not found: " + paramName);
		return;
	case DataCode:
		break;
	default:
		BLogger::error( "[MaterialInstance] Parameter of incorrect type in shader description: " + paramName);
		return;
	}

	auto storedData = mShaderData.find( paramName );
	if ( storedData != mShaderData.end() ){
		//There's a value for this param already set
		*static_cast<Data*>(storedData->second) = data;
	}else{
		mShaderData[ paramName ] = new Data(data);
	}
};

template<typename Data, Shaders::ShaderDataType DataCode>
void BMaterial::_setParamByValue( std::string const& paramName, Data data){

	Shaders::ShaderDataType paramType = mShaderDescription->getParamType( paramName);

	switch( paramType ){
	case _B_SD_NOTFOUND:
		BLogger::error( "[MaterialInstance] Parameter not found: " + paramName);
		return;
	case DataCode:
		break;
	default:
		BLogger::error( "[MaterialInstance] Parameter of incorrect type in shader description: " + paramName);
		return;
	}

	auto storedData = mShaderData.find( paramName );
	if ( storedData != mShaderData.end() ){
		//There's a value for this param already set
		*static_cast<Data*>(storedData->second) = data;
	}else{
		mShaderData[ paramName ] = new Data(data);
	}
};
*/