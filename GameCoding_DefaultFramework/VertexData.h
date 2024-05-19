#pragma once
struct VertexTextureData {
	Vec3 position = { 0,0,0 };
	Vec2 uv = { 0,0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> desc;
};

struct VertexColorData {
	Vec3 position = { 0,0,0 };
	Color color = { 0,0,0,0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> desc;
};

