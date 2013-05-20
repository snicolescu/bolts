// NewBolts.cpp : Defines the entry point for the console application.
//
#include <System/system_win32.h>
#include <Core/logger.h>
#include <Rendering/Rendering.h>
#include <Rendering/VertexBuffer.hpp>
#include <Rendering/GPUProgram.h>
#include <Helpers/MeshBuilder.h>
#include <Helpers/Camera.h>
#include <IO/FileManager.h>
#include <boost/chrono.hpp>
#include <cmath>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace Bolts;

class DemoGame: public Bolts::Input::WindowEventListener, public Bolts::Input::MouseEventListener
{
public:
	DemoGame():m_bExiting(false),m_renderer(){};

	void updateUniforms()
	{
		std::for_each ( m_program->GetUsedUniforms ().begin (), m_program->GetUsedUniforms ().end (),
			[&]( const Rendering::GPUProgram::UniformDesc& uniformDesc){
				if ( uniformDesc.paramName == "u_color"){
					m_program->SetUniformValue ( uniformDesc, vec3( 1.f, 0.5f,0.f));
				}else if ( uniformDesc.paramName == "u_wvp"){
					m_program->SetUniformValue ( uniformDesc, m_wvpMatrix);
				} 
		});
	}

	void loadShaders(){

		Rendering::GPUProgram::ShaderType vShader, fShader;
		std::vector<char> fileBuffer;
		size_t fileSize = Bolts::IO::fileSize( "media/SimpleShaderVS.glsl");

		if (fileSize){
			fileBuffer.resize( fileSize+1);
			//Bolts::IO::readFromFileAsText( "media/testVS.glsl", fileBuffer.data(),  fileSize);
			Bolts::IO::readFromFileAsText( "media/SimpleShaderVS.glsl", fileBuffer.data(),  fileSize);

			//std::string str( fileBuffer.data());
			std::cout << "VS: \n" << fileBuffer.data() << std::endl;

			vShader = Rendering::LoadShader( fileBuffer.data(), fileSize, GL_VERTEX_SHADER);
		}

		fileSize = Bolts::IO::fileSize( "media/SimpleShaderFS.glsl");

		if (fileSize){
			fileBuffer.resize( fileSize+1);
			//Bolts::IO::readFromFileAsText( "media/testFS.glsl", fileBuffer.data(),  fileSize);
			Bolts::IO::readFromFileAsText( "media/SimpleShaderFS.glsl", fileBuffer.data(),  fileSize);
			std::string str( &fileBuffer[0]);
			std::cout << "FS: \n" << str << std::endl;

			fShader = Rendering::LoadShader( fileBuffer.data(), fileSize, GL_FRAGMENT_SHADER);
		}		

		if ( !vShader.IsValid() || !fShader.IsValid())
			return;

		m_program = Rendering::GPUProgram::Create();

		m_program->AttachShader( vShader);
		m_program->AttachShader( fShader);

		m_renderer.EnableProgram ( m_program);

		updateUniforms ();

		m_program->PrintParameterInfo();
	}

	void init(){
		m_System.InitApp();
		m_window1 = m_System.NewWindow( "Shooty Demo", 400, 400, System::Window::S_TITLE | System::Window::S_MAXIMIZE | System::Window::S_CLOSE);
		//m_window2 = m_System.NewWindow( "Demo Application2", 400, 400, System::Window::S_TITLE | System::Window::S_CLOSE | System::Window::S_RESIZEABLE);

		m_System.RegisterWindowEventListener ( this);
		m_System.RegisterMouseEventListener ( this);

		//NOTE: You do not need to manually call DestroyGraphicsContext
		//		Also, closing the context holding window before destroying/transferring the context is bad
		if (m_System.CreateGraphicsContext ( m_window1)){
			LOGINST << Console::F_HINT << "Successfully initialized the graphics context for Window 1" << Console::ENDL;
		}else{
			LOGINST << Console::F_ERR << "Error creating a graphics context" << Console::ENDL;
		}
		m_renderer.Initialize ();

		const int numVerts = 3;

		float triangleVertices1[ numVerts*6] = {
			-1.0f,  1.0f, 0.f, 0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 0.f, 1.0f, 0.0f, 0.0f,
			-1.0f, -1.0f, 0.f, 0.0f, 0.0f, 1.0f
		};

		Rendering::GPUBufferDescription testDesc;
		testDesc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_POSITION, Rendering::BGVT_FLOAT, 3, 0) );
		testDesc.AddStream ( Rendering::GPUBufferDescription::StreamDesc( Rendering::BGSS_COLOR0, Rendering::BGVT_FLOAT, 3, 3*sizeof(float)) );
		m_vbuffer1 = new Rendering::VertexBuffer( testDesc);
		m_vbuffer1->SetNumberOfVertices (numVerts);
		m_vbuffer1->SetData ( Rendering::BUH_STATIC_DRAW, triangleVertices1, sizeof(triangleVertices1), true);

		//m_renderer.EnableVertexBuffer ( m_vbuffer1);

		float triangleVertices2[ numVerts*6] = {
			1.0f, -1.0f, 0.f, 1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.f, 0.0f, 1.0f, 0.0f,
			1.0f,  1.0f, 0.f, 0.0f, 0.0f, 1.0f
		};

		m_vbuffer2 = new Rendering::VertexBuffer( testDesc);
		m_vbuffer2->SetNumberOfVertices (numVerts);
		m_vbuffer2->SetData ( Rendering::BUH_STATIC_DRAW, triangleVertices2, sizeof(triangleVertices2), true);

		m_vbuffer2 = MeshBuilder::GenCubeMesh ( 1.f);
		//m_renderer.EnableVertexBuffer ( m_vbuffer1);

		loadShaders ();
		
	}

	bool update()
	{
		static boost::chrono::system_clock::time_point lastFrame = boost::chrono::system_clock::now();
		/*
		for ( long i = 0; i < 500000; ++i )
			std::sqrt( 123.456L ); // burn some time
		*/
		boost::chrono::duration<double> dtSec = boost::chrono::system_clock::now() - lastFrame;
		lastFrame = boost::chrono::system_clock::now();
		//std::cout <<  "Update:  took " << dtSec.count() << " seconds\n";

		//Captures Events from the specified windows
		//	Events will be triggered now
		m_System.Update( m_window1 );
		//m_System.Update( m_window2 );

		static float val = 0.f;
		val += (float) (dtSec.count());
		if ( val > 1.f) //Reset once per second
			val = 0.f;

		Camera cam;
		cam.SetPosition ( vec3( 0.f, 0.f, 7.f));
		cam.SetTarget (	vec3( 0.f, 0.f, 0.f));
		cam.SetUpVector ( vec3(0.f, 1.f, 0.f));

		mat4 worldMatrix(1.f);
		//worldMatrix = glm::translate ( worldMatrix, vec3( 0.f, 0.f, val));
		worldMatrix = glm::rotate ( worldMatrix, val*360.f, vec3( 0.f, 1.f, 0.f));
		//worldMatrix = glm::rotate ( worldMatrix, -180.f, vec3( 1.f, 0.f, 0.f));

		m_wvpMatrix = cam.GetProjectionMatrix() * cam.GetViewMatrix() * worldMatrix;
		updateUniforms ();

		m_renderer.EnableVertexBuffer ( m_vbuffer1);

		//m_renderer.Draw ( Rendering::BPT_TRIANGLES);
		m_renderer.ResetVertexBuffers ();

		m_renderer.EnableVertexBuffer ( m_vbuffer2);

		m_renderer.Draw ( Rendering::BPT_TRIANGLES);
		m_renderer.ResetVertexBuffers ();

		m_System.SwapWindowBuffers ();
		m_renderer.Clear ();

		return !m_bExiting;
	}

	void HandleWindowEvent (Input::windowEvent_t event){
		LOGINST << Console::C_GREEN << "[WindowEvent] " << event.type << " Values: " <<event.value1 << "x" << event.value2 << Console::ENDL; 

		switch ( event.type){
		case Bolts::Input::WE_CLOSED:
			m_System.CloseWindow ( event.windowHandle );
			break;
		}
	}

	void OnMouseEvent (Input::mouseEvent_t event){
		switch (event.button){
		case Input::B_RIGHT:
			{
				loadShaders ();
				//m_System.EnableFullscreen ( m_window1, Bolts::System::dimension2d_t( 1366,768));
				break;
			}
		}
		//Log << Console::C_GREEN << "[MouseEvent] Button: " << event.button<< " State: " << event.state << " Pos: " << event.mouseX << " | " << event.mouseY << Console::ENDL; 
	}

private:
	//InputManager			m_InputManager;
	System::Win32System		m_System;	
	System::windowHandle_t	m_window1;
	System::windowHandle_t	m_window2;
	bool					m_bExiting;

	Rendering::Renderer		m_renderer;

	Rendering::VertexBufferPtr	m_vbuffer1;
	Rendering::VertexBufferPtr	m_vbuffer2;
	Rendering::GPUProgramPtr	m_program;
	mat4						m_wvpMatrix;
};



int main()//(int argc, char* argv[])
{
	DemoGame game;
	game.init();

	while ( game.update() );

    return 0;
}

