#include"Light.h"
#include<imgui/imgui.h>

//コンストラクタ
Light::Light(IWorld* world)
{
	world_ = world;
	name_ = "Light";
	tag_ = "LightTag";
}

//描画
void Light::draw()const {
	static float ambient[] = { 0.0f,0.0,0.0f,1.0f };
	static float diffuse[] = { 1.0f,0.8f,0.3f,1.0f };
	static float specular[] = { 1.0f,0.8f,0.3f,1.0f };
	static float position[] = { -10.0f,20.0f,10.0f,0.0f };

#ifdef _DEBUG
	// ImGuiによるパラメータ調整
	ImGui::Begin("Lighting");
	ImGui::ColorEdit4("ambient", ambient, ImGuiColorEditFlags_Float);
	ImGui::ColorEdit4("diffuse", diffuse, ImGuiColorEditFlags_Float);
	ImGui::ColorEdit4("specular", specular, ImGuiColorEditFlags_Float);
	ImGui::DragFloat4("position", position);
	ImGui::End();
#endif // DEBUG

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//シャドウマップ用のライトの位置を設定
	GSvector3 shadow_map_light_position{ position[0],position[1],position[2] };
	gsSetShadowMapLightPosition(&shadow_map_light_position);
}