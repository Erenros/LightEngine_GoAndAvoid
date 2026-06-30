#include "Window.h"
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "Render/Text.h"
#include "Texture.h"
 
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_sdl2.h> 
#include <backends/imgui_impl_sdlrenderer2.h> 

#include "Engine/Entity.h"
#include "Shape.h"
#include "Engine/GameManager.h"

constexpr float32 RENDER_TARGET_WIDTH = 1920.f;
constexpr float32 RENDER_TARGET_HEIGHT = 1080.f;

void Window::Create(const char* pName,int32 width, int32 height, uint32 windowFlags, uint32 rendererFlags, int32 x, int32 y)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		std::cout << "SDL_Init_Error :  " << SDL_GetError() << std::endl;
		return;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Error SDL2_image Initialization";
		return;
	} 

	mp_Window = SDL_CreateWindow(pName, x, y, width, height, windowFlags);
	if (mp_Window == nullptr) {
		std::cerr << "Window failed to create" << std::endl;
		return;
	}

	mp_Renderer = SDL_CreateRenderer(mp_Window, -1, rendererFlags);
	if (mp_Renderer == nullptr) {
		std::cerr << "Renderer failed to create" << std::endl;
		return;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "[Initialisation] : Audio Error : " << SDL_GetError() << std::endl;
		return;
	}

	if (TTF_Init() != 0)
	{
		std::cout << "[Initialisation] : Font Error" << std::endl;
		return;
	}

	// Create a Render Target

	mp_RenderTarget = SDL_CreateTexture(mp_Renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, RENDER_TARGET_WIDTH, RENDER_TARGET_HEIGHT);
	SDL_SetTextureScaleMode(mp_RenderTarget, SDL_ScaleModeLinear);

	if (!mp_RenderTarget)
	{
		std::cout << "CreateTexture failed: " << SDL_GetError() << '\n';
	}

	if (SDL_SetRenderTarget(mp_Renderer, mp_RenderTarget) != 0)
	{
		std::cout << "SetRenderTarget failed: " << SDL_GetError() << '\n';
	}

	SDL_SetRenderDrawColor(mp_Renderer, 255, 0, 0, 255);

	SDL_RenderClear(mp_Renderer);

	SDL_SetRenderTarget(mp_Renderer, NULL);


	//InitImGUI();
}

Vector2f Window::GetMousePositionOnRenderTarget()
{
	Vector2u mousePos = GetMousePosition(); 
	Vector2f windowSize = GetWindowSize();   

	float32 scaleX = RENDER_TARGET_WIDTH / windowSize.x;
	float32 scaleY = RENDER_TARGET_HEIGHT / windowSize.y;

	return Vector2f{
		static_cast<float32>(mousePos.x) * scaleX,
		static_cast<float32>(mousePos.y) * scaleY
	};
}

void Window::InitImGUI()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	 
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(mp_Window, mp_Renderer);
	ImGui_ImplSDLRenderer2_Init(mp_Renderer);
}

void Window::StartImGUIFrame()
{
	/*ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	ImGui::NewFrame();*/
}

void Window::ImGUIUpdate()
{
	/*ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_MenuBar;

	ImGui::Begin("DockRoot", nullptr, flags);
	 
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("Gizmo"))
				GameManager::GetInstance().SetGizmoVisualState();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	 
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	static bool init = false;
	if (!init)
	{
		init = true;
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		ImGuiID center = dockspace_id;

		ImGui::DockBuilderDockWindow("Scene", center);

		ImGui::DockBuilderFinish(dockspace_id);
	}
	 
	ImGui::End();


	if (ImGui::Begin("Scene"))
	{
		ImVec2 avail = ImGui::GetContentRegionAvail();

		const float aspect = 1920.0f / 1080.0f;

		float width = avail.x;
		float height = width / aspect;

		if (height > avail.y)
		{
			height = avail.y;
			width = height * aspect;
		}

		ImVec2 cursor = ImGui::GetCursorPos();

		ImGui::SetCursorPos(ImVec2(
			cursor.x + (avail.x - width) * 0.5f,
			cursor.y + (avail.y - height) * 0.5f
		));

		ImGui::Image((ImTextureID)mp_RenderTarget, ImVec2(width, height));
	}

	ImGui::End();

	ImGui::Render();*/
}

void Window::ClearWindowWithColor(uint8 r, uint8 g, uint8 b, uint8 a)
{
	SDL_SetRenderDrawColor(mp_Renderer, r, g, b, a); 
}

void Window::End(){

	//ImGui_ImplSDLRenderer2_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();


	SDL_DestroyTexture(mp_RenderTarget);
	SDL_DestroyRenderer(mp_Renderer);
	SDL_DestroyWindow(mp_Window);

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Window::Present()
{ 
	//SDL_SetRenderDrawColor(mp_Renderer, 20, 20, 20, 255);
	//SDL_RenderClear(mp_Renderer);

	//ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), mp_Renderer);
	SDL_SetRenderTarget(mp_Renderer, nullptr);
	 
	SDL_SetRenderDrawColor(mp_Renderer, 20, 20, 20, 255);
	SDL_RenderClear(mp_Renderer);
	 
	SDL_RenderCopy(mp_Renderer, mp_RenderTarget, nullptr, nullptr);

	SDL_RenderPresent(mp_Renderer);
}

void Window::Clear()
{
	SDL_SetRenderTarget(mp_Renderer, mp_RenderTarget);

	SDL_SetRenderDrawColor(mp_Renderer, 30, 30, 30, 255);

	SDL_RenderClear(mp_Renderer);
}

void Window::DrawTextOnRenderer(Text* text)
{
	SDL_Texture* texture = text->CreateTexture(this);
	if (texture == nullptr)
		return;

	DrawOnRenderer(texture, nullptr,text->GetSDLRect());
}

void Window::DrawOnRenderer(SDL_Texture* pTexture, SDL_Rect* srcrect, SDL_Rect* dstrect){
	SDL_RenderCopy(mp_Renderer, pTexture, srcrect, dstrect);
}

void Window::Draw(gcle::Shape* pShape)
{ 

	const std::vector<SDL_Vertex*>& verticesPtr = pShape->GetVerticies();

	std::vector<SDL_Vertex> vertices;
	vertices.reserve(verticesPtr.size());
	for (SDL_Vertex* v : verticesPtr)
	{
		vertices.push_back(*v);
	}

	if (pShape->GetTexture() == nullptr)
		SDL_RenderGeometry(mp_Renderer, nullptr, vertices.data(), static_cast<int32>(vertices.size()), pShape->GetIndicies().data(), static_cast<int32>(pShape->GetIndicies().size()));
	else {
		Texture* text = pShape->GetTexture();
		SDL_RenderGeometry(mp_Renderer, pShape->GetTexture()->GetSDLTexture(), vertices.data(), static_cast<int32>(vertices.size()), pShape->GetIndicies().data(), static_cast<int32>(pShape->GetIndicies().size()));
	}
}

bool Window::IsInsideWindow(Entity* entity){ 

	Vector2f camPos = SceneManager::GetInstance().GetCurrentScene()->GetCurrentCamera()->GetPosition();
	float32 margin = 50.f;
	
	AABB entityAABB;
	if (static_cast<int32>(entity->GetRenderShape()->GetRotation()) % 180 != 0) {
		entityAABB = GetRotatedAABB(entity->GetRenderPosition(), { entity->GetRenderShape()->GetWidth(), entity->GetRenderShape()->GetHeight() }, entity->GetRenderShape()->GetRotation() * DEG_TO_RAD);
		entityAABB = { entityAABB.minX - margin, entityAABB.minY - margin, entityAABB.maxX + margin, entityAABB.maxY + margin };
	}
	else
		entityAABB = { entity->GetRenderShape()->GetPosition(0.f, 0.f).x - margin , entity->GetRenderShape()->GetPosition(0.f, 0.f).y - margin , entity->GetRenderShape()->GetPosition(1.f, 1.f).x + margin, entity->GetRenderShape()->GetPosition(1.f, 1.f).y + margin};

	AABB windowAABB = { -margin, -margin, margin + RENDER_TARGET_WIDTH , margin + RENDER_TARGET_HEIGHT };


	return windowAABB.overlaps(entityAABB);
}

void Window::DrawDebug(gcle::Shape* pShape)
{
	std::vector<SDL_FPoint*> pointsPtr;

	switch (pShape->GetShape())
	{
	case gcle::Shapes::Rectangle:
	{
		auto pSh = static_cast<gcle::Rectangle*>(pShape);
		pointsPtr = pSh->GetHollow();
		break;
	} 
	case gcle::Shapes::Circle:
	{
		pointsPtr = (static_cast<gcle::Circle*>(pShape))->GetHollow();
		break;
	} 
	case gcle::Shapes::Triangle:
	{
		pointsPtr = (static_cast<gcle::Triangle*>(pShape))->GetHollow();
		break;
	}

	default:
		break;
	}



	std::vector<SDL_FPoint> points;
	points.reserve(pointsPtr.size());
	for (SDL_FPoint* p : pointsPtr)
	{
		points.push_back(*p);
	}

	SDL_RenderDrawLinesF(mp_Renderer, points.data(), static_cast<int32>(points.size()));
}

Vector2u Window::GetMousePosition()
{
	Vector2u pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}

Vector2f Window::GetWindowSize()
{
	Vector2u size;
	SDL_GetWindowSize(mp_Window, &size.x, &size.y);
	return Vector2f{ static_cast<float32>(size.x), static_cast<float32>(size.y) };
}